#pragma once

#include <string>
#include <ctime>

class Contract
{
public:
	Contract(void);
	~Contract(void);

};

class Share
{
public:
	std::string name;
	std::string currency;
	double initial_price;
	tm trade_date;
	tm final_date;
	double notional_amount;
	double calc_present_value();
};

