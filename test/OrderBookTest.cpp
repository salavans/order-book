#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE DemoTests
#endif
#include <boost/test/unit_test.hpp>

#include <memory>
#include <string>

#include "orderbook/Order.hh"
#include "orderbook/OrderBook.hh"

using namespace orderbook;
std::shared_ptr<Order> CreateOrder(std::string order_id, Side side, long size, double price) {
  auto order = std::make_shared<Order>();
  order->SetOrderId(order_id);
  order->SetSide(side);
  order->SetOrderSize(size);
  order->SetPrice(price);
  return order;
}

void BOOST_EXPECT_TRUE(bool results){
    BOOST_CHECK(results == true);
}

void BOOST_EXPECT_FALSE(bool results){
    BOOST_CHECK(results == false);
}

BOOST_AUTO_TEST_SUITE(orderbook_suite)

    BOOST_AUTO_TEST_CASE(add_order_test)
    {
        OrderBook book;
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order1", BUY, 100, 10.05)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order2", SELL, 100, 10.10)));
        BOOST_EXPECT_FALSE(book.AddOrder(CreateOrder("order2", SELL, 100, 10.05)));
        BOOST_EXPECT_FALSE(book.AddOrder(CreateOrder("order1", BUY, 100, 10.05)));
        book.PrintBook();
    }

    BOOST_AUTO_TEST_CASE(cancel_order_test)
    {
        OrderBook book;
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order1", BUY, 100, 10.05)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order2", SELL, 100, 10.15)));
        BOOST_EXPECT_TRUE(book.CancelOrder(CreateOrder("order2", SELL, 100, 10.05)));
        BOOST_EXPECT_FALSE(book.CancelOrder(CreateOrder("order2", SELL, 100, 10.05)));
        book.PrintBook();
    }
    
    BOOST_AUTO_TEST_CASE(amend_order_test)
    {
        OrderBook book;
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order1", BUY, 100, 10.05)));
        BOOST_EXPECT_TRUE(book.AmendOrder(CreateOrder("order1", BUY, 200, 10.05)));
        BOOST_EXPECT_TRUE(book.AmendOrder(CreateOrder("order1", BUY, 50, 10.05)));
        BOOST_EXPECT_FALSE(book.AmendOrder(CreateOrder("order1", BUY, 50, 10.05)));
        BOOST_EXPECT_FALSE(book.AmendOrder(CreateOrder("order2", BUY, 500, 10.05)));
        book.PrintBook();
    }

    BOOST_AUTO_TEST_CASE(exec_order_test)
    {
        OrderBook book;
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order1", SELL, 100, 10.05)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order2", SELL, 200, 10.55)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order3", SELL, 50, 10.5)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order4", BUY, 50, 9.5)));
        book.PrintBook();
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order5", BUY, 250, 10.5)));
        book.PrintBook();
    }

    /*
*---BUY--|--Sell--
*100@10.0|200@10.005
*300@ 9.5|100@10.005
*400@ 9.5|200@10.01
*300@9.49
*/
    BOOST_AUTO_TEST_CASE(query_test)
    {
        OrderBook book;
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order1", BUY, 100, 10.0)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order2", BUY, 300, 9.5)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order3", BUY, 400, 9.5)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order4", BUY, 300, 9.49)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order5", SELL, 200, 10.005)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order6", SELL, 100, 10.005)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order7", SELL, 200, 10.01)));
        BOOST_EXPECT_TRUE(book.QueryOrder("order7"));
        BOOST_EXPECT_TRUE(book.QueryOrder("order3"));
        BOOST_EXPECT_FALSE(book.QueryOrder("o3"));
        book.QueryPricePoint(BUY, 0);
        book.QueryPricePoint(BUY, 1);
        book.QueryPricePoint(BUY, 2);
        book.QueryPricePoint(SELL, 0);
        book.QueryPricePoint(SELL, 1);
        book.QueryPricePoint(SELL, 2);
        book.PrintBook();
    }
    BOOST_AUTO_TEST_CASE(cancel_or_amend_terminated_order)
    {
        OrderBook book;
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order1", SELL, 100, 10.05)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order2", SELL, 200, 10.55)));
        BOOST_EXPECT_TRUE(book.AddOrder(CreateOrder("order4", BUY, 100, 10.05)));
        BOOST_EXPECT_FALSE(book.AmendOrder(CreateOrder("order1", SELL, 50, 10.05)));
        BOOST_EXPECT_FALSE(book.CancelOrder(CreateOrder("order1", SELL, 50, 10.05)));
        BOOST_EXPECT_TRUE(book.QueryOrder("order1"));

        book.PrintBook();
    }






BOOST_AUTO_TEST_SUITE_END()