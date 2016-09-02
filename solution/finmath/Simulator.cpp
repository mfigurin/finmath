
#include "stdafx.h"
#include "Simulator.h"


CorrelationGenerator::CorrelationGenerator(Sample::CorrelationMatrix<double>& matrix, Sample::RandomGenerator& generator) : 
	correlation_matrix(matrix), 
	generator(generator),
	distribution(Sample::Matrix2<double>( matrix.rows(), 1)) {
	next_sample();
};

void CorrelationGenerator::next_sample() {
	for( int i = 0; i < correlation_matrix.rows(); i++ ) {
		distribution(i) = generator.nextValue();
	}
	distribution = correlation_matrix * distribution;
}

double CorrelationGenerator::wiener(int index) { 
	return distribution(index);
}


Share::Share(std::string name, std::string currency, double initial_price, double drift, double volatility) :
	name(name), 
	currency(currency), 
	initial_price(initial_price),
	sigma(volatility), 
	nu(drift-sigma*sigma/2)
{}

Share::Share( const Share& share ) {
	name = share.name;
	currency = share.currency;
	initial_price = share.initial_price;
	sigma = share.sigma;
	nu = share.nu;
}

Share::~Share(void) {}

void Share::update_current_price(double time, double wiener_process){
	current_price = initial_price * exp(nu * time + sigma * wiener_process);
}

Simulator::Simulator(Sample::ContractCalendar& calendar, double notional_amount, std::vector<Share> &basket, double knock_in_percentage, CorrelationGenerator& correlation_generator) : 
	calendar(calendar),
	notional_amount(notional_amount),
	basket(basket), 
	knock_in_percentage(knock_in_percentage),
	correlation_generator(correlation_generator),
	sample_count(1000)
{}

Simulator::~Simulator(void) {}

inline double Simulator::short_interest_rate(void)
{
	return 0.03;
}

double Simulator::currency_rate(std::string currency1, std::string currency2, double time){
	if ((currency1.compare("USD")==0) & (currency2.compare("HKD")==0))
		return 7.76;
	throw std::exception( "Unsupported currency" );
}

double Simulator::least_performing_share(std::vector<Share> &basket) {
	std::vector<double> performance_level(basket.size());
	for (unsigned int i = 0; i < basket.size(); i++)
		performance_level[i] = basket[i].current_price / basket[i].initial_price;
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
	return notional_amount * performance;
}

double Simulator::equity_amount_sample(){

	Sample::CalendarItems steps = calendar.getCalendarItems();

	bool knocked_in = false;
	double lps;
	int i = 0;
	for (std::list<Sample::TimePeriodItem*>::iterator it = steps.begin(); it != steps.end(); ++it, i++){		
		double time = (*it)->deltaT;

		// jump to final date if the knock-in event happened
		if (knocked_in)
			time = calendar.getContractDeltaT() ;
		correlation_generator.next_sample();
		for (unsigned int j = 0; j < basket.size(); j++) {
			basket[j].update_current_price(time, correlation_generator.wiener(j));
		}
		lps = least_performing_share (basket);		
		knocked_in |= lps < knock_in_percentage;
	}
	std::cout << i << " steps done" << "\n";
	//return determine_equity_amount(lps, knocked_in) / currency_rate("USD", "HKD", period);
	return determine_equity_amount(lps, knocked_in);
}

double Simulator::equity_amount(void){
	double sum = 0;
	for ( int i = 0; i < sample_count; i++){
		std::cout << "iteration: " <<  i << "\n";
		double amount = equity_amount_sample();
		sum += amount;
	}
	return sum / sample_count;
}

double Simulator::number_of_periods(void){
	return calendar.getContractDeltaT();
}

double Simulator::present_value(void){
	return equity_amount()  / pow (( 1 + short_interest_rate()), number_of_periods());
}