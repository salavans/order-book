#include "PricePoint.hh"
#include <iostream>

namespace orderbook {

PricePoint::PricePoint(double price, IPricePointUpdateCallBack *pp_callback) {
  _price = price;
  _pp_callback = pp_callback;
  _total_leaves_quantity = 0;
  _head = nullptr;
  _tail = nullptr;
}

double PricePoint::GetPrice() const {
  return _price;
}
long PricePoint::GetTotalLeaveQty() const {
  return _total_leaves_quantity;
}

bool PricePoint::AggressPricePoint(Order *order) {
  auto next = _head;
  while (next != nullptr) {
    auto traded_size = ExecuteOrder(order, next);
    _total_leaves_quantity -= traded_size;
    if (order->GetLeavesQuantity() == 0) {
      break;
    }
    next = next->_next;
  }
  return true;
}

int PricePoint::ExecuteOrder(Order *aggressor, Order *passive) {
  auto traded_size = 0;
  if (aggressor->GetLeavesQuantity() >= passive->GetLeavesQuantity()) {
    traded_size = passive->GetLeavesQuantity();
  } else {
    traded_size = aggressor->GetLeavesQuantity();
  }
  aggressor->SetTradedSize(traded_size);
  aggressor->SetTradedPrice(aggressor->GetPrice());
  passive->SetTradedSize(traded_size);
  passive->SetTradedPrice(aggressor->GetPrice());

  _pp_callback->OnExecutedOrder(this, aggressor, passive);
  return traded_size;
}

bool PricePoint::AddOrder(Order *order_ptr) {
  AppendOrder(order_ptr);
  _total_leaves_quantity += order_ptr->GetLeavesQuantity();
  _pp_callback->OnAddOrder(this, order_ptr);
  return true;
}

bool PricePoint::AmendOrder(Order *org_order, Order *amend) {
  if (amend->GetSize() > org_order->GetSize()) {
    RemoveOrder(org_order);
    AppendOrder(org_order);
  }
  _total_leaves_quantity -= org_order->GetLeavesQuantity();
  org_order->SetOrderSize(amend->GetSize());
  _total_leaves_quantity += org_order->GetLeavesQuantity();
  _pp_callback->OnAmendOrder(this, org_order);
  return true;
}

bool PricePoint::CancelOrder(Order *order_ptr) {
  RemoveOrder(order_ptr);
  _total_leaves_quantity -= order_ptr->GetLeavesQuantity();
  order_ptr->SetOrderStatus(OS_CANCELD);
  _pp_callback->OnCancelOrder(this, order_ptr);
  return true;
}

int PricePoint::FindOrderLevel(Order *order) {
  int level = 0;
  auto next = _head;
  while (next != nullptr) {
    if (next == order) {
      return level;
    }
    level++;
    next = next->_next;
  }
  return -1;
}

void PricePoint::RemoveOrder(Order *order) {
  if (_head == order) {
    _head = order->_next;
    if (_head != nullptr) {
      _head->_previous = nullptr;
    }
  } else if (_tail == order) {
    _tail = order->_previous;
    _tail->_next = nullptr;
  }
  if (order->_previous != nullptr) {
    order->_previous->_next = order->_next;
  }
  if (order->_next != nullptr) {
    order->_next->_previous = order->_previous;
  }
  order->_next = nullptr;
  order->_previous = nullptr;
}
void PricePoint::AppendOrder(Order *order) {
  if (_head == nullptr) {
    _head = order;
    _head->_previous = nullptr;
    _head->_next = nullptr;
    _tail = nullptr;
  } else {
    if (_tail != nullptr) {
      _tail->_next = order;
      order->_previous = _tail;
      _tail = order;
      _tail->_next = nullptr;
    } else {
      _tail = order;
      _head->_next = _tail;
      _tail->_previous = _head;
      _tail->_next = nullptr;
    }
  }
}

std::ostream &operator<<(std::ostream &out, const PricePoint *pricepoint) {
  std::string str = "";
  auto next = pricepoint->_head;
  while (next != nullptr) {
    str += "(" + next->GetOrderId() + ":" + std::to_string(next->GetLeavesQuantity()) + ")";
    next = next->_next;
  }
  out << "(Price:" << pricepoint->_price << ")"
      << "[" << str << "]";
  return out;
}
}; // namespace orderbook