#include "Util.hh"

#include <memory>
#include <string>

namespace orderbook {
Side Util::GetSide(std::string str) {
  if (str == "buy" || str == "bid") {
    return BUY;
  }
    return SELL;
}

std::string Util::GetSideStr(Side side) {
  switch (side) {
  case BUY:
    return "bid";
  case SELL:
    return "ask";
  }
  return "";
}

std::string Util::GetOrderStatusStr(OrderStatus status) {
  switch (status) {
  case OS_OPEN:
    return "OPEN";
  case OS_CANCELD:
    return "CANCELED";
  case OS_PFILLED:
    return "PFILLED";
  case OS_FILLED:
    return "FILLED";
  }
  return "";
}

//order 1001 buy 100 12.30
std::shared_ptr<Order> Util::CreateNewOrder(const std::vector<std::string> &input) {
  if (input.size() == 5) {
    try {
      auto order = std::make_shared<Order>();
      order->SetOrderId(input[1]);
      order->SetSide(Util::GetSide(input[2]));
      order->SetOrderSize(std::stoi(input[3]));
      order->SetPrice(std::stod(input[4]));
      return order;
    } catch (...) {
      return nullptr;
    }
  }
  return nullptr;
}
std::shared_ptr<Order> Util::CreateAmendOrder(const std::vector<std::string> &input) {
  if (input.size() == 3) {
    try {
      auto order = std::make_shared<Order>();
      order->SetOrderId(input[1]);
      order->SetOrderSize(std::stoi(input[2]));
      return order;
    } catch (...) {
      return nullptr;
    }
  }
  return nullptr;
}
std::shared_ptr<Order> Util::CreateCancelOrder(const std::vector<std::string> &input) {
  if (input.size() == 2) {
    try {
      auto order = std::make_shared<Order>();
      order->SetOrderId(input[1]);
      return order;
    } catch (...) {
      return nullptr;
    }
  }
  return nullptr;
}
bool Util::CreateOrderQuery(const std::vector<std::string> &input, std::string &order_id) {
  if (input.size() == 3) {
    try {
      order_id = input[2];
      return true;
    } catch (...) {
      return false;
    }
  }
  return false;
}
bool Util::CreatePricePointQuery(const std::vector<std::string> &input, Side &side, int &level) {
  if (input.size() == 4) {
    try {
      side = Util::GetSide(input[2]);
      level = std::stoi(input[3]);
      return true;
    } catch (...) {
      return false;
    }
  }
  return false;
}

} // namespace orderbook