#include <SoftwareSerial.h>
#include "DSTime.h"
#include "Utility.h"

tmElements_t tm;
void updateTimeStamp(){
  if(!RTC.read(tm)){
    if(RTC.chipPresent()){
      error(F("DS1307 is stopped"));
    }
    else{
      error(F("Read error! Check circuitry"));
    }
  }
}

byte g_prevMinute;
byte g_prevSecond;
bool secondChanged(){
  g_prevMinute = tm.Minute;
  g_prevSecond = tm.Second;
  updateTimeStamp();
  return g_prevSecond != tm.Second;
}

bool minuteChanged(){
  return g_prevMinute != tm.Minute;
}

String minuteTimeStamp(char Minute){
  if(Minute == -1){
    return twoDigits(tmYearToCalendar(tm.Year)) + twoDigits(tm.Month) + twoDigits(tm.Day) + twoDigits(tm.Hour) + twoDigits(tm.Minute);
  }
  else{
    return twoDigits(tmYearToCalendar(tm.Year)) + twoDigits(tm.Month) + twoDigits(tm.Day) + twoDigits(tm.Hour) + twoDigits(Minute);
  }
}

String secondTimeStamp(){
  return minuteTimeStamp() + twoDigits(tm.Second);
}


String twoDigits(int n){
  if(n >= 0 && n < 10)
    return "0" + String(n);
  return String(n);
}




void StartDS1307(){
  updateTimeStamp();
}

