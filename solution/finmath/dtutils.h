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

		static void DTUtils::getLocalTime(tm * date) { 
			 __time64_t long_time;
			 _time64(&long_time);
 			 _localtime64_s(date, &long_time);
		}

		static void DTUtils::setDateTimeFields(tm * date, int year, int month, int day, int hour, int min, int sec) {
			 date->tm_year = year - 1900;
			 date->tm_mon = month - 1;
			 date->tm_mday = day;
			 date->tm_hour = hour;
			 date->tm_min = min;
			 date->tm_sec = sec;
        }

		static void DTUtils::tmToLong(tm * tmDate, __time64_t * time) {
			*time = _mktime64(tmDate);
			if (*time == -1) 			{
				throw "Unable to make time using mktime";
			}
		}
};

} //namesapce


