#include "stdafx.h"
#include <time.h>
#include "determinationdayiterator.h"
#include "dtutils.h"

namespace Sample {

DeterminationDayIterator::DeterminationDayIterator(CalendarMode mode, TimePeriodItem* items) {
	root_ = current_pointer_ = items;
	factor_ = (CalendarMode::CALENDAR_DAYS == mode) ? factor_ = 365.0 : 252.0 ;
}

DeterminationDayIterator::~DeterminationDayIterator() {
}

bool  DeterminationDayIterator::hasNext() {
	//holiday check should be allready done in ContractCalendar
	return current_pointer_->next != NULL;
}

void  DeterminationDayIterator::last() {
	while ( hasNext()) {
		next();
	}
}

void  DeterminationDayIterator::next() {
	if (hasNext()) {
		current_pointer_ = current_pointer_ -> next;
	}
	else {
		throw "There are no more Determenation Day Periods";
	}
}

void  DeterminationDayIterator::reset() {
		current_pointer_ = root_ ;
}

double DeterminationDayIterator::getValue() {
	return current_pointer_ -> traidingDayNumberNumber_/factor_;
}

}