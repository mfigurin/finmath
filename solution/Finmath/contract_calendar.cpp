#include "stdafx.h"
#include <time.h>
#include <fstream>

#include "contract_calendar.h"

namespace finmath {

	ContractCalendar::ContractCalendar(CalendarMode mode, tm start_time, tm end_time) : start_time_(start_time), end_time_(end_time) {
		if (mktime(&start_time_) >= mktime(&end_time_))
			throw std::exception("Bad contract dates");

		init_trading_days_list(mode);
	}

	void ContractCalendar::init_trading_days_list(CalendarMode mode) {

		time_t current = mktime(&start_time_);
		time_t endtime = mktime(&end_time_);

		tm next_day;

		double calendar_year_days = 365;
		double trading_year_days = 252;

		calendar_days_number_ = 0;
		trading_days_number_ = 0;
		bool is_trading_days  = (CalendarMode::CALENDAR_DAYS == mode) ? false : true; 
		while (current <= endtime) {
			calendar_days_number_++;
			_localtime64_s(&next_day, &current);
			if (is_trading_day(&next_day)) {
				trading_days_number_++;
				double delta = (is_trading_days)? trading_days_number_/trading_year_days : calendar_days_number_/calendar_year_days;
				CalendarItem* current_item = new CalendarItem(calendar_days_number_, trading_days_number_, next_day, delta);

				list_.push_back(current_item);
			}
			current += DAY_INCREMENT;
		};

		contract_deltaT_ = is_trading_days ? trading_days_number_/trading_year_days : calendar_days_number_/calendar_year_days;
	}

	//Check that day is not Saturday or Sunday
	bool ContractCalendar::is_trading_day(tm* time){
		return time ->tm_wday > 0 && time ->tm_wday < 6;
	}

	int ContractCalendar::get_trading_days_number(){
		return trading_days_number_;
	}

	int ContractCalendar::get_calendar_days_number() {
		return calendar_days_number_ ;
	}

	CalendarItemList ContractCalendar::get_calendar_items() {
		return list_;
	}

	double ContractCalendar::get_contract_deltaT(void) {
		return contract_deltaT_;
	}

	CalendarMode ContractCalendar::get_calendar_mode() {
		return mode_;
	}
	
	std::ostream& operator<<(std::ostream &strm, const ContractCalendar &calendar) {
		
		 char start[30];
         strftime(start,30,"%m-%d-%Y",&calendar.start_time_);
		 char end[30];
         strftime(end,30,"%m-%d-%Y",&calendar.end_time_);

		 strm << 
			"Contract first date:" << start  << std::endl <<
			"Contract last date:" << end  << std::endl <<
			"Total contract trading days number:" << calendar.trading_days_number_<< std::endl ;
		return strm;
	}
} //namespace