#include "stdafx.h"
#include "ctime"
#include "dtutils.h"
#include <exception>
#include "contractcalendar.h"
#include "determenationdayiterator.h"


namespace Sample {

	TimePeriodIterator* DefaultTimePeriodIteratorProvider::getTimePeriodIterator(TimeGranularity granualarity, CalendarMode mode, int value, TimePeriodItem* list)  {

		switch ( granualarity ) {
			case TimeGranularity::DAY:
			case TimeGranularity::HOUR:
			case TimeGranularity::MINUTE:
				throw "TimePeriodIterator: requested granulatity is not currently supported";
			case TimeGranularity::KNOCK_IN_DETERMITION_DAY:
				return new DetermenationDayIterator(mode, list);
				break;
			default:
				throw "DefaultTimePeriodIteratorProvider: unknown granularity";
		}
	};

	/*_____________________________________________________________________________________

	ContractCalendar
	_______________________________________________________________________________________
	*/

	ContractCalendar::ContractCalendar(int from_year, int from_month, int from_day, int to_year, int to_month, int to_day) {

		//Lets save contract start/end datetime moments in the tm structure(store start/end datetime as tm the structure)
		DTUtils::getLocalTime(&start_time_);
		DTUtils::setDateTimeFields(&start_time_, from_year, from_month, from_day, 8, 0, 0); //trading day start at 8.00

		DTUtils::getLocalTime(&end_time_);
		DTUtils::setDateTimeFields(&end_time_, to_year, to_month, to_day, 8, 0, 0); 

		if (mktime(&start_time_) >= mktime(&end_time_))
			throw std::exception("Bad contract dates");

		//Now: lets create a basic list of trading days. It will be used by iterators later... 	
		initTradingDaysList();
		time_period_iterator_provider_ = new DefaultTimePeriodIteratorProvider();

	}

	void ContractCalendar::initTradingDaysList() {
		__time64_t current, endtime;
		DTUtils::tmToLong(&start_time_, &current);
		DTUtils::tmToLong(&end_time_, &endtime);
		tm next_day;

		calendar_days_number_ = 0;
		traiding_days_number_ = 0;

		TimePeriodItem* last_item = NULL;

		while (current < endtime) {
			calendar_days_number_++;
			_localtime64_s(&next_day, &current);
			if (isTraidingDay(next_day.tm_year, next_day.tm_mon, next_day.tm_mday)) {
				TimePeriodItem* current_item = new TimePeriodItem(calendar_days_number_, ++traiding_days_number_);
				if (data_list_ == NULL) {
					data_list_ = current_item;
				} else {
					last_item->next = current_item;
				}
				last_item = current_item;
			}
			current += day_increment;
		};
	}

	TimePeriodIterator* ContractCalendar::getTimePeriodIterator(TimeGranularity granularity, CalendarMode mode, int value) {
		return time_period_iterator_provider_ ->getTimePeriodIterator(granularity, mode, value, data_list_ ) ;
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

	void ContractCalendar::setTimePeriodIteratorProvider(TimePeriodIteratorProvider* provider) {
		iterator_provider_ = provider;
	}

} //namespace