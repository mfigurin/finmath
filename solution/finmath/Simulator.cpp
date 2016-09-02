
#include "stdafx.h"
#include "Simulator.h"

CorrelationGenerator::CorrelationGenerator(Sample::CorrelationMatrix<double> matrix) : matrix(matrix){};

void CorrelationGenerator::next_sample() {}
double CorrelationGenerator::wiener(int index) { 
	return 0.0;
}

GBM::GBM(double initial_price, double drift, double volatility) : S0(initial_price), sigma(sigma), nu(drift-sigma*sigma/2) {}
GBM::~GBM(void) {}

double GBM::price_at(double time, double wiener_process){
	return S0 * exp(nu * time + sigma * wiener_process);
}

Simulator::Simulator(Contract contract, int sample_count, double knock_in_percentage, CorrelationGenerator correlation_generator) : 
	contract(contract), 
	sample_count(sample_count), 
	knock_in_percentage(knock_in_percentage),
	correlation_generator(correlation_generator){}
Simulator::~Simulator(void) {}

inline double Simulator::short_interest_rate(void)
{
	return 0.3;
}

double Simulator::currency_rate(std::string currency1, std::string currency2, double time){
	if ((currency1.compare("USD")==0) & (currency2.compare("HKD")==0))
		return 7.76;
	throw std::exception( "Unsupported currency" );
}

double Simulator::least_performing_share(std::vector<double> share_prices, double time) {
	std::vector<double> performance_level(share_prices.size());
	for (unsigned int i = 0; i < share_prices.size(); i++)
		performance_level[i] = share_prices[i] * currency_rate("usd","hkd", time) / contract.basket[i].initial_price * currency_rate("usd","hkd", 0);
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
	return contract.notional_amount * lps;
}

double Simulator::equity_amount_sample(void){

	double period = 3;
	double step = 1/365;

	int basket_size = contract.basket.size();

	std::vector<GBM> gbm;
	for (int i = 0; i < basket_size; i++){
		gbm[i] = GBM(contract.basket[i].initial_price, 0.3, 0.2);
	}

	std::vector<double> wiener(basket_size);
	std::vector<double> price(basket_size);
	bool knocked_in = false;
	double lps;
	for (double time = step; time <= period; time+=step){
		// jump to final date if the knock-in event happened
		if (knocked_in)
			time = period;
		correlation_generator.next_sample();
		for (int j = 0; j < basket_size; j++) {
			price[j] = gbm[j].price_at(time, correlation_generator.wiener(j));
		}
		lps = least_performing_share (price, step);		
		knocked_in |= lps < knock_in_percentage;
	}

	return determine_equity_amount(lps, knocked_in);
}

double Simulator::equity_amount(void){
	double sum = 0;
	for ( int i = 0; i < sample_count; i++){
		sum += equity_amount_sample();
	}
	return sum / sample_count;
}

double Simulator::number_of_periods(void){
	return 3;
}

double Simulator::present_value(void){
	return equity_amount()  / pow (( 1 + short_interest_rate()), number_of_periods());
}