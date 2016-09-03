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

	// Base class to generate random numbers.
	// Implement a custom generator to implement unit tests with a predefine generation strategy
	class RandomGenerator
	{
	public:
		virtual double nextValue() = 0;
	};
	
	// NormalDistribution implements the normal distribution generator.
	// By default the same seed is used by simulation tasks to generate reproducable result.
	// Initialize the NormalDistribution instance with a ramdom seed to generate a unique random sequence.
	class NormalDistribution : public RandomGenerator
	{
		std::default_random_engine generator_;
		std::normal_distribution<double> distribution_;
		void setup(double, double, int); 

	public: 
		NormalDistribution(int seed = NULL);
		NormalDistribution(double mean, double stdev, int seed = NULL);

		double nextValue();
	};

	// CorrelationGenerator converts independent random numbers to the tuples of the correlated numbers.
	// Cholesky decomposition of the correlation matrix is used.
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

	// Share represents an Underlying item of the Shares basket
	class Share
	{
		std::string name_;
		std::string currency_;
		double initial_price_;
		
		double mu_;  // drift		
		double sigma_;  // volatility
		double nu_;  // used to avoid redundant calculations: nu = mu - sigma_*sigma_/2  
		double current_price_;  // simulator sets this field while simulating the price motion

		// This function prints the Share instance properties.
		// Use the following statement to print the properties to console:
		//
		//    std::cout << share 
		//
		friend std::ostream& operator<<(std::ostream&, const Share&); 
	public:

		Share(std::string name, std::string currency, double initial_price, double drift, double volatility);
		Share(const Share& share );
		~Share(void);

		void update_current_price(double time, double wiener_process_sample);

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
	
	// Implements Monte Carlo simulation of the correlated processes.
	//
	// To get the present value of the specified contract create a simulator with the contract parameters,
	// and call sim.simulate_present_value() 
	//
	// See finmath.cpp example to learn how to initialize the underlying basket and the correlation generator.
	
	class Simulator
	{
		ContractCalendar& calendar_;
		double notional_amount_;
		std::vector<Share>& basket_;
		CorrelationGenerator& correlation_generator_;
		double knock_in_percentage_;
		double short_interest_rate_;
		int sample_count_;  // number of simulations. Default value is 10000

		double currency_rate(std::string currency1, std::string currency2, double time);
		double number_of_periods(void);
		double least_performing_share(std::vector<Share>& basket);
		double equity_amount_sample(int* steps_done, bool store_data);
		double determine_equity_amount(double price, bool knocked_in);
		
		// Debugging properties and functions

		bool jump_to_final_date_;	
		bool check_knock_in_event_;
		int stored_iteration_index_;
		std::vector<std::vector<double>> stored_iteration_data_;
		std::string data_file_name_;
		bool suppress_party_A_payments_;	
		bool suppress_party_B_payments_;	

		// Writes one iteration data to .csv file
		void write_iteration_data();

		// Prints the Simulator instance properties.
		// Use the following statement to print the properties to console:
		//
		//    std::cout << sim 
		//
		friend std::ostream& operator<<(std::ostream&, const Simulator&);
	
	public:

		Simulator(ContractCalendar& calendar, double notional_amount, double short_interest_rate, std::vector<Share>& basket, double knock_in_percentage, CorrelationGenerator& correlation_generator);
		~Simulator(void);

		// Simulate equity amount multiple times and return the average value
		double simulate_equity_amount(void);

		// Simulate Present Value using equity amount as Future Value
		double simulate_present_value(void);

		// Simulator calculates the equity amount 10000 times by default.
		// Use this function to change the default value before starting the simulation
		void set_sample_count(int count);

		// Simulator can save details of one iteration to .csv file
		// .csv file can be used later to draw charts in Excel or for debuging purpose
		void save_iteration_data(int index, std::string file_name);
		
		// Can be used to turn off knock-in event checking for debug purposes
		void check_knock_in_event(bool mode);
		
		// Simulation jumps to the final contract date if the knock-in event happened.
		// Call sim.jump_to_final_date(false) to turn off that optimization.
		void jump_to_final_date(bool mode);

		// Payments a party to pay can be suppressed.
		// This feature should be used for debugging purpose only
		void suppress_payments(bool suppress_party_A_payments, bool suppress_party_B_payments);
	};
}
