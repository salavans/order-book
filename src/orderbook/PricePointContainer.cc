#include "PricePointContainer.hh"
#include <iostream>
#include <math.h>

namespace orderbook {

PricePointContainer::PricePointContainer(Side side, IOrderUpdateCallBack *callback) {
  _side = side;
  _order_update_callback = callback;
  _pp_head = nullptr;
  _pp_tail = nullptr;
}
bool PricePointContainer::AggressContainer(Order *order_ptr) {

  auto next = _pp_head;
  while (next != nullptr) {
    if ((_side == BUY && isgreaterequal(next->GetPrice(), order_ptr->GetPrice())) ||
        (_side == SELL && islessequal(next->GetPrice(), order_ptr->GetPrice()))) {
      next->AggressPricePoint(order_ptr);
      if (order_ptr->GetLeavesQuantity() == 0) {
        break;
      } else {
        auto removed_pp = next;
        next = next->_pp_next;
        RemovePricePoint(removed_pp);
      }
    } else {
      break;
    }
  }
}

bool PricePointContainer::AddOrder(Order *order_ptr) {
  // std::cout << "PricePointContainer::AddOrder Order[" << order_ptr->GetOrderId() << "], price[" << order_ptr->GetPrice() << "]" << std::endl;
  std::shared_ptr<PricePoint> pricepoint = nullptr;
  long long lprice = order_ptr->GetPrice() * NUMBER_OF_PRECISION;
  auto iter = _map_price_point.find(lprice);
  if (iter == _map_price_point.end()) {
    pricepoint = std::make_shared<PricePoint>(order_ptr->GetPrice(), this);
    _map_price_point.insert(std::pair<long long, std::shared_ptr<PricePoint>>(lprice, pricepoint));
    AddPricePoint(pricepoint.get());
  } else {
    pricepoint = iter->second;
  }
  //std::cout << "PricePointContainer::AddOrder PricePoint[" << pricepoint->GetPrice() << "]" << std::endl;
  return pricepoint->AddOrder(order_ptr);
}

bool PricePointContainer::AmendOrder(Order *org_order, Order *amend) {
  long long lprice = org_order->GetPrice() * NUMBER_OF_PRECISION;
  auto iter = _map_price_point.find(lprice);
  if (iter != _map_price_point.end()) {
    return iter->second->AmendOrder(org_order, amend);
  }
  return false;
}

bool PricePointContainer::CancelOrder(Order *order_ptr) {
  auto results = false;
  long long lprice = order_ptr->GetPrice() * NUMBER_OF_PRECISION;
  auto iter = _map_price_point.find(lprice);
  if (iter != _map_price_point.end()) {
    results = iter->second->CancelOrder(order_ptr);
    RemovePricePoint(iter->second.get());
  }
  return results;
}

std::string PricePointContainer::QueryPricePointByLevel(int level) {
  auto pricepoint = GetPricePoint(level);
  if (pricepoint != nullptr) {
    return std::to_string(pricepoint->GetPrice()) + ", " + std::to_string(pricepoint->GetTotalLeaveQty());
  }
  return "PricePoint not found at this level[" + std::to_string(level) + "]";
}

int PricePointContainer::FindOrderLevel(Order *order_ptr) {
  long long lprice = order_ptr->GetPrice() * NUMBER_OF_PRECISION;
  auto iter = _map_price_point.find(lprice);
  if (iter != _map_price_point.end()) {
    return iter->second->FindOrderLevel(order_ptr);
  }
  return -1;
}

PricePoint *PricePointContainer::GetPricePoint(int level) {
  int position = 0;
  auto next = _pp_head;
  while (next != nullptr) {
    if (position == level) {
      return next;
    }
    position++;
    next = next->_pp_next;
  }
  return nullptr;
}
void PricePointContainer::AddPricePoint(PricePoint *pricepoint) {
  //  std::cout << "PricePointContainer::AddPricePoint PricePoint[" << pricepoint->GetPrice() << "]" << std::endl;
  if (_pp_head == nullptr) {
    _pp_head = pricepoint;
    _pp_head->_pp_previous = nullptr;
    _pp_head->_pp_next = nullptr;
    _pp_tail = nullptr;
    return;
  }
  auto pp_next = _pp_head;
  while (pp_next != nullptr) {
    if (_side == BUY) {
      if (isless(pp_next->GetPrice(), pricepoint->GetPrice())) { //(x<y)
        break;
      }
      pp_next = pp_next->_pp_next;
      continue;
    } else {
      if (isgreater(pp_next->GetPrice(), pricepoint->GetPrice())) { //(x>y)
        break;
      }
      pp_next = pp_next->_pp_next;
      continue;
    }
  }
  if (pp_next == nullptr) { //append it below tail
    if (_pp_tail == nullptr) {
      _pp_tail = pricepoint;
      _pp_head->_pp_next = _pp_tail;
      _pp_tail->_pp_previous = _pp_head;
      _pp_tail->_pp_next = nullptr;
    } else {
      _pp_tail->_pp_next = pricepoint;
      pricepoint->_pp_previous = _pp_tail;
      _pp_tail = pricepoint;
      _pp_tail->_pp_next = nullptr;
    }
  } else if (pp_next == _pp_head) {
    pricepoint->_pp_next = _pp_head;
    pricepoint->_pp_previous = nullptr;
    _pp_head->_pp_previous = pricepoint;
    _pp_head = pricepoint;

  } else {
    pricepoint->_pp_previous = pp_next->_pp_previous;
    pp_next->_pp_previous->_pp_next = pricepoint;
    pp_next->_pp_previous = pricepoint;
    pricepoint->_pp_next = pp_next;
  }
}

void PricePointContainer::RemovePricePoint(PricePoint *pricepoint) {
  //std::cout << "PricePointContainer::RemovePricePoint PricePoint[" << pricepoint->GetPrice() << "] Open Qty[" << pricepoint->GetTotalLeaveQty() << "]" << std::endl;
  if (pricepoint->GetTotalLeaveQty() <= 0) {
    if (_pp_head == pricepoint) {
      _pp_head = pricepoint->_pp_next;
      if (_pp_head != nullptr) {
        _pp_head->_pp_previous = nullptr;
      }
    } else if (_pp_tail == pricepoint) {
      _pp_tail = pricepoint->_pp_previous;
      _pp_tail->_pp_next = nullptr;
    }
    if (pricepoint->_pp_previous != nullptr) {
      pricepoint->_pp_previous->_pp_next = pricepoint->_pp_next;
    }
    if (pricepoint->_pp_next != nullptr) {
      pricepoint->_pp_next->_pp_previous = pricepoint->_pp_previous;
    }
    pricepoint->_pp_next = nullptr;
    pricepoint->_pp_previous = nullptr;
    _map_price_point.erase(pricepoint->GetPrice());
  }
}

void PricePointContainer::OnAddOrder(PricePoint *pricepoint, Order *order) {
  _order_update_callback->OnAddOrder(order);
}

void PricePointContainer::OnAmendOrder(PricePoint *pricepoint, Order *order) {
  _order_update_callback->OnAmendOrder(order);
}

void PricePointContainer::OnCancelOrder(PricePoint *pricepoint, Order *order) {
  _order_update_callback->OnCancelOrder(order);
}

void PricePointContainer::OnExecutedOrder(PricePoint *pricepoint, Order *aggressor, Order *order) {
  _order_update_callback->OnExecutedOrder(aggressor, order);
}

std::ostream &operator<<(std::ostream &out, const std::shared_ptr<PricePointContainer> book_side) {
  out << "-------------------------Side[" << (book_side->_side == BUY ? "BUY" : "SELL") << "]-------------------------" << std::endl;
  auto next = book_side->_pp_head;
  while (next != nullptr) {
    out << next << std::endl;
    next = next->_pp_next;
  }
  return out;
}

}; // namespace orderbook