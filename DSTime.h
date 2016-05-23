#pragma once

#include <DS1307RTC.h>

//Updates tmElements_t tm
extern tmElements_t tm;
void updateTimeStamp();

//Updates the timestamp and returns true if the second has changed
bool secondChanged();

//Returns true if the last secondChanged() call also changed the minute
bool minuteChanged();

//Returns a string matching YYYYMMDDHHMM. Uses the Minute argument, if provided
String minuteTimeStamp(char Minute = -1);

//Returns a string matching YYYYMMDDHHMMSS
String secondTimeStamp();

//Converts a 0 to 00, 8 to 08, etc.
String twoDigits(int n);

//Initializes values for minuteChanged and secondChanged. 
//Should run just before the loop begins
void StartDS1307();

