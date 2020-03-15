#include <iostream>
#include "orderbook/OrderBook.hh"
#include "orderbook/Util.hh"
#include <string>
#include <sstream>

#include <boost/algorithm/string.hpp>



std::shared_ptr<orderbook::OrderBook> _g_orderbook;


bool ProcessInput(std::string order) {
	boost::algorithm::trim(order);
	if(order == "quit"){
		exit(0);
	}
	std::vector<std::string> input;
	boost::algorithm::split(input, order, boost::is_any_of(" "));
	if(input.size() > 0){
		if(input[0] == "order"){
			auto order = orderbook::Util::CreateNewOrder(input);
			if(order != nullptr){
				_g_orderbook->AddOrder(order);
				return true;
			}
		}
		else if(input[0] == "amend"){
			auto order = orderbook::Util::CreateAmendOrder(input);
			if(order != nullptr){
				_g_orderbook->AmendOrder(order);
				return true;
			}
		}else if(input[0] == "cancel"){
			auto order = orderbook::Util::CreateCancelOrder(input);
			if(order != nullptr){
				_g_orderbook->CancelOrder(order);
				return true;
			}
		}
		else if(input[0] == "q"){
			if(input.size() > 1 && input[1] == "order"){
				std::string order_id = "";
				if(orderbook::Util::CreateOrderQuery(input , order_id)){
					_g_orderbook->QueryOrder(order_id);
					return true;
				}
			}
			else if(input.size() > 1 && input[1] == "level"){
				int level= 0;
				orderbook::Side side= orderbook::BUY;
				if(orderbook::Util::CreatePricePointQuery(input ,side , level)){
					_g_orderbook->QueryPricePoint(side, level);
					return true;
				}
			}
	
		}
	}
	return false;
}

void ReadInput(std::istream& in) {
	while(!in.eof()){
		std::stringbuf val;
        in.get(val);
        if(ProcessInput(val.str())==false){
			std::cerr << "Input Error!" << std::endl;
		}
		in.get();
	}
}

int main() {
    _g_orderbook = std::make_shared<orderbook::OrderBook>();
    ReadInput(std::cin);
    return 0;
}