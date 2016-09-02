#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../finmath/dtutils.h"

namespace FinmathUnitTest
{
	TEST_CLASS(DtutilsTest)
	{
	public:

		TEST_METHOD(dayOfWeek)
		{
			int dayOfWeek = Sample::DTUtils::dayOfWeek(2000, 1, 1);
			Assert::AreEqual(6, dayOfWeek);

			dayOfWeek = Sample::DTUtils::dayOfWeek(2016, 9, 2);
			Assert::AreEqual(5, dayOfWeek);

			dayOfWeek = Sample::DTUtils::dayOfWeek(2020, 12, 31);
			Assert::AreEqual(4, dayOfWeek);

			dayOfWeek = Sample::DTUtils::dayOfWeek(2020, 2, 29);
			Assert::AreEqual(6, dayOfWeek);
     	}

		TEST_METHOD(isTraidingDay)
		{
			bool isTradingDay = Sample::DTUtils::isTraidingDay(2016, 9, 1);
			Assert::AreEqual(true, isTradingDay);

			isTradingDay = Sample::DTUtils::isTraidingDay(2016, 9, 4);
			Assert::AreEqual(false, isTradingDay);

			isTradingDay = Sample::DTUtils::isTraidingDay(2020, 2, 29);
			Assert::AreEqual(false, isTradingDay);

			isTradingDay = Sample::DTUtils::isTraidingDay(2020, 2, 28);
			Assert::AreEqual(true, isTradingDay);
		}

	};
}