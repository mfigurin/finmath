#include "stdafx.h"
#include "CppUnitTest.h"
#include "test_utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../finmath/simulator.h"

using namespace test_utils;

namespace FinmathUnitTest
{
	TEST_CLASS(SimulatorTest)
	{
	public:
		
		TEST_METHOD(CalculatePresentValue)
		{
			std::vector<Share> basket;
			basket.push_back(Share("001_HK", "HKD", 17.4985, 0.03, 0.2));
			basket.push_back(Share("002_HK", "HKD", 17.1770, 0.03, 0.2));
			basket.push_back(Share("003_HK", "HKD", 19.2590, 0.03, 0.2));

			finmath::CorrelationMatrix matrix(3);
			matrix.set(0,1, 0.35);
			matrix.set(0,2, -0.4);
			matrix.set(1,2, 0.1);

			tm trade_date;
			tm final_date;
			finmath::DTUtils::set_tm_fields(&trade_date, 2016, 1, 25);
			finmath::DTUtils::set_tm_fields(&final_date, 2019, 1, 24);

			finmath::ContractCalendar calendar(finmath::CalendarMode::CALENDAR_DAYS, trade_date, final_date);
			CorrelationGenerator correlation_generator(matrix, finmath::NormalDistribution());

			Simulator sim(calendar, 400.00, 0.03, basket, 0.72, correlation_generator);
			double present_value = sim.simulate_present_value();
			std::cout << "Present value: " << present_value;
			sim.save_iteration_data(0, "sim.txt");
		}

		TEST_METHOD(CheckEquityAmount)
		{
			std::vector<Share> basket(3, Share("001_HK", "HKD", 17.4985, 0.03, 0.2));
			basket[0] = Share("001_HK", "HKD", 17.4985, 0.03, 0.2);
			basket[1] = Share("002_HK", "HKD", 17.1770, 0.03, 0.2);
			basket[2] = Share("003_HK", "HKD", 19.2590, 0.03, 0.2);

			finmath::CorrelationMatrix matrix(3);
			matrix.set(0,1, 0.0);
			matrix.set(0,2, 0.0);
			matrix.set(1,2, 0.0);

			tm trade_date;
			tm final_date;
			finmath::DTUtils::set_tm_fields(&trade_date, 2016, 1, 25);
			finmath::DTUtils::set_tm_fields(&final_date, 2017, 1, 24);

			finmath::ContractCalendar calendar(finmath::CalendarMode::CALENDAR_DAYS, trade_date, final_date);
			CorrelationGenerator correlation_generator(matrix, ZeroGenerator());

			Simulator sim(calendar, 400.00, 0.03, basket, 0.72, correlation_generator);
			sim.set_sample_count(1);
   		    sim.simulate_equity_amount();
		}
	};
}