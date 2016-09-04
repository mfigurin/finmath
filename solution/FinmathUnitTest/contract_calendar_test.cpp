#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../finmath/contract_calendar.h"
using namespace finmath;

namespace FinmathUnitTest
{
	TEST_CLASS(DtutilsTest)
	{
	public:
		
		TEST_METHOD(is_traiding_day)
		{
			tm trade_date;
			tm final_date;
			DTUtils::set_tm_fields(&trade_date, 2016, 1, 1);
			DTUtils::set_tm_fields(&final_date, 2020, 12, 31);
			ContractCalendar calendar(CalendarMode::CALENDAR_DAYS, trade_date, final_date);
			
			tm check_date;
			DTUtils::set_tm_fields(&check_date, 2016, 9, 1);
			Assert::AreEqual(true, calendar.is_traiding_day(&check_date));

			DTUtils::set_tm_fields(&check_date, 2016, 9, 4);
			Assert::AreEqual(false, calendar.is_traiding_day(&check_date));

			DTUtils::set_tm_fields(&check_date, 2020, 2, 29);
			Assert::AreEqual(false, calendar.is_traiding_day(&check_date));

			DTUtils::set_tm_fields(&check_date, 2020, 2, 28);
			Assert::AreEqual(true, calendar.is_traiding_day(&check_date));

		}

	};
}