#pragma once

#include <vector>
#include <exception>
#include <algorithm>
#include <functional>
#include "Contract.h"
#include "Matrix2.h"

class CorrelationGenerator {
	Sample::CorrelationMatrix<double> matrix;
public:
	CorrelationGenerator(Sample::CorrelationMatrix<double> matrix);
	void next_sample();
	double wiener(int index);
};

class GBM {
	// initial price
	double S0;
	// volatility
	double sigma;
	// mu - sigma*sigma/2
	double nu;

public:
	GBM (double initial_price, double drift, double volatility);
	~GBM(void);
	double price_at(double time, double wiener_process);
};


class Simulator
{
	Contract contract;
	int sample_count;
	CorrelationGenerator correlation_generator;
	double knock_in_percentage;
public:
	Simulator(Contract contract, int sample_count, double knock_in_percentage, CorrelationGenerator correlation_generator);
	~Simulator(void);
	double short_interest_rate(void);
	double currency_rate(std::string currency1, std::string currency2, double time);
	double least_performing_share(std::vector<double> share_prices, double time);
	double determine_equity_amount(double price, bool knocked_in);
	double equity_amount_sample(void);
	double equity_amount(void);
	double present_value(void);
	double number_of_periods(void);
};

