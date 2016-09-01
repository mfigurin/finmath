#pragma once
#include "contractcalendar.h"

namespace Sample {

	class DetermenationDayIterator : public TimePeriodIterator {

	public:
		DetermenationDayIterator(CalendarMode mode, TimePeriodItem* items);
		~DetermenationDayIterator();

		bool  hasNext();
		void  next();
		double getValue();
		void last();
		void reset();

	private:
		double factor_;
		TimePeriodItem* current_pointer_;
		TimePeriodItem* root_;
		static const int day_increment = 24 * 60 * 60;
	
};

}