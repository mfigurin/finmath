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

		TEST_METHOD(CheckCorrelationGenerator)
        {
            int cnt = 100000;
            NormalDistribution normal_distribution;

            finmath::CorrelationMatrix corr(3);
            corr.set_correlation( 0, 1, 0.35 );
            corr.set_correlation( 0, 2, -0.4 );
            corr.set_correlation( 1, 2, 0.1 );

            finmath::Matrix rnd_generated(3, cnt);
            finmath::CorrelationGenerator generator(corr, NormalDistribution());

            std::vector<double> average(3, 0.);
            for(int j=0; j<cnt; ++j) {
                for(int i=0; i<3; ++i) {
                    rnd_generated(i,j) = generator.wiener(i);
                    average[i] += rnd_generated(i,j);
                }
                generator.next_sample();
            }

            for(int i=0; i<3; ++i) {
                average[i] /= cnt;
            }

            double cov01 = 0, cov02 = 0, cov12 = 0;

            std::vector<double> sigma(3,0);
            for(int i=0; i<cnt; ++i) {
                double diff0 = rnd_generated(0,i) - average[0];
                double diff1 = rnd_generated(1,i) - average[1];
                double diff2 = rnd_generated(2,i) - average[2];

                cov01 += diff0 * diff1;
                cov02 += diff0 * diff2;
                cov12 += diff1 * diff2;

                sigma[0] += diff0*diff0;
                sigma[1] += diff1*diff1;
                sigma[2] += diff2*diff2;
            }

            double cor01 = cov01/(sqrt(sigma[0])*sqrt(sigma[1]));
            double cor02 = cov02/(sqrt(sigma[0])*sqrt(sigma[2]));
            double cor12 = cov12/(sqrt(sigma[1])*sqrt(sigma[2]));

			Assert::AreEqual( abs( cor01 - (0.35) ) <= 0.01, true );
			Assert::AreEqual( abs( cor02 - (-0.4) ) <= 0.01, true );
			Assert::AreEqual( abs( cor12 - (0.1) ) <= 0.01, true );

        }


		TEST_METHOD(CheckEquityAmount)
		{
			double initial_price_0 = 17.4985;
			double drift = 0.03;
			double volatility = 0.2;
			std::vector<Share> basket(3, Share("001_HK", "HKD", initial_price_0, drift, volatility));
			basket[0] = Share("001_HK", "HKD", initial_price_0, drift, volatility);

			double initial_price_1 = 17.1770;
			basket[1] = Share("002_HK", "HKD", initial_price_1, drift, volatility);

			double initial_price_2 = 19.2590;
			basket[2] = Share("003_HK", "HKD", initial_price_2, drift, volatility);

			finmath::CorrelationMatrix matrix(3);
			matrix.set_correlation(0,1, 0.0);
			matrix.set_correlation(0,2, 0.0);
			matrix.set_correlation(1,2, 0.0);

			tm trade_date;
			tm final_date;
			finmath::set_tm_fields(&trade_date, 2017, 1, 25);
			finmath::set_tm_fields(&final_date, 2018, 1, 24);

			finmath::ContractCalendar calendar(finmath::CalendarMode::CALENDAR_DAYS, trade_date, final_date);
			CorrelationGenerator correlation_generator(matrix, ZeroGenerator());

			double notional_amount = 400.0;
			Simulator sim(calendar, notional_amount, 0.03, basket, 0.72, correlation_generator);
			sim.set_sample_count(10);
   		    double equity_amount = sim.simulate_equity_amount();
			Assert::AreEqual(notional_amount * std::max(exp(drift - volatility*volatility/2.0) - 1.0, 0.0), equity_amount);

			std::cout << "equity_amount: " << equity_amount;
		}
	};
}