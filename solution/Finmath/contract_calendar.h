#pragma once

#include <list>
#include <ctime>

namespace finmath {
	class DTUtils
	{
	public:

		static void DTUtils::get_local_time(tm * date) { 
			__time64_t long_time;
			_time64(&long_time);
			_localtime64_s(date, &long_time);
		}

		static void DTUtils::set_tm_fields(tm * date, int year, int month, int day, int hour = 0, int min = 0, int sec = 0 ) {
			date->tm_year = year - 1900;
			date->tm_mon = month - 1;
			date->tm_mday = day;
			date->tm_hour = hour;
			date->tm_min = min;
			date->tm_sec = sec;
		}

		static void DTUtils::tm_to_long(tm * tmDate, __time64_t * time) {
			*time = _mktime64(tmDate);
			if (*time == -1) 			{
				throw std::exception("Unable to make time using mktime");
			}
		}
	};

	// Defines the mode for the simulation time delta calculation : n/252 or n/365
	enum CalendarMode {
		TRADING_DAYS = 1,
		CALENDAR_DAYS = 2
	};
		
	//Time info for simulation steps
	class CalendarItem
	{
	public:
		CalendarItem(int contract_day_number, int trading_day_number, tm time, double delta ) {
			contract_day_number_ = contract_day_number;
			trading_day_number_ = trading_day_number;
			time_ = time;
			deltaT_ = delta;
		};
		~CalendarItem();

		// the calendar day number from the contract start date
		int contract_day_number_ ;

		// the trading day number from the contract start date
		int trading_day_number_ ;

		// the local time for this trading day number
		tm time_;

		// The time delta from the contract start date (n/252 or n/365)
		double deltaT_;

	};

	typedef std::list<CalendarItem*> CalendarItemList;

	class ContractCalendar 
	{
	public:
		
		// ContractCalendar constructor
		//	mode		: defines the suimulation time delta calculation
		//					CalendarMode.TRADING_DAYS: the time delta is n/252, where n is the simulation step number
		//					CalendarMode.CALENDAR_DAYS: the time delta is n/365, where n is the simulation step number
		//	start_time	: the contract start time
		//	end_time:	: the contract end time

		ContractCalendar(CalendarMode mode, tm start_time, tm end_time);
		
		// return the calendar mode
		CalendarMode get_calendar_mode();
		
		// return time delta for the contract. The return value depends on the CalendarMode:
		//	CalendarMode.TRADING_DAYS : the return value is: trading_days_number / 252
		//	CalendarMode.CALENDAR_DAYS : the return value is: trading_days_number / 365
		double get_contract_deltaT(void); 
		
		//Returns the list of the time info items for the contract trading days 
		CalendarItemList get_calendar_items();
		
		// Prints the ContractCalendar instance properties.
		// Use the following statement to print the properties to console:
		//    std::cout << cakendar 
		//
		friend std::ostream& operator<<(std::ostream&, const ContractCalendar&); 
		
		// Check the time to be a trading day
		// Currently it checks the week day number of this time to be in the Monday..Friday time span
		// Public holidays are not taken into account
		bool is_trading_day(tm * time);

		// return the total number of the trading days for this contract 
		int get_trading_days_number();
		
		// return the total number of the calendar days for this contract 
		int get_calendar_days_number();

		//create the list of the trading days for this contract 
		void init_trading_days_list(CalendarMode mode);

	private:
	

		//the contract calendar days number 
		int calendar_days_number_;
		
		// the contract trading days number
		int trading_days_number_;
		
		//Defines the way for the simulation steps time delta calculation: n/252 or n/365, 
		CalendarMode mode_;

		//the whole contract time delta in terms of  (contract trading days number) / N, where N is: 252 or 365  
		double contract_deltaT_;

		//this is tm type info about contract time span  
		struct tm start_time_;
		struct tm end_time_;

		static const int DAY_INCREMENT = 24 * 60 * 60;
		
		//The contract time info for every trading day
		std::list<CalendarItem*> list_;
	};
}