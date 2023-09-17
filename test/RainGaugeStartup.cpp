///////////////////////////////////////////////////////////////////////////////////////////////////
// RainGaugeStartup.cpp
//
// Googletest unit test for RainGauge - artificial test cases
//
// Test that rain gauge values are preserved after sensor startup,
// i.e. sensor reset or battery change
//
// https://github.com/matthias-bs/BresserWeatherSensorReceiver
//
//
// created: 09/2022
//
//
// MIT License
//
// Copyright (c) 2022 Matthias Prinke
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// History:
//
// 20220830 Created
//
// ToDo: 
// -
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>

#define TOLERANCE 0.11
#include "RainGauge.h"

//#define _DEBUG_CIRCULAR_BUFFER_

#if defined(_DEBUG_CIRCULAR_BUFFER_)
    #define DEBUG_CB() { rainGauge.printCircularBuffer(); }

#else
  #define DEBUG_CB() {}
#endif


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

TEST(TestRainGaugeStartup, TestRainGauge) {
  nvData_t nvData = {
   .tsBuf = {0},
   .rainBuf = {0}, 
   .head = 0,
   .tail = 0,
   .startupPrev = false,
   .rainStartup = 0,
   .tsDayBegin = 0xFF,
   .rainDayBegin = 0,
   .tsWeekBegin = 0xFF,
   .rainWeekBegin = 0,
   .wdayPrev = 0xFF,
   .tsMonthBegin = 0xFF,
   .rainMonthBegin = 0,
   .rainPrev = 0,
   .rainOvf = 0
  };

  RainGauge rainGauge(&nvData);
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;
    
  printf("< RainStartup >\n");
   
  setTime("2023-07-16 8:00:00", tm, ts);
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);
  ASSERT_NEAR(0, rainGauge.currentDay(), TOLERANCE);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);

  setTime("2023-07-16 8:05:00", tm, ts);
  rainGauge.update(tm, rainSensor = 10.0);
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);
  ASSERT_NEAR(0, rainGauge.currentDay(), TOLERANCE);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);

  setTime("2023-07-16 8:10:00", tm, ts);
  rainGauge.update(tm, rainSensor = 15.0);
  ASSERT_NEAR(5, rainGauge.pastHour(), TOLERANCE);
  ASSERT_NEAR(5, rainGauge.currentDay(), TOLERANCE);
  ASSERT_NEAR(5, rainGauge.currentWeek(), TOLERANCE);

  setTime("2023-07-16 8:15:00", tm, ts);
  rainGauge.update(tm, rainSensor = 0, true);
  ASSERT_NEAR(5, rainGauge.pastHour(), TOLERANCE);
  ASSERT_NEAR(5, rainGauge.currentDay(), TOLERANCE);
  ASSERT_NEAR(5, rainGauge.currentWeek(), TOLERANCE);
}
