///////////////////////////////////////////////////////////////////////////////////////////////////
// RainGaugeHourOv.cpp
//
// Googletest unit tests for RainGauge - artificial test cases
//
// https://github.com/matthias-bs/BresserWeatherSensorReceiver
//
//
// created: 09/2023
//
//
// MIT License
//
// Copyright (c) 2023 Matthias Prinke
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
// 20230917 Created
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
 * Test rainfall during past hour (with rain gauge overflow)
 */
TEST(TestRainHourOv, RainHourOv) {
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

  printf("< RainHourOv >\n");
  
  setTime("2022-09-06 8:00", tm, ts);
  rainGauge.update(tm, 10.0);
  DEBUG_CB();
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:06", tm, ts);
  rainGauge.update(tm, 10.1);
  DEBUG_CB();
  ASSERT_NEAR(0.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 8:12", tm, ts);
  rainGauge.update(tm, 60.3);
  DEBUG_CB();
  ASSERT_NEAR(50.3, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:18", tm, ts);
  rainGauge.update(tm, 0.6);
  DEBUG_CB();
  ASSERT_NEAR(90.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:24", tm, ts);
  rainGauge.update(tm, 10.0);
  DEBUG_CB();
  ASSERT_NEAR(100.0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:30", tm, ts);
  rainGauge.update(tm, 11.5);
  DEBUG_CB();
  ASSERT_NEAR(101.5, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-06 8:36", tm, ts);
  rainGauge.update(tm, 12.1);
  DEBUG_CB();
  ASSERT_NEAR(102.1, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-06 8:42", tm, ts);
  rainGauge.update(tm, 92.8);
  DEBUG_CB();
  ASSERT_NEAR(182.8, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 8:48", tm, ts);
  rainGauge.update(tm, 23.6);
  DEBUG_CB();
  ASSERT_NEAR(213.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 8:54", tm, ts);
  rainGauge.update(tm, 14.5);
  DEBUG_CB();
  ASSERT_NEAR(304.5, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 9:00", tm, ts);
  rainGauge.update(tm, 15.5);
  DEBUG_CB();
  ASSERT_NEAR(305.5, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 9:06", tm, ts);
  rainGauge.update(tm, 5.5);
  DEBUG_CB();
  ASSERT_NEAR(405.5 - 10.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 9:12", tm, ts);
  rainGauge.update(tm, 17.8);
  DEBUG_CB();
  ASSERT_NEAR(417.8 - 60.3, rainGauge.pastHour(), TOLERANCE);
}
