#pragma once

#include <vector>
#include <exception>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>

#include "Matrix.h"
#include "contractcalendar.h"

namespace Sample {
	class RandomGenerator
	{
	public:
		virtual double nextValue() = 0;
	};

	class NormalDistribution : public RandomGenerator
	{
		std::default_random_engine _generator;
		std::normal_distribution<double> _distribution;

	public: 
		NormalDistribution();
		NormalDistribution(double mean, double stdev);

		double nextValue();
	};
}

class CorrelationGenerator 
{
	Sample::CorrelationMatrix& correlation_matrix;
	Sample::RandomGenerator& generator;
	Sample::Matrix distribution;

public:
	CorrelationGenerator(Sample::CorrelationMatrix& matrix, Sample::RandomGenerator& generator = Sample::NormalDistribution());
	void next_sample();
	double wiener(int index);
};

class Share
{
	// volatility
	double sigma;
	// mu - sigma*sigma/2
	double nu;

public:
	std::string name;
	std::string currency;
	double initial_price;
	double current_price;

	Share (std::string name, std::string currency, double initial_price, double drift, double volatility);
	Share( const Share& share );
	~Share(void);

	void update_current_price(double time, double wiener_process);
};

class Simulator
{
	Sample::ContractCalendar& calendar;
	double notional_amount;
	std::vector<Share>& basket;
	CorrelationGenerator& correlation_generator;
	double knock_in_percentage;
	int sample_count;

	// debug info
	int steps_done;
	double simulated_equity_amount;
	double short_interest_rate;

public:
	Simulator(Sample::ContractCalendar& calendar, double notional_amount, double short_interest_rate, std::vector<Share>& basket, double knock_in_percentage, CorrelationGenerator& correlation_generator);
	~Simulator(void);

	double currency_rate(std::string currency1, std::string currency2, double time);
	double least_performing_share(std::vector<Share>& basket);
	double determine_equity_amount(double price, bool knocked_in);
	double equity_amount_sample();
	double equity_amount(void);
	double number_of_periods(void);
	double present_value(void);
};
