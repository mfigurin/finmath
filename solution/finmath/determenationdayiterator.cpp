#include "stdafx.h"
#include <time.h>
#include "determenationdayiterator.h"
#include "dtutils.h"

namespace Sample {

DetermenationDayIterator::DetermenationDayIterator(CalendarMode mode, TimePeriodItem* items) {
	root_ = current_pointer_ = items;
	factor_ = (CalendarMode::CALENDAR_DAYS == mode) ? factor_ = 365.0 : 252.0 ;
}

DetermenationDayIterator::~DetermenationDayIterator() {
}

bool  DetermenationDayIterator::hasNext() {
	//holiday check should be allredy done in ContractCalendar
	return current_pointer_->next != NULL;
}

void  DetermenationDayIterator::last() {
	while ( hasNext()) {
		next();
	}
}

void  DetermenationDayIterator::next() {
	if (hasNext()) {
		current_pointer_ = current_pointer_ -> next;
	}
	else {
		throw "There are no more Determenation Day Periods";
	}
}

void  DetermenationDayIterator::reset() {
	if (hasNext()) {
		current_pointer_ = current_pointer_ -> next;
	}
	else {
		throw "There are no more Determenation Day Periods";
	}
}

double DetermenationDayIterator::getValue() {
	return current_pointer_ -> traidingDayNumberNumber_/factor_;
}

}