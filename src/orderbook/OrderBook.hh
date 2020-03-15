#pragma once
#include <map>
#include <memory>
#include <string>

#include "Order.hh"
#include "PricePointContainer.hh"
namespace orderbook {
class OrderBook : public IOrderUpdateCallBack {
public:
  OrderBook();
  bool AggressBook(std::shared_ptr<Order> order_ptr);
  bool AddOrder(std::shared_ptr<Order> order_ptr);
  bool AmendOrder(std::shared_ptr<Order> amend);
  bool CancelOrder(std::shared_ptr<Order> order_ptr);
  bool QueryOrder(std::string order_id);
  void QueryPricePoint(Side side, int level);

  //updates
  void OnAddOrder(Order *order);
  void OnAmendOrder(Order *order);
  void OnCancelOrder(Order *order);
  void OnExecutedOrder(Order *aggressor, Order *order);
  void PrintBook();

private:
  void PrintOrderQuery(Order *order, int level);
  std::shared_ptr<PricePointContainer> _bid_side;
  std::shared_ptr<PricePointContainer> _sell_side;
  std::map<std::string, std::shared_ptr<Order>> _map_global_order;
};
}; // namespace orderbook