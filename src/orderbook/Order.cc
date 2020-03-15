#include "Order.hh"
#include "Util.hh"
namespace orderbook {

Order::Order() {
  _next = nullptr;
  _previous = nullptr;

  _order_id = "";
  _price = 0.0;
  _order_quantity = 0;
  _leaves_quantity = 0;
  _executed_size = 0;
  _executed_price = 0.0;
  _side = BUY;
  _order_status = OS_OPEN;
}

void Order::SetOrderId(std::string order_id) {
  _order_id = order_id;
}
void Order::SetSide(Side side) {
  _side = side;
}
void Order::SetPrice(double price) {
  _price = price;
}
void Order::SetOrderSize(int size) {
  _order_quantity = size;
  _leaves_quantity = size - _executed_size;
  if (_order_quantity == _leaves_quantity) {
    _order_status = OS_OPEN;
  }
}
void Order::SetOrderStatus(OrderStatus status) {
  _order_status = status;
}

void Order::SetReason(std::string text) {
  _text = text;
}

void Order::SetTradedSize(int size) {
  _executed_size += size;
  _leaves_quantity = _leaves_quantity - size;
  if (_leaves_quantity == 0) {
    _order_status = OS_FILLED;
  } else {
    _order_status = OS_PFILLED;
  }
}

void Order::SetTradedPrice(double price) {
  _executed_price = price;
}

const std::string &Order::GetOrderId() {
  return _order_id;
}
Side Order::GetSide() {
  return _side;
}
double Order::GetPrice() {
  return _price;
}
int Order::GetSize() {
  return _order_quantity;
}
int Order::GetLeavesQuantity() {
  return _leaves_quantity;
}
OrderStatus Order::GetStatus() {
  return _order_status;
}
int Order::GetExecutedSize() {
  return _executed_size;
}

double Order::GetExecutedPrice() {
  return _executed_price;
}

ExecType Order::GetExecType() {
  return _exec_type;
}

const std::string &Order::GetReason() {
  return _text;
}

std::ostream &operator<<(std::ostream &out, Order *order) {
  out << "Order[" << order->GetOrderId() << "], Size[" << order->GetSize() << "], Price[" << order->GetPrice() << "], Status[" << Util::GetOrderStatusStr(order->GetStatus()) << "], Side[" << Util::GetSideStr(order->GetSide()) << "]"
      << "Leave Quantity[" << order->GetLeavesQuantity() << "]";
  return out;
}

} // namespace orderbook