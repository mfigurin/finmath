
#include "stdafx.h"
#include "simulator.h"

using namespace finmath;

int _tmain(int argc, _TCHAR* argv[])
{
	tm trade_date;
	tm final_date;
	DTUtils::set_tm_fields(&trade_date, 2016, 1, 25);
	DTUtils::set_tm_fields(&final_date, 2019, 1, 25);
	ContractCalendar calendar(CalendarMode::CALENDAR_DAYS, trade_date, final_date);

	double notional_amount = 400.00;
	double short_interest_rate = 0.03;
	double knock_in_event_percentage = 0.72;

	std::vector<Share> basket;
	basket.push_back(Share("001_HK", "HKD", 17.4985, 0.03, 0.2));
	basket.push_back(Share("002_HK", "HKD", 17.1770, 0.03, 0.2));
	basket.push_back(Share("003_HK", "HKD", 19.2590, 0.03, 0.2));

	CorrelationMatrix matrix(3);
	matrix.set_correlation(0,1, 0.35);
	matrix.set_correlation(0,2, -0.4);
	matrix.set_correlation(1,2, 0.1);
	CorrelationGenerator correlationGenerator(matrix, NormalDistribution());

	Simulator sim(calendar, notional_amount, short_interest_rate, basket, knock_in_event_percentage, correlationGenerator);

	sim.set_sample_count(100);
	//sim.suppress_payments(true, false);
	//sim.save_iteration_data(0, "data.csv");
	//sim.check_knock_in_event(false);
	//sim.jump_to_final_date(false);
	
	std::cout << "\n*** Simulation settings ***\n\n";
	std::cout << sim;
	std::cout << "\n*** Simulation started ***\n\n";
	std::cout << "\nPresent value: " << sim.simulate_present_value() << std::endl;	
	std::cout << "\n*** Simulation finished ***\n";
}