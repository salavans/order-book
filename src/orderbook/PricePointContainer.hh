#pragma once
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#define NUMBER_OF_PRECISION 10000000
#include "PricePoint.hh"
namespace orderbook {
class PricePointContainer : public IPricePointUpdateCallBack {
public:
  PricePointContainer(Side side, IOrderUpdateCallBack *callback);
  bool AggressContainer(Order *order_ptr);
  bool AddOrder(Order *order_ptr);
  bool AmendOrder(Order *org_order, Order *amend);
  bool CancelOrder(Order *order_ptr);
  std::string QueryPricePointByLevel(int level);
  int FindOrderLevel(Order *order_ptr);

  // updates
  void OnAddOrder(PricePoint *pricepoint, Order *order);
  void OnAmendOrder(PricePoint *pricepoint, Order *order);
  void OnCancelOrder(PricePoint *pricepoint, Order *order);
  void OnExecutedOrder(PricePoint *pricepoint, Order *aggressor, Order *order);

private:
  PricePoint *GetPricePoint(int level);

  void AddPricePoint(PricePoint *pricepoint);
  void RemovePricePoint(PricePoint *pricepoint);
  Side _side;
  PricePoint *_pp_head;
  PricePoint *_pp_tail;
  std::map<long long, std::shared_ptr<PricePoint>> _map_price_point;
  IOrderUpdateCallBack *_order_update_callback;

public:
  friend std::ostream &operator<<(std::ostream &out, const std::shared_ptr<PricePointContainer> book_side);
};
}; // namespace orderbook