
#include "stdafx.h"
#include "simulator.h"

using namespace finmath;

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Share> basket;
	basket.push_back(Share("001_HK", "HKD", 17.4985, 0.03, 0.2));
	basket.push_back(Share("002_HK", "HKD", 17.1770, 0.03, 0.2));
	basket.push_back(Share("003_HK", "HKD", 19.2590, 0.03, 0.2));

	CorrelationMatrix matrix(3);
	matrix.set_correlation(0,1, 0.35);
	matrix.set_correlation(0,2, -0.4);
	matrix.set_correlation(1,2, 0.1);

	tm trade_date;
	tm final_date;
	DTUtils::set_tm_fields(&trade_date, 2016, 1, 25);
	DTUtils::set_tm_fields(&final_date, 2019, 1, 25);

	ContractCalendar calendar(CalendarMode::CALENDAR_DAYS, trade_date, final_date);
	CorrelationGenerator correlationGenerator(matrix, NormalDistribution());

	Simulator sim(calendar, 400.00, 0.03, basket, 0.72, correlationGenerator);

	std::cout << sim;

	sim.set_sample_count(100);
	//sim.suppress_payments(false, true);
	//sim.save_iteration_data(0, "data.csv");
	//sim.check_knock_in_event(false);
	//sim.jump_to_final_date(false);

	std::cout << "Present value: " << sim.simulate_present_value() << std::endl;	
}