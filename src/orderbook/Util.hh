#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Order.hh"

namespace orderbook {
class Util {
public:
  static Side GetSide(std::string str);
  static std::string GetSideStr(Side side);
  static std::string GetOrderStatusStr(OrderStatus status);
  static std::shared_ptr<Order> CreateNewOrder(const std::vector<std::string> &input);
  static std::shared_ptr<Order> CreateAmendOrder(const std::vector<std::string> &input);
  static std::shared_ptr<Order> CreateCancelOrder(const std::vector<std::string> &input);
  static bool CreateOrderQuery(const std::vector<std::string> &input, std::string &order_id);
  static bool CreatePricePointQuery(const std::vector<std::string> &input, Side &side, int &level);
};
} // namespace orderbook