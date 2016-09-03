#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../finmath/contract_calendar.h"

namespace FinmathUnitTest
{
	TEST_CLASS(DtutilsTest)
	{
	public:

		TEST_METHOD(day_of_week)
		{
			int day_of_week = Sample::DTUtils::day_of_week(2000, 1, 1);
			Assert::AreEqual(6, day_of_week);

			day_of_week = Sample::DTUtils::day_of_week(2016, 9, 2);
			Assert::AreEqual(5, day_of_week);

			day_of_week = Sample::DTUtils::day_of_week(2020, 12, 31);
			Assert::AreEqual(4, day_of_week);

			day_of_week = Sample::DTUtils::day_of_week(2020, 2, 29);
			Assert::AreEqual(6, day_of_week);
		}

		TEST_METHOD(is_traiding_day)
		{
			bool isTradingDay = Sample::DTUtils::is_traiding_day(2016, 9, 1);
			Assert::AreEqual(true, isTradingDay);

			isTradingDay = Sample::DTUtils::is_traiding_day(2016, 9, 4);
			Assert::AreEqual(false, isTradingDay);

			isTradingDay = Sample::DTUtils::is_traiding_day(2020, 2, 29);
			Assert::AreEqual(false, isTradingDay);

			isTradingDay = Sample::DTUtils::is_traiding_day(2020, 2, 28);
			Assert::AreEqual(true, isTradingDay);
		}

	};
}