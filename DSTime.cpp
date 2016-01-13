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

uint8_t currentSecond;
bool secondChanged(uint8_t* prevSecond){
  updateTimeStamp();
  if(currentSecond == tm.Second){
    return false;
  }
  else{
    if(prevSecond)
      *prevSecond = currentSecond;
    currentSecond = tm.Second;
    return true;
  }
}

uint8_t currentMinute;
bool minuteChanged(uint8_t* prevMinute){
  updateTimeStamp();
  if(currentMinute == tm.Minute){
    return false;
  }
  else{
    if(prevMinute)
      *prevMinute = currentMinute;
    currentMinute = tm.Minute;
    return true;
  }
}

String minuteTimeStamp(uint8_t Minute){
  if(Minute == (uint8_t)-1){
  return twoDigits(tmYearToCalendar(tm.Year)) + twoDigits(tm.Month) + twoDigits(tm.Day) + twoDigits(tm.Hour) + twoDigits(tm.Minute);
  }
  else{
    return twoDigits(tmYearToCalendar(tm.Year)) + twoDigits(tm.Month) + twoDigits(tm.Day) + twoDigits(tm.Hour) + twoDigits(Minute);
  }
}


String twoDigits(int n){
  if(n >= 0 && n < 10)
    return "0" + String(n);
  return String(n);
}




void StartDS1307(){
  updateTimeStamp();
  currentSecond = tm.Second;
  currentMinute = tm.Minute;
}

