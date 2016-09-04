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
				throw "Unable to make time using mktime";
			}
		}
	};

	enum CalendarMode {
		TRAIDING_DAYS = 1,
		CALENDAR_DAYS = 2
	};

	class CalendarItem
	{
	public:
		CalendarItem(int contract_day_number, int traiding_day_number, tm time, double delta ) {
			contract_day_number_ = contract_day_number;
			traiding_day_number_ = traiding_day_number;
			time_ = time;
			deltaT_ = delta;
		};
		~CalendarItem();

		int contract_day_number_ ;
		int traiding_day_number_ ;
		tm time_;
		double deltaT_;

	};

	typedef std::list<CalendarItem*> CalendarItemList;

	class ContractCalendar 
	{
	public:
		ContractCalendar(CalendarMode mode, tm start_time, tm end_time);
		CalendarMode get_calendar_mode();
		double get_contract_deltaT(void); 
		CalendarItemList get_calendar_items();
		// This function prints the ContarctCalendar instance properties.
		// Use the following statement to print the properties to console:
		//    std::cout << cakendar 
		//
		friend std::ostream& operator<<(std::ostream&, const ContractCalendar&); 
		bool is_traiding_day(tm * time);


	private:
		int get_traiding_days_number();
		int get_calendar_days_number();
		void init_traiding_days_list(CalendarMode mode);

		int calendar_days_number_;
		int traiding_days_number_;
		CalendarMode mode_;
		double contract_deltaT_;
		//this is tm type info about contract time span  
		struct tm start_time_;
		struct tm end_time_;
		static const int DAY_INCREMENT = 24 * 60 * 60;
		std::list<CalendarItem*> list_;
	};
}