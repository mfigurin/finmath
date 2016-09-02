#include "stdafx.h"
#include "ctime"
#include <list>
#include "dtutils.h"
#include <exception>
#include "contractcalendar.h"

namespace Sample {

	ContractCalendar::ContractCalendar(CalendarMode mode, int from_year, int from_month, int from_day, int to_year, int to_month, int to_day) {

		DTUtils::getLocalTime(&start_time_);
		DTUtils::setDateTimeFields(&start_time_, from_year, from_month, from_day, 0, 0, 0); 

		DTUtils::getLocalTime(&end_time_);
		DTUtils::setDateTimeFields(&end_time_, to_year, to_month, to_day, 0, 0, 0); 

		if (mktime(&start_time_) >= mktime(&end_time_))
			throw std::exception("Bad contract dates");

		initTraidingDaysList(mode);

	}

	void ContractCalendar::initTraidingDaysList(CalendarMode mode) {
		__time64_t current, endtime;
		DTUtils::tmToLong(&start_time_, &current);
		DTUtils::tmToLong(&end_time_, &endtime);
		tm next_day;

		double calendar_year_days = 365;
		double traiding_year_days = 252;

		calendar_days_number_ = 0;
		traiding_days_number_ = 0;
		bool is_traiding_days  = (CalendarMode::CALENDAR_DAYS == mode) ? false : true; 
		while (current < endtime) {
			calendar_days_number_++;
			_localtime64_s(&next_day, &current);
			if (isTraidingDay(next_day.tm_year, next_day.tm_mon, next_day.tm_mday)) {
				double delta = (is_traiding_days)? traiding_days_number_/traiding_year_days : calendar_days_number_/calendar_year_days;
				TimePeriodItem* current_item = new TimePeriodItem(calendar_days_number_, traiding_days_number_, next_day, delta);
				list.push_back(current_item);
			}
			current += DAY_INCREMENT;
		};
	}

	//TODO check locale and read from file, db, etc...
	bool ContractCalendar::isHoliday(int year, int month, int day){
		return false;
	}
	bool ContractCalendar::isTraidingDay(int year, int month, int day){
		return DTUtils::isTraidingDay(year, month, day);
	}

	int ContractCalendar::getTraidingDaysNumber(){
		return traiding_days_number_;
	}

	int ContractCalendar::getCalendarDaysNumber() {
		return calendar_days_number_ ;
	}

	CalendarItems ContractCalendar::getCalendarItems() {
		return list;
	}


} //namespace