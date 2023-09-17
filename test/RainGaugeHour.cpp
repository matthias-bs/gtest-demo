///////////////////////////////////////////////////////////////////////////////////////////////////
// RainGaugeHour.cpp
//
// Googletest unit tests for RainGauge - artificial test cases
//
// Test rainfall during past hour (no rain gauge overflow)
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

/*
 * Test rainfall during past hour (no rain gauge overflow)
 */
TEST(TestRainGaugeHour, RainHour) {
  nvData_t data = {
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

  RainGauge rainGauge(&data);
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;

  printf("< RainHour >\n");
  
  setTime("2022-09-06 8:00", tm, ts);
  //rainGauge.init(tm, rainSensor=10.0);
  rainGauge.update(tm, rainSensor=10.0);
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:06", tm, ts);
  rainGauge.update(tm, rainSensor=10.1);
  DEBUG_CB();
  ASSERT_NEAR(0.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 8:12", tm, ts);
  rainGauge.update(tm, rainSensor=10.3);
  DEBUG_CB();
  ASSERT_NEAR(0.3, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:18", tm, ts);
  rainGauge.update(tm, rainSensor=10.6);
  DEBUG_CB();
  ASSERT_NEAR(0.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:24", tm, ts);
  rainGauge.update(tm, rainSensor=11.0);
  DEBUG_CB();
  ASSERT_NEAR(1.0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:30", tm, ts);
  rainGauge.update(tm, rainSensor=11.5);
  DEBUG_CB();
  ASSERT_NEAR(1.5, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-06 8:36", tm, ts);
  rainGauge.update(tm, rainSensor=12.1);
  DEBUG_CB();
  ASSERT_NEAR(2.1, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-06 8:42", tm, ts);
  rainGauge.update(tm, rainSensor=12.8);
  DEBUG_CB();
  ASSERT_NEAR(2.8, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 8:48", tm, ts);
  rainGauge.update(tm, rainSensor=13.6);
  DEBUG_CB();
  ASSERT_NEAR(3.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:54", tm, ts);
  rainGauge.update(tm, rainSensor=14.5);
  DEBUG_CB();
  ASSERT_NEAR(4.5, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 9:00", tm, ts);
  rainGauge.update(tm, rainSensor=15.5);
  DEBUG_CB();
  ASSERT_NEAR(5.5, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 9:06", tm, ts);
  rainGauge.update(tm, rainSensor=16.6);
  DEBUG_CB();
  ASSERT_NEAR(16.6 - 10.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 9:12", tm, ts);
  rainGauge.update(tm, rainSensor=17.8);
  DEBUG_CB();
  ASSERT_NEAR(17.8 - 10.3, rainGauge.pastHour(), TOLERANCE);
}
