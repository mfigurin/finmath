#include "stdafx.h"
#include "Contract.h"

Share::Share(std::string name, std::string currency, double initial_price) :
	name(name), 
	currency(currency), 
	initial_price(initial_price) {}

Share::~Share(void) {}

Contract::Contract(tm trade_date,	tm final_date, double notional_amount,	std::vector<Share> basket) : 
	trade_date(trade_date),
	final_date(final_date), 
	notional_amount(notional_amount),	
	basket(basket) {};

Contract::~Contract(void)
{
}
