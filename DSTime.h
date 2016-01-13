#pragma once

#include <DS1307RTC.h>

//Updates tmElements_t tm
void updateTimeStamp();

extern tmElements_t tm;
//Return true if the minute or second has changed since the last time this was called
//If provided with a pointer, sets it to the previous value
bool minuteChanged(uint8_t* = NULL);
bool secondChanged(uint8_t* = NULL);

//Returns a string matching YYYYMMDDHH. Uses the Minute argument, if provided
String minuteTimeStamp(uint8_t Minute = tm.Minute);

//Converts a 0-9 into 00-09
String twoDigits(int n);

//Initializes values for minuteChanged and secondChanged. 
//Should run just before the loop begins
void StartDS1307();

