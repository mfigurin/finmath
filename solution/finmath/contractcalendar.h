//  
// ContractCalendar - holds the start and final dates of the contract
// Provide tools to iterate contract period in terms of traiding or calendar days :  1/252, 2/252, 3/252 or 1/365, 2/365, ...or 1/356 :  
//
//  ContractCalendar* calendar = new ContractCalendar(2016, 4, 25,   2016, 12, 31);
//
//	TimePeriodIterator* iterator = calendar->getTimePeriodIterator(TimeGranularity::KNOCK_IN_DETERMITION_DAY, CalendarMode::CALENDAR_DAYS, 0);
//
//	for (; iterator->hasNext(); iterator->next()) {
//		double value = iterator->getValue();   // here getValue looks like  n/365
//	}
//	
//
// TimePeriodIterator* iterator = calendar->getTimePeriodIterator(TimeGranularity::KNOCK_IN_DETERMITION_DAY, CalendarMode::TRAIDING_DAYS, 0);
//
//	for (; iterator->hasNext(); iterator->next()) {
//		double value = iterator->getValue();   // here getValue looks like  n/252
//	}
//

#pragma once

namespace Sample {

class TimePeriodIterator
{
	public:
		virtual bool  hasNext() = 0;
		virtual void  next() = 0;
		virtual double getValue() = 0;
		virtual void  last() = 0 ;
		virtual void reset() = 0;
};

// Granularity types for time span iterators

enum TimeGranularity {
	MINUTE = 1,
	HOUR = 2,
	DAY = 3,
	KNOCK_IN_DETERMINATION_DAY = 4
};

enum CalendarMode {
	TRAIDING_DAYS = 1,
	CALENDAR_DAYS = 2
};

class TimePeriodItem
{
	public:
		TimePeriodItem(int contractDayNumber, int traidingDayNumberNumber) {
			contractDayNumber_ = contractDayNumber;
			traidingDayNumberNumber_ = traidingDayNumberNumber;
		};
		~TimePeriodItem();

		int contractDayNumber_ ;
		int traidingDayNumberNumber_ ;
		TimePeriodItem* next;
};

class TimePeriodIteratorProvider {
	public:
		virtual TimePeriodIterator* getTimePeriodIterator(TimeGranularity granularity, CalendarMode mode, int granularityValue, TimePeriodItem* list) = 0 ; 
};

class DefaultTimePeriodIteratorsProvider : public TimePeriodIteratorProvider  {
	public:
		 TimePeriodIterator* getTimePeriodIterator(TimeGranularity granularity, CalendarMode mode, int granularityValue, TimePeriodItem* list) ; 
};

class ContractCalendar 
{
	public:
		ContractCalendar(int from_year, int from_month, int from_day, int to_year, int to_month, int to_day);
		void setTimePeriodIteratorProvider(TimePeriodIteratorProvider* provider);
		TimePeriodIterator* getTimePeriodIterator(TimeGranularity granularity, CalendarMode mode, int value);
		virtual bool isHoliday(int year, int month, int day);
		virtual bool isTraidingDay(int year, int month, int day);
		virtual int getTraidingDaysNumber();
		virtual int getCalendarDaysNumber();

	private:
		void initTradingDaysList();

		int calendar_days_number_;
		int traiding_days_number_;

		TimePeriodIteratorProvider* time_period_iterator_provider_ ;
		TimePeriodItem* data_list_ ;

		//This is tm type info about contract time span  
		struct tm start_time_;
		struct tm end_time_;
	
		static const int day_increment = 24 * 60 * 60;
};

}