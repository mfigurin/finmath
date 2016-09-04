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
		
		TEST_METHOD(TraidingDayTest)
		{
			tm trade_date;
			tm final_date;
			set_tm_fields(&trade_date, 2016, 1, 1);
			set_tm_fields(&final_date, 2020, 12, 31);
			mktime(&trade_date); 
			mktime(&final_date); 

			ContractCalendar calendar(CalendarMode::CALENDAR_DAYS, trade_date, final_date);
			
			tm check_date;
			set_tm_fields(&check_date, 2016, 9, 1);
			mktime(&check_date); 
			bool e = calendar.is_trading_day(&check_date);
			Assert::IsTrue(calendar.is_trading_day(&check_date));

			set_tm_fields(&check_date, 2016, 9, 4);
			mktime(&check_date); 	
			e = calendar.is_trading_day(&check_date);
			Assert::IsFalse(calendar.is_trading_day(&check_date));

			set_tm_fields(&check_date, 2020, 2, 29);
			mktime(&check_date); 
			e = calendar.is_trading_day(&check_date);
			Assert::IsFalse(calendar.is_trading_day(&check_date));

			set_tm_fields(&check_date, 2020, 2, 28);
			mktime(&check_date); 
			e = calendar.is_trading_day(&check_date);
			Assert::IsTrue(calendar.is_trading_day(&check_date));

		}

		TEST_METHOD(CalendarTest)
		{
				tm trade_date;
				tm final_date;

				set_tm_fields(&trade_date, 2016, 1, 1);
				set_tm_fields(&final_date, 2016, 12, 31);
				ContractCalendar calendar2016(CalendarMode::TRADING_DAYS, trade_date, final_date);
				Assert::AreEqual(366, calendar2016.get_calendar_days_number());  
		
				set_tm_fields(&trade_date, 2017, 1, 1);
				set_tm_fields(&final_date, 2017, 12, 31);
				ContractCalendar calendar2017(CalendarMode::TRADING_DAYS, trade_date, final_date);
				Assert::AreEqual(365, calendar2017.get_calendar_days_number());  


		}
	
	};
}