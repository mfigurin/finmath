#include "stdafx.h"
#include "ctime"
#include <list>
#include <exception>
#include "contractcalendar.h"

namespace Sample {

	ContractCalendar::ContractCalendar(CalendarMode mode, int from_year, int from_month, int from_day, int to_year, int to_month, int to_day) {
		DTUtils::get_local_time(&start_time_);
		DTUtils::set_tm_fields(&start_time_, from_year, from_month, from_day, 0, 0, 0); 

		DTUtils::get_local_time(&end_time_);
		DTUtils::set_tm_fields(&end_time_, to_year, to_month, to_day, 0, 0, 0); 

		if (mktime(&start_time_) >= mktime(&end_time_))
			throw std::exception("Bad contract dates");

		init_traiding_days_list(mode);
	}

	void ContractCalendar::init_traiding_days_list(CalendarMode mode) {
		__time64_t current, endtime;
		DTUtils::tm_to_long(&start_time_, &current);
		DTUtils::tm_to_long(&end_time_, &endtime);
		tm next_day;

		double calendar_year_days = 365;
		double traiding_year_days = 252;

		calendar_days_number_ = 0;
		traiding_days_number_ = 0;
		bool is_traiding_days  = (CalendarMode::CALENDAR_DAYS == mode) ? false : true; 
		while (current <= endtime) {
			calendar_days_number_++;
			_localtime64_s(&next_day, &current);
			if (is_traiding_day(next_day.tm_year + 1900, next_day.tm_mon + 1, next_day.tm_mday)) {
				traiding_days_number_++;
				double delta = (is_traiding_days)? traiding_days_number_/traiding_year_days : calendar_days_number_/calendar_year_days;
				TimePeriodItem* current_item = new TimePeriodItem(calendar_days_number_, traiding_days_number_, next_day, delta);

				list.push_back(current_item);
			}
			current += DAY_INCREMENT;
		};

		contract_deltaT = is_traiding_days ? traiding_days_number_/traiding_year_days : calendar_days_number_/calendar_year_days;
	}

	//TODO check locale and read from file, db, etc...
	bool ContractCalendar::is_holiday(int year, int month, int day){
		return false;
	}

	bool ContractCalendar::is_traiding_day(int year, int month, int day){
		return DTUtils::is_traiding_day(year, month, day);
	}

	int ContractCalendar::get_traiding_days_number(){
		return traiding_days_number_;
	}

	int ContractCalendar::get_calendar_days_number() {
		return calendar_days_number_ ;
	}

	CalendarItems ContractCalendar::get_calendar_items() {
		return list;
	}

	double ContractCalendar::get_contract_deltaT(void) {
		return contract_deltaT;
	}

	CalendarMode ContractCalendar::get_calendar_mode() {
		return mode_;
	}
} //namespace