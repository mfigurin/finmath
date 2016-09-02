#pragma once

#include <string>
#include <vector>
#include <ctime>

class Share
{
public:
	std::string name;
	std::string currency;
	double initial_price;

	Share (std::string name, std::string currency, double initial_price);
	~Share(void);
};


class Contract
{

public:
	tm trade_date;
	tm final_date;
	double notional_amount;
	std::vector<Share> basket;

	Contract(tm trade_date,	tm final_date, double notional_amount,	std::vector<Share> basket);
	~Contract(void);

};

