#pragma once
#include <memory>
#include <string>

#include "Order.hh"
namespace orderbook {
class PricePoint;
class IPricePointUpdateCallBack {
public:
  virtual ~IPricePointUpdateCallBack() {}
  virtual void OnAddOrder(PricePoint *pricepoint, Order *order) = 0;
  virtual void OnAmendOrder(PricePoint *pricepoint, Order *order) = 0;
  virtual void OnCancelOrder(PricePoint *pricepoint, Order *order) = 0;
  virtual void OnExecutedOrder(PricePoint *pricepoint, Order *aggressor, Order *order) = 0;
};
class PricePoint {
public:
  PricePoint(double price, IPricePointUpdateCallBack *pp_callback);
  bool AggressPricePoint(Order *order_ptr);
  bool AddOrder(Order *order_ptr);
  bool AmendOrder(Order *org_order, Order *amend);
  bool CancelOrder(Order *order_ptr);
  int FindOrderLevel(Order *order_ptr);
  double GetPrice() const;
  long GetTotalLeaveQty() const;

  PricePoint *_pp_next;
  PricePoint *_pp_previous;

private:
  int ExecuteOrder(Order *aggressor, Order *passive);
  void RemoveOrder(Order *order);
  void AppendOrder(Order *order);
  double _price;
  long _total_leaves_quantity;
  Order *_head;
  Order *_tail;
  IPricePointUpdateCallBack *_pp_callback;

public:
  friend std::ostream &operator<<(std::ostream &out, const PricePoint *pricepoint);
};
}; // namespace orderbook