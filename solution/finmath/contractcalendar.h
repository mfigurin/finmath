#pragma once
#include <list>
namespace Sample {

	enum CalendarMode {
		TRAIDING_DAYS = 1,
		CALENDAR_DAYS = 2
	};

	class TimePeriodItem
	{
		public:
			TimePeriodItem(int contractDayNumber, int traidingDayNumber, tm time, double delta ) {
				contractDayNumber_ = contractDayNumber;
				traidingDayNumber_ = traidingDayNumber;
				time_ = time;
				deltaT = delta;
			};
			~TimePeriodItem();

			int contractDayNumber_ ;
			int traidingDayNumber_ ;
			tm time_;
			double deltaT;

		};

	typedef std::list<TimePeriodItem*> CalendarItems;

	class ContractCalendar 
	{
		public:
			ContractCalendar(CalendarMode mode, int from_year, int from_month, int from_day, int to_year, int to_month, int to_day);
			CalendarMode getCalendarMode();
			double getContractDeltaT(); 
			CalendarItems getCalendarItems();

		private:
			bool isHoliday(int year, int month, int day);
			bool isTraidingDay(int year, int month, int day);
			int getTraidingDaysNumber();
			int getCalendarDaysNumber();
			void initTraidingDaysList(CalendarMode mode);
			
			int calendar_days_number_;
			int traiding_days_number_;
			CalendarMode mode_;
			double contractDeltaT;
			//This is tm type info about contract time span  
			struct tm start_time_;
			struct tm end_time_;

			static const int DAY_INCREMENT = 24 * 60 * 60;

			std::list<TimePeriodItem*> list;

		};

}