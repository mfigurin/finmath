#include <iostream>
#include <string>
#include <sstream>

#include "stdafx.h"
#include "Simulator.h"


int _tmain(int argc, _TCHAR* argv[])
{
	struct tm trade_date = {0};
	trade_date.tm_year = 2016;
	trade_date.tm_mon = 1;
	trade_date.tm_mday = 25;

	struct tm final_date = {0};
	final_date.tm_year = 2019;
	final_date.tm_mon = 1;
	final_date.tm_mday = 25;

	std::vector<Share> basket(3, Share("001_HK", "HKD", 17.4985, 0.03, 0.2));
	basket[0] = Share("001_HK", "HKD", 17.4985, 0.03, 0.2);
	basket[1] = Share("002_HK", "HKD", 17.1770, 0.03, 0.2);
	basket[2] = Share("003_HK", "HKD", 19.2590, 0.03, 0.2);

	Sample::CorrelationMatrix<double> matrix(3);
	matrix.set(0,1, 0.35);
	matrix.set(0,2, -0.4);
	matrix.set(1,2, 0.1);

	CorrelationGenerator correlationGenerator(matrix, Sample::NormalDistribution());

	Simulator sim(trade_date, final_date, 400.00, basket, 0.72, correlationGenerator);
	double present_value = sim.present_value();
	std::cout << "Present value: " << present_value;
}

