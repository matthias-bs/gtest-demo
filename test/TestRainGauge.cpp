///////////////////////////////////////////////////////////////////////////////////////////////////
// TestRainGauge.cpp
//
// CppUTest unit tests for RainGauge - artificial test cases
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
void RainHour() {
  RainGauge rainGauge;
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


/*
 * Test rainfall during past hour (no rain gauge overflow),
 * short update interval (5 minutes)
 */
void RainHourShort() {
  RainGauge rainGauge;
  rainGauge.reset();
  
  tm        tm;
  time_t    ts;
  float     rainSensor;

  printf("< RainHourShort >\n");
  
  setTime("2022-09-11 15:00", tm, ts);
  rainGauge.init(tm, rainSensor=10.0);
  rainGauge.update(tm, rainSensor=10.0);
  DEBUG_CB();
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:05", tm, ts);
  rainGauge.update(tm, rainSensor=10.1);
  DEBUG_CB();
  ASSERT_NEAR(0.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 15:10", tm, ts);
  rainGauge.update(tm, rainSensor=10.3);
  DEBUG_CB();
  ASSERT_NEAR(0.3, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:15", tm, ts);
  rainGauge.update(tm, rainSensor=10.6);
  DEBUG_CB();
  ASSERT_NEAR(0.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:20", tm, ts);
  rainGauge.update(tm, rainSensor=11.0);
  DEBUG_CB();
  ASSERT_NEAR(1.0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:25", tm, ts);
  rainGauge.update(tm, rainSensor=11.5);
  DEBUG_CB();
  ASSERT_NEAR(1.5, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-11 15:30", tm, ts);
  rainGauge.update(tm, rainSensor=12.1);
  DEBUG_CB();
  ASSERT_NEAR(2.1, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-11 15:35", tm, ts);
  rainGauge.update(tm, rainSensor=12.8);
  DEBUG_CB();
  ASSERT_NEAR(2.8, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 15:40", tm, ts);
  rainGauge.update(tm, rainSensor=13.6);
  DEBUG_CB();
  ASSERT_NEAR(3.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:45", tm, ts);
  rainGauge.update(tm, rainSensor=14.5);
  DEBUG_CB();
  ASSERT_NEAR(4.5, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 15:50", tm, ts);
  rainGauge.update(tm, rainSensor=15.5);
  DEBUG_CB();
  ASSERT_NEAR(5.5, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 15:55", tm, ts);
  rainGauge.update(tm, rainSensor=16.6);
  DEBUG_CB();
  ASSERT_NEAR(6.6, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 16:00", tm, ts);
  rainGauge.update(tm, rainSensor=17.8);
  DEBUG_CB();
  ASSERT_NEAR(7.8, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 16:05", tm, ts);
  rainGauge.update(tm, rainSensor=18.8);
  DEBUG_CB();
  ASSERT_NEAR(18.8 - 10.1, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 16:10", tm, ts);
  rainGauge.update(tm, rainSensor=19.9);
  DEBUG_CB();
  ASSERT_NEAR(19.9 - 10.3, rainGauge.pastHour(), TOLERANCE);
}


/*
 * Test rainfall during past hour (no rain gauge overflow),
 * long update interval (10 minutes)
 * The ring buffer will not be filled completely.
 */
void RainHourLong() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;

  printf("< RainHourLong >\n");
  
  setTime("2022-09-11 15:00", tm, ts);
  rainGauge.update(tm, rainSensor=10.0);
  DEBUG_CB();
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:10", tm, ts);
  rainGauge.update(tm, rainSensor=10.1);
  DEBUG_CB();
  ASSERT_NEAR(0.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 15:20", tm, ts);
  rainGauge.update(tm, rainSensor=10.3);
  DEBUG_CB();
  ASSERT_NEAR(0.3, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:30", tm, ts);
  rainGauge.update(tm, rainSensor=10.6);
  DEBUG_CB();
  ASSERT_NEAR(0.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:40", tm, ts);
  rainGauge.update(tm, rainSensor=11.0);
  DEBUG_CB();
  ASSERT_NEAR(1.0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 15:50", tm, ts);
  rainGauge.update(tm, rainSensor=11.5);
  DEBUG_CB();
  ASSERT_NEAR(1.5, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-11 16:00", tm, ts);
  rainGauge.update(tm, rainSensor=12.1);
  DEBUG_CB();
  ASSERT_NEAR(2.1, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-11 16:10", tm, ts);
  rainGauge.update(tm, rainSensor=12.8);
  DEBUG_CB();
  ASSERT_NEAR(12.8 - 10.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 16:20", tm, ts);
  rainGauge.update(tm, rainSensor=13.6);
  DEBUG_CB();
  ASSERT_NEAR(13.6 - 10.3, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 16:30", tm, ts);
  rainGauge.update(tm, rainSensor=14.5);
  DEBUG_CB();
  ASSERT_NEAR(14.5 - 10.6, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 16:40", tm, ts);
  rainGauge.update(tm, rainSensor=15.5);
  DEBUG_CB();
  ASSERT_NEAR(15.5 - 11.0, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 16:50", tm, ts);
  rainGauge.update(tm, rainSensor=16.6);
  DEBUG_CB();
  ASSERT_NEAR(16.6 - 11.5, rainGauge.pastHour(), TOLERANCE);
}


/*
 * Test rainfall during past hour (no rain gauge overflow),
 * extremely long update interval (65 minutes)
 * The distance between head and tail will be > 1h.
 */
void RainHourExtreme() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;

  printf("< RainHourExtreme >\n");
  
  setTime("2022-09-11 15:00", tm, ts);
  rainGauge.update(tm, rainSensor=10.0);
  DEBUG_CB();
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 16:05", tm, ts);
  rainGauge.update(tm, rainSensor=10.1);
  DEBUG_CB();
  ASSERT_NEAR(0.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-11 17:10", tm, ts);
  rainGauge.update(tm, rainSensor=10.3);
  DEBUG_CB();
  ASSERT_NEAR(0.2, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 18:15", tm, ts);
  rainGauge.update(tm, rainSensor=10.6);
  DEBUG_CB();
  ASSERT_NEAR(0.3, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 19:20", tm, ts);
  rainGauge.update(tm, rainSensor=11.0);
  DEBUG_CB();
  ASSERT_NEAR(0.4, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-11 20:25", tm, ts);
  rainGauge.update(tm, rainSensor=11.5);
  DEBUG_CB();
  ASSERT_NEAR(0.5, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-11 21:40", tm, ts);
  rainGauge.update(tm, rainSensor=12.1);
  DEBUG_CB();
  ASSERT_NEAR(0.6, rainGauge.pastHour(), TOLERANCE);  
}


/*
 * Test daily rainfall (no rain gauge overflow)
 */
void RainDaily() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;

  printf("< RainDaily >\n");
  setTime("2022-09-06 8:00", tm, ts);
  ASSERT_NEAR(0, rainGauge.currentDay(), TOLERANCE);

  rainGauge.update(tm, rainSensor=10.0);
  ASSERT_NEAR(0, rainGauge.currentDay(), TOLERANCE);

  setTime("2022-09-06 12:00", tm, ts);
  rainGauge.update(tm, rainSensor=12.0);
  ASSERT_NEAR(2, rainGauge.currentDay(), TOLERANCE);

  setTime("2022-09-06 16:00", tm, ts);
  rainGauge.update(tm, rainSensor=14.0);
  ASSERT_NEAR(4, rainGauge.currentDay(), TOLERANCE);

  setTime("2022-09-06 20:00", tm, ts);
  rainGauge.update(tm, rainSensor=16.0);
  ASSERT_NEAR(6, rainGauge.currentDay(), TOLERANCE);

  setTime("2022-09-06 23:59", tm, ts);
  rainGauge.update(tm, rainSensor=18.0);
  ASSERT_NEAR(8, rainGauge.currentDay(), TOLERANCE);

  // Next Day
  setTime("2022-09-07 00:00", tm, ts);
  rainGauge.update(tm, rainSensor=20.0);
  ASSERT_NEAR(0, rainGauge.currentDay(), TOLERANCE);  

  setTime("2022-09-07 04:00", tm, ts);
  rainGauge.update(tm, rainSensor=22.0);
  ASSERT_NEAR(2, rainGauge.currentDay(), TOLERANCE);  
}


/*
 * Test weekly rainfall (no rain gauge overflow)
 */
void RainWeekly() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;

  printf("< RainWeekly >\n");
  
  setTime("2022-09-06 8:00:00", tm, ts);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);

  rainGauge.update(tm, rainSensor=10.0);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);

  setTime("2022-09-06 16:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=15.0);
  ASSERT_NEAR(5, rainGauge.currentWeek(), TOLERANCE);
  
  setTime("2022-09-06 23:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=20.0);
  ASSERT_NEAR(10, rainGauge.currentWeek(), TOLERANCE);
  
  setTime("2022-09-07 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=25.0);
  ASSERT_NEAR(15, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-08 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=30.0);
  ASSERT_NEAR(20, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-09 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=35.0);
  ASSERT_NEAR(25, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-10 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=40.0);
  ASSERT_NEAR(30, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-11 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=45.0);
  ASSERT_NEAR(35, rainGauge.currentWeek(), TOLERANCE);  

  // Next Week
  setTime("2022-09-12 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=50.0);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-13 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=50.0);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);  
}


/*
 * Test monthly rainfall (no rain gauge overflow)
 */
void RainMonthly() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor = 0;
  float     rainMonthly;
    
  printf("< RainMonthly >\n");
  
  setTime("2022-09-06 12:00:00", tm, ts);
  ASSERT_NEAR(rainMonthly=0, rainGauge.currentMonth(), TOLERANCE);

  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly = 0, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-07 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);
  
  setTime("2022-09-08 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);
  
  setTime("2022-09-09 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-10 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-11 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-12 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-13 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-14 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-15 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-16 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-17 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);
  
  setTime("2022-09-18 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);
  
  setTime("2022-09-19 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-20 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-21 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-22 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-23 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-24 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-25 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-26 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-27 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-28 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-29 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-30 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);

  // New month
  setTime("2022-10-01 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly = 0, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-10-02 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);
}


/*
 * Test rainfall during past hour (with rain gauge overflow)
 */
void RainHourOv() {
  RainGauge rainGauge;
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


/*
 * Test rainfall during past hour (with rain gauge overflow),
 * timestamps across Midnight
 */
void RainHourOvMidnight() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;

  printf("< RainHourOvMidnight >\n");
  
  setTime("2022-09-06 23:00", tm, ts);
  rainGauge.update(tm, 10.0);
  DEBUG_CB();
  ASSERT_NEAR(0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 23:06", tm, ts);
  rainGauge.update(tm, 10.1);
  DEBUG_CB();
  ASSERT_NEAR(0.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 23:12", tm, ts);
  rainGauge.update(tm, 60.3);
  DEBUG_CB();
  ASSERT_NEAR(50.3, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 23:18", tm, ts);
  rainGauge.update(tm, 0.6);
  DEBUG_CB();
  ASSERT_NEAR(90.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 23:24", tm, ts);
  rainGauge.update(tm, 10.0);
  DEBUG_CB();
  ASSERT_NEAR(100.0, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 23:30", tm, ts);
  rainGauge.update(tm, 11.5);
  DEBUG_CB();
  ASSERT_NEAR(101.5, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-06 23:36", tm, ts);
  rainGauge.update(tm, 12.1);
  DEBUG_CB();
  ASSERT_NEAR(102.1, rainGauge.pastHour(), TOLERANCE);  

  setTime("2022-09-06 23:42", tm, ts);
  rainGauge.update(tm, 92.8);
  DEBUG_CB();
  ASSERT_NEAR(182.8, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-06 23:48", tm, ts);
  rainGauge.update(tm, 23.6);
  DEBUG_CB();
  ASSERT_NEAR(213.6, rainGauge.pastHour(), TOLERANCE);

  setTime("2022-09-06 23:54", tm, ts);
  rainGauge.update(tm, 14.5);
  DEBUG_CB();
  ASSERT_NEAR(304.5, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-07 00:00", tm, ts);
  rainGauge.update(tm, 15.5);
  DEBUG_CB();
  ASSERT_NEAR(305.5, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-07 00:06", tm, ts);
  rainGauge.update(tm, 5.5);
  DEBUG_CB();
  ASSERT_NEAR(405.5 - 10.1, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-07 00:12", tm, ts);
  rainGauge.update(tm, 17.8);
  DEBUG_CB();
  ASSERT_NEAR(417.8 - 60.3, rainGauge.pastHour(), TOLERANCE);
  
  setTime("2022-09-07 00:18", tm, ts);
  rainGauge.update(tm, 17.8);
  DEBUG_CB();
  ASSERT_NEAR(417.8 - 100.6, rainGauge.pastHour(), TOLERANCE);

}


/*
 * Test daily rainfall (with rain gauge overflow)
 */
void RainDailyOv() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;
  float     rainDaily = 0.0;

  printf("< RainDailyOv >\n");

  setTime("2022-09-06 8:00", tm, ts);
  rainGauge.update(tm, rainSensor = 0.0);
  ASSERT_NEAR(0, rainGauge.currentDay(), TOLERANCE);

  rainGauge.update(tm, rainSensor += 10.0);
  ASSERT_NEAR(rainDaily += 10, rainGauge.currentDay(), TOLERANCE);

  setTime("2022-09-06 12:00", tm, ts);
  rainGauge.update(tm, rainSensor += 42.0);
  ASSERT_NEAR(rainDaily += 42, rainGauge.currentDay(), TOLERANCE);

  setTime("2022-09-06 16:00", tm, ts);
  rainGauge.update(tm, rainSensor =  2.0);
  ASSERT_NEAR(rainDaily += 50, rainGauge.currentDay(), TOLERANCE);

  setTime("2022-09-06 20:00", tm, ts);
  rainGauge.update(tm, rainSensor += 53.0);
  ASSERT_NEAR(rainDaily += 53, rainGauge.currentDay(), TOLERANCE);

  setTime("2022-09-06 23:59", tm, ts);
  rainGauge.update(tm, rainSensor = 5.0);
  ASSERT_NEAR(rainDaily += 50, rainGauge.currentDay(), TOLERANCE);

  // Next Day
  setTime("2022-09-07 00:00", tm, ts);
  rainGauge.update(tm, rainSensor=42.0);
  ASSERT_NEAR(0, rainGauge.currentDay(), TOLERANCE);  

  setTime("2022-09-07 04:00", tm, ts);
  rainGauge.update(tm, rainSensor= 2.0);
  ASSERT_NEAR(60, rainGauge.currentDay(), TOLERANCE);  
}

/*
 * Test weekly rainfall (with rain gauge overflow)
 */
void RainWeeklyOv() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor;
    
  printf("< RainWeeklyOv >\n");
   
  setTime("2022-09-06 8:00:00", tm, ts);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);

  rainGauge.update(tm, rainSensor = 10.0);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);

  setTime("2022-09-06 16:00:00", tm, ts);
  rainGauge.update(tm, rainSensor =  0);
  ASSERT_NEAR(90, rainGauge.currentWeek(), TOLERANCE);
  
  setTime("2022-09-06 23:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 60.0);
  ASSERT_NEAR(150, rainGauge.currentWeek(), TOLERANCE);
  
  setTime("2022-09-07 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 20.0);
  ASSERT_NEAR(210, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-08 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 10.0);
  ASSERT_NEAR(300, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-09 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=  5.0);
  ASSERT_NEAR(395, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-10 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 10.0);
  ASSERT_NEAR(400, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-11 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 35.0);
  ASSERT_NEAR(425, rainGauge.currentWeek(), TOLERANCE);  

  // Next Week
  setTime("2022-09-12 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=50.0);
  ASSERT_NEAR(0, rainGauge.currentWeek(), TOLERANCE);  

  setTime("2022-09-13 04:00:00", tm, ts);
  rainGauge.update(tm, rainSensor=80.0);
  ASSERT_NEAR(30, rainGauge.currentWeek(), TOLERANCE);  

}


/*
 * Test monthly rainfall (no rain gauge overflow)
 */
void RainMonthlyOv() {
  RainGauge rainGauge;
  rainGauge.reset();

  tm        tm;
  time_t    ts;
  float     rainSensor = 0;
  float     rainMonthly;
    
  printf("< RainMonthlyOv >\n");
  
  setTime("2022-09-06 12:00:00", tm, ts);
  ASSERT_NEAR(rainMonthly=0, rainGauge.currentMonth(), TOLERANCE);

  rainGauge.update(tm, rainSensor = 5);
  ASSERT_NEAR(rainMonthly = 0, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-07 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 50);
  ASSERT_NEAR(rainMonthly = 45, rainGauge.currentMonth(), TOLERANCE);
  
  setTime("2022-09-08 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 5);
  ASSERT_NEAR(rainMonthly = 100, rainGauge.currentMonth(), TOLERANCE);
  
  setTime("2022-09-09 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 55);
  ASSERT_NEAR(rainMonthly = 150, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-10 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 90);
  ASSERT_NEAR(rainMonthly = 185, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-11 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 55);
  ASSERT_NEAR(rainMonthly = 250, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-12 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 5);
  ASSERT_NEAR(rainMonthly = 300, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-13 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 65);
  ASSERT_NEAR(rainMonthly = 360, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-14 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 95);
  ASSERT_NEAR(rainMonthly = 390, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-15 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 0);
  ASSERT_NEAR(rainMonthly = 395, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-16 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 99);
  ASSERT_NEAR(rainMonthly = 494, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-17 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 1);
  ASSERT_NEAR(rainMonthly = 496, rainGauge.currentMonth(), TOLERANCE);
  
  setTime("2022-09-18 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 5);
  ASSERT_NEAR(rainMonthly = 500, rainGauge.currentMonth(), TOLERANCE);
  
  setTime("2022-09-19 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 25);
  ASSERT_NEAR(rainMonthly = 520, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-20 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 20);
  ASSERT_NEAR(rainMonthly = 615, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-21 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 80);
  ASSERT_NEAR(rainMonthly = 675, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-22 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 30);
  ASSERT_NEAR(rainMonthly = 725, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-23 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 40);
  ASSERT_NEAR(rainMonthly = 735, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-24 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 60);
  ASSERT_NEAR(rainMonthly = 755, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-25 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 90);
  ASSERT_NEAR(rainMonthly = 785, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-26 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 30);
  ASSERT_NEAR(rainMonthly = 825, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-27 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 80);
  ASSERT_NEAR(rainMonthly = 875, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-28 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 40);
  ASSERT_NEAR(rainMonthly = 935, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-29 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 10);
  ASSERT_NEAR(rainMonthly = 1005, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-09-30 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 15);
  ASSERT_NEAR(rainMonthly = 1010, rainGauge.currentMonth(), TOLERANCE);

  // New month
  setTime("2022-10-01 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor = 20);
  ASSERT_NEAR(rainMonthly = 0, rainGauge.currentMonth(), TOLERANCE);

  setTime("2022-10-02 12:00:00", tm, ts);
  rainGauge.update(tm, rainSensor+=5);
  ASSERT_NEAR(rainMonthly += 5, rainGauge.currentMonth(), TOLERANCE);
  
}


/*
 * Test that rain gauge values are preserved after sensor startup,
 * i.e. sensor reset or battery change
 */
void TestRainStartup(void) {
  RainGauge rainGauge;
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


TEST(TestRainGauge, TestRainGaugeBasic) {
      //TestRainStartup();
      RainHour();
      RainHourShort();
      RainHourLong();
      RainHourExtreme();
      RainDaily();
      RainWeekly();
      //RainMonthly();
      RainHourOv();
      RainHourOvMidnight();
      //RainDailyOv();
      //RainWeeklyOv();
      //RainMonthlyOv();
}
