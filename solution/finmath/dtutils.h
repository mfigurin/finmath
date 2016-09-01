#pragma once
#include <time.h>

namespace Sample {
class DTUtils
{
	public:
		static int DTUtils::dayOfWeek(int y, int m, int d) 	{
			int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
			y -= m < 3;
			return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
		}

		//TODO check holidays
		static bool DTUtils::isTraidingDay(int year, int month, int day) { 
			int daynum = DTUtils::dayOfWeek(year, month, day);
			return (daynum > 0 && daynum < 6);
		}

}

} //namesapce


