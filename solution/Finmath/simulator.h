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
#include "contract_calendar.h"

namespace Sample {
	class RandomGenerator
	{
	public:
		virtual double nextValue() = 0;
	};

	class NormalDistribution : public RandomGenerator
	{
		std::default_random_engine generator_;
		std::normal_distribution<double> distribution_;

	public: 
		NormalDistribution();
		NormalDistribution(double mean, double stdev);

		double nextValue();
	};
}

class CorrelationGenerator 
{
	Sample::CorrelationMatrix& correlation_matrix_;
	Sample::RandomGenerator& generator_;
	Sample::Matrix distribution_;

public:
	CorrelationGenerator(Sample::CorrelationMatrix& matrix, Sample::RandomGenerator& generator = Sample::NormalDistribution());
	void next_sample(void);
	double wiener(int index);
};

class Share
{
	std::string name_;
	std::string currency_;
	double initial_price_;
	double current_price_;

	// volatility
	double sigma_;
	// mu - sigma_*sigma_/2
	double nu_;

public:

	Share(std::string name, std::string currency, double initial_price, double drift, double volatility);
	Share(const Share& share );
	~Share(void);

	void update_current_price(double time, double wiener_process);

	double inline performance_level(void) const {
		return current_price_ / initial_price_;
	}
};

class Simulator
{
	Sample::ContractCalendar& calendar_;
	double notional_amount_;
	std::vector<Share>& basket_;
	CorrelationGenerator& correlation_generator_;
	double knock_in_percentage_;
	int sample_count_;
	double short_interest_rate_;

public:
	Simulator(Sample::ContractCalendar& calendar, double notional_amount, double short_interest_rate, std::vector<Share>& basket, double knock_in_percentage, CorrelationGenerator& correlation_generator);
	~Simulator(void);

	void set_sample_count(int count);
	double currency_rate(std::string currency1, std::string currency2, double time);
	double least_performing_share(std::vector<Share>& basket);
	double determine_equity_amount(double price, bool knocked_in);
	double equity_amount_sample(int* steps_done);
	double equity_amount(void);
	double number_of_periods(void);
	double present_value(void);
};
