


#include <gtest/gtest.h>

#include "Arduino.h"
#include "RainGauge.h"
#define TOLERANCE 0.11

/**
 * \example
 * struct tm tm;
 * time_t t;
 * strptime("6 Dec 2001 12:33:45", "%d %b %Y %H:%M:%S", &tm);
 * tm.tm_isdst = -1;      // Not set by strptime(); tells mktime()
 *                        // to determine whether daylight saving time
 *                        // is in effect
 * t = mktime(&tm);
 */

static void setTime(const char *time, tm &tm, time_t &ts)
{
  strptime(time, "%Y-%m-%d %H:%M", &tm);
  ts = mktime(&tm);
}


TEST(RainGauge, poc)
{
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;

  fprintf(stderr, "test start\n");
  printf("< RainHour >\n");
  
  setTime("2022-09-06 8:00", tm, ts);
  rainGauge.update(tm, rainSensor=10.0);
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);  
}

