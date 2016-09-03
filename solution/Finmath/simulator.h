#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>

#include "matrix.h"
#include "contract_calendar.h"

using namespace finmath;

namespace finmath {
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
	CorrelationMatrix& correlation_matrix_;
	RandomGenerator& generator_;
	Matrix distribution_;

public:
	CorrelationGenerator(CorrelationMatrix& matrix, RandomGenerator& generator = NormalDistribution());
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

	double inline current_price(void) const {
		return current_price_;
	}

	std::string inline name(void) const {
		return name_;
	}
};

class Simulator
{
	ContractCalendar& calendar_;
	double notional_amount_;
	std::vector<Share>& basket_;
	CorrelationGenerator& correlation_generator_;
	double knock_in_percentage_;
	int sample_count_;
	double short_interest_rate_;

	bool check_knock_in_event_;
	int stored_iteration_index_;
	std::vector<std::vector<double>> stored_iteration_data_;

public:
	Simulator(ContractCalendar& calendar, double notional_amount, double short_interest_rate, std::vector<Share>& basket, double knock_in_percentage, CorrelationGenerator& correlation_generator);
	~Simulator(void);

	void set_sample_count(int count);
	double currency_rate(std::string currency1, std::string currency2, double time);
	double least_performing_share(std::vector<Share>& basket);
	double determine_equity_amount(double price, bool knocked_in);
	double equity_amount_sample(int* steps_done, bool store_data);
	double equity_amount(void);
	double number_of_periods(void);
	double present_value(void);
	void store_iteration(int index);
	std::vector<std::vector<double>> stored_iteration_data();
	void save_iteration_data(std::string file_name);
	void check_knock_in_event(bool mode);
};
