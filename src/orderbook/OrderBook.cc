#include "OrderBook.hh"

#include "Util.hh"

#include "iostream"

namespace orderbook {
OrderBook::OrderBook() {
  _bid_side = std::make_shared<PricePointContainer>(BUY, this);
  _sell_side = std::make_shared<PricePointContainer>(SELL, this);
  _map_active_order.clear();
  _map_all_order.clear();
}

bool OrderBook::AggressBook(std::shared_ptr<Order> order_ptr) {
  auto book_side = (order_ptr->GetSide() == BUY) ? _sell_side : _bid_side;
  return book_side->AggressContainer(order_ptr.get());
}

bool OrderBook::AddOrder(std::shared_ptr<Order> order_ptr) {
  auto itr = _map_all_order.find(order_ptr->GetOrderId());
  if (itr != _map_all_order.end()) {
    std::cout << "OrderBook::AddOrder Reject!, Duplicate order Id[" << order_ptr->GetOrderId() << "]" << std::endl;
    return false;
  }
  _map_all_order[order_ptr->GetOrderId()] = order_ptr;
  AggressBook(order_ptr);
  if (order_ptr->GetLeavesQuantity() > 0) {
    _map_active_order[order_ptr->GetOrderId()] = order_ptr;
    auto book_side = (order_ptr->GetSide() == BUY) ? _bid_side : _sell_side;
    book_side->AddOrder(order_ptr.get());
  }
  return true;
}

bool OrderBook::AmendOrder(std::shared_ptr<Order> amend) {
  auto itr = _map_active_order.find(amend->GetOrderId());
  if (itr == _map_active_order.end()) {
    std::cout << "OrderBook::AmendOrder AmendReject!, Order not found[" << amend->GetOrderId() << "]" << std::endl;
    return false;
  }
  auto org_order = itr->second;
  if ( (!(amend->GetSize() > 0)) ||(org_order->GetSize() == amend->GetSize())) {
    std::cout << "OrderBook::AmendOrder AmendReject!, Invalid order size" << std::endl;
    return false;
  }
  auto book_side = (org_order->GetSide() == BUY) ? _bid_side : _sell_side;
  return book_side->AmendOrder(org_order.get(), amend.get());
}

bool OrderBook::CancelOrder(std::shared_ptr<Order> order_ptr) {
  auto itr = _map_active_order.find(order_ptr->GetOrderId());
  if (itr == _map_active_order.end()) {
    std::cout << "OrderBook::CancelOrder CancelReject!, Order not found[" << order_ptr->GetOrderId() << "]" << std::endl;
    return false;
  }
  auto org_order = itr->second;
  _map_active_order.erase(order_ptr->GetOrderId());
  auto book_side = (org_order->GetSide() == BUY) ? _bid_side : _sell_side;
  return book_side->CancelOrder(org_order.get());
}

bool OrderBook::QueryOrder(std::string order_id) {
  auto itr = _map_all_order.find(order_id);
  if (itr == _map_all_order.end()) {
    std::cout << "OrderBook::Query Order Rejected!, Order not found[" << order_id << "]" << std::endl;
    return false;
  }
  auto org_order = itr->second;
  auto book_side = (org_order->GetSide() == BUY) ? _bid_side : _sell_side;
  auto level = book_side->FindOrderLevel(org_order.get());
  PrintOrderQuery(itr->second.get(), level);
  return true;
}

void OrderBook::QueryPricePoint(Side side, int level) { //“bid, 1, 12.10, 200
  auto book_side = (side == BUY) ? _bid_side : _sell_side;
  std::cout  << Util::GetSideStr(side) << ", "
            << level << ", " << book_side->QueryPricePointByLevel(level) << std::endl;
}

void OrderBook::OnAddOrder(Order *order) {
  std::cout << "ExecType[NEW], " << order << std::endl;
}
void OrderBook::OnAmendOrder(Order *order) {
  std::cout << "ExecType[REPLACED], " << order << std::endl;
}
void OrderBook::OnCancelOrder(Order *order) {
  std::cout << "ExecType[CANCELED], " << order << std::endl;
}
void OrderBook::OnExecutedOrder(Order *aggressor, Order *order) {
  std::cout << "ExecType[TRADED], " << aggressor
            << ", Exec Price[" << aggressor->GetExecutedPrice() << "], Exec Size[" << aggressor->GetExecutedSize() << "]" << std::endl;

  std::cout << "ExecType[TRADED], " << order
            << ", Exec Price[" << order->GetExecutedPrice() << "], Exec Size[" << order->GetExecutedSize() << "]" << std::endl;
  if(order->GetStatus() == OS_FILLED){
    _map_active_order.erase(order->GetOrderId());
  }
}

void OrderBook::PrintOrderQuery(Order *order, int level) {
  if ((order->GetLeavesQuantity()>0) && level >= 0) {
    std::cout << order->GetOrderId() << ", " << level <<  ", " << Util::GetOrderStatusStr(order->GetStatus()) << ", " << order->GetPrice() << ", " << order->GetLeavesQuantity() << std::endl;
  } else {
    std::cout << order->GetOrderId() << ", " << -1  << ", " << Util::GetOrderStatusStr(order->GetStatus()) << ", " << order->GetPrice() << ", " << order->GetLeavesQuantity() << std::endl;
  }
}

void OrderBook::PrintBook() {
  std::cout << "--------------------------------------------------------------" << std::endl;
  std::cout << "-------------------------OrderBook----------------------------" << std::endl;
  std::cout << "--------------------------------------------------------------" << std::endl;
  std::cout << _bid_side << std::endl;
  std::cout << _sell_side << std::endl;
  std::cout << "--------------------------------------------------------------" << std::endl;
}

}; // namespace orderbook