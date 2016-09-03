#include "stdafx.h"
#include <random>
#include <fstream>
#include "simulator.h"

using namespace finmath;

namespace finmath {

	NormalDistribution::NormalDistribution(double mean, double stddev, int seed) {	
		setup(mean, stddev, seed); 
	}
	NormalDistribution::NormalDistribution(int seed) {
		setup(0.0, 1.0, seed);
	}
	void NormalDistribution::setup(double mean, double stddev, int seed) {
		generator_ = seed != NULL ? std::default_random_engine((unsigned int)seed) : std::default_random_engine();
		distribution_ = std::normal_distribution<double>(mean, stddev);
	}

	double NormalDistribution::nextValue() {
		return distribution_(generator_);
	}


	CorrelationGenerator::CorrelationGenerator(CorrelationMatrix& matrix, RandomGenerator& generator) : 
		correlation_matrix_(matrix), 
		generator_(generator),
		distribution_(Matrix( matrix.rows(), 1)) 
	{
		next_sample();
	};

	void CorrelationGenerator::next_sample(void) {
		for( int i = 0; i < correlation_matrix_.rows(); i++ ) {
			distribution_(i) = generator_.nextValue();
		}
		distribution_ = correlation_matrix_ * distribution_;
	}

	double CorrelationGenerator::wiener(int index) { 
		return distribution_(index);
	}

	Share::Share(std::string name, std::string currency, double initial_price, double drift, double volatility) :
		name_(name), 
		currency_(currency), 
		initial_price_(initial_price),
		sigma_(volatility), 
		nu_(drift-sigma_*sigma_/2)
	{}

	Share::Share( const Share& share ) {
		name_ = share.name_;
		currency_ = share.currency_;
		initial_price_ = share.initial_price_;
		sigma_ = share.sigma_;
		nu_ = share.nu_;
	}

	Share::~Share(void) {}

	void Share::update_current_price(double time, double wiener_process){
		current_price_ = initial_price_ * exp(nu_ * time + sigma_ * wiener_process);
	}

	Simulator::Simulator(ContractCalendar& calendar, double notional_amount, double short_interest_rate, std::vector<Share> &basket, double knock_in_percentage, CorrelationGenerator& correlation_generator) : 
		calendar_(calendar),
		notional_amount_(notional_amount),
		short_interest_rate_(short_interest_rate),
		basket_(basket), 
		knock_in_percentage_(knock_in_percentage),
		correlation_generator_(correlation_generator),
		sample_count_(10000),
		stored_iteration_index_(-1),
		check_knock_in_event_(true)
	{}

	Simulator::~Simulator(void) {}

	void Simulator::set_sample_count(int count){
		sample_count_ = count;
	}

	// currency conversion is not used in the test since the currency rate is constant and 
	// equity amounts are calculated in USD: $400.00 * percentage 

	//double Simulator::currency_rate(std::string currency1, std::string currency2, double time){
	//	if ((currency1.compare("USD")==0) & (currency2.compare("HKD")==0))
	//		return 7.76;
	//	throw std::exception( "Unsupported currency" );
	//}

	double Simulator::least_performing_share(std::vector<Share> &basket) {
		std::vector<double> performance_level(basket_.size());
		for (unsigned int i = 0; i < basket_.size(); i++)
			performance_level[i] = basket_[i].performance_level();
		return *std::min_element(performance_level.begin(), performance_level.end());
	}

	double Simulator::determine_equity_amount(double lps, bool knocked_in){
		double performance;
		if (!knocked_in)
			performance = std::max(lps - 1.0, 0.0);
		else if (lps >= 1.0)
			performance = lps - 1.0;
		else
			performance = 1.0 - lps;
		return notional_amount_ * performance;
	}

	double Simulator::equity_amount_sample(int* performed_steps_count, bool store_data){

		CalendarItems steps = calendar_.get_calendar_items();

		bool knocked_in = false;
		bool knocked_in_processed = false;
		double lps;
		int i = 0;
		for (std::list<TimePeriodItem*>::iterator it = steps.begin(); it != steps.end() && !knocked_in_processed; ++it, i++){		
			double time = (*it)->deltaT;

			// jump to final date if the knock-in event happened
			if (knocked_in){
				time = calendar_.get_contract_deltaT() ;
				knocked_in_processed = true;
			}
			correlation_generator_.next_sample();
			for (unsigned int j = 0; j < basket_.size(); j++) {
				basket_[j].update_current_price(time, correlation_generator_.wiener(j));
			}
			if (store_data){
				std::vector<double> data(basket_.size() + 1);
				data[0] = time;
				for (unsigned int j = 0; j < basket_.size(); j++) {
					data[j+1] = basket_[j].current_price();
				}
				stored_iteration_data_.push_back(data);
			}

			lps = least_performing_share (basket_);
			if (check_knock_in_event_){
				knocked_in |= lps < knock_in_percentage_;
			}
		}
		*performed_steps_count = i;
		return determine_equity_amount(lps, knocked_in);
	}

	double Simulator::equity_amount(void){
		double sum = 0;
		for ( int i = 0; i < sample_count_; i++){
			int performed_steps_count;
			double simulated_equity_amount = equity_amount_sample(&performed_steps_count, i == stored_iteration_index_);
			sum += simulated_equity_amount;
			std::cout << 
				"iteration:" << std::setw(7) << std::left << i << 
				"steps:" << std::setw(6) << std::left << performed_steps_count << 
				"entity amount:" << simulated_equity_amount <<  std::endl;
		}
		return sum / sample_count_;
	}

	double Simulator::number_of_periods(void){
		return calendar_.get_contract_deltaT();
	}

	double Simulator::present_value(void){
		return equity_amount()  / pow (( 1 + short_interest_rate_), number_of_periods());
	}

	void Simulator::store_iteration(int index){
		stored_iteration_index_ = index;
	}

	std::vector<std::vector<double>> Simulator::stored_iteration_data(){
		return stored_iteration_data_;
	}

	void Simulator::save_iteration_data(std::string file_name){
		std::ofstream file;
		file.open(file_name);
		std::vector<Share>::iterator it; //�������� �������� it

		file << "time,'";
		for (unsigned int j = 0; j < basket_.size(); j++) {
			file << basket_[j].name() << ',';
		}
		file << std::endl;
		for (std::vector<std::vector<double>>::iterator it = stored_iteration_data_.begin(); it != stored_iteration_data_.end(); it++) {
			for (unsigned int j = 0; j < (*it).size(); j++) {
				file << (*it)[j] << ',';
			}
			file << std::endl;
		}
		file.close();
		std::cout << "Sample data (iteration " << stored_iteration_index_ << ") stored in '" << file_name << "' file" << std::endl;
	}

	void Simulator::check_knock_in_event(bool mode){
		check_knock_in_event_ = mode;
	}
}