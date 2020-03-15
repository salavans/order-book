#pragma once
#include <iostream>
#include <string>
namespace orderbook {
enum OrderStatus {
  OS_OPEN = 1,
  OS_FILLED,
  OS_PFILLED,
  OS_CANCELD
};
enum ExecType {
  EX_NEW = 1,
  EX_TRADED,
  EX_REPLACED,
  EX_CANCELD,
  EX_REJECTED
};
enum Side {
  BUY = 1,
  SELL
};

class Order;
class IOrderUpdateCallBack {
public:
  virtual ~IOrderUpdateCallBack() {}
  virtual void OnAddOrder(Order *order) = 0;
  virtual void OnAmendOrder(Order *order) = 0;
  virtual void OnCancelOrder(Order *order) = 0;
  virtual void OnExecutedOrder(Order *aggressor, Order *order) = 0;
};

class Order {
public:
  Order();
  void SetOrderId(std::string order_id);
  void SetSide(Side side);
  void SetPrice(double price);
  void SetOrderSize(int size);
  void SetOrderStatus(OrderStatus status);

  void SetReason(std::string text);
  void SetTradedSize(int size);
  void SetTradedPrice(double price);

  const std::string &GetOrderId();
  Side GetSide();
  double GetPrice();
  int GetSize();
  int GetLeavesQuantity();
  OrderStatus GetStatus();
  int GetExecutedSize();
  double GetExecutedPrice();
  ExecType GetExecType();

  const std::string &GetReason();

  Order *_next;
  Order *_previous;

private:
  std::string _order_id;
  double _price;
  unsigned int _order_quantity;
  unsigned int _leaves_quantity;
  unsigned int _executed_size;
  double _executed_price;
  Side _side;
  OrderStatus _order_status;
  ExecType _exec_type;
  std::string _text;

public:
  friend std::ostream &operator<<(std::ostream &out, Order *order);
};
} // namespace orderbook