#include <SoftwareSerial.h>
#include "Globals.h"
#include "VehicleInfo.h"
#include "Utility.h"
#include "DSTime.h"





void setup() {
  delay(1000);


  //For Yun devices. Handles communication between arduino and linux processors
  Bridge.begin();

  //Allows SD card access
  FileSystem.begin();

  /*
    LCD_SH1106 lcd;
    lcd.begin();

    lcd.clear();
    lcd.setFontSize(FONT_SIZE_MEDIUM);
    lcd.println("Hello, world!");
    delay(1000);
  */

  //If in debug mode, start the serial connection to communicate with a pc
  //Otherwise, begin the OBD connection
#ifdef DEBUG
  Serial.begin(9600);
  delay(1000);
  while (!Serial);
#else
  OBDSetup();
#endif


  //Remove temporary files TODO: validate and use them instead)
  execShell("mv /mnt/sd/data/tmp/* /mnt/sd/data/partial");

  //Run linux-side code
  Process startup;
  startup.runShellCommandAsynchronously("/mnt/sd/scripts/c.sh");

  //Check the arduino environment is good
  #ifdef DEBUG
  environmentChecks();
  #endif

  //Set high when writing to SD card
  pinMode(13, OUTPUT);

  //Check that the vehicle info file exists and create if it not
  checkVehicleInfo();

  //Start RTC
  StartDS1307();

}

void syncLinuxTime(){
  Process p;
  p.runShellCommand("date +%s");
  String unixTime;
  while(p.available() > 0){
    unixTime = p.readString();
  }
 logprintln("Set time to " + unixTime);
 RTC.set(time_t(strtoul(unixTime.c_str(), NULL, 10)));
}


void loop() {

  //Wait for the start of a new second
  if (secondChanged()) {
    unsigned long timer = millis();

    readAllPIDs();

    if (FileSystem.exists("/mnt/sd/flags/connected"))
      logprintln("Connected");
    else
      logprintln("Not connected");

    if (minuteChanged()) {
      String Name = minuteTimeStamp((tm.Minute - 1) % 60);
      logprintln(Name + " done");
      execShell("mv /mnt/sd/data/tmp/" + Name + " /mnt/sd/data/complete/" + Name);

      Process sync;
      sync.runShellCommandAsynchronously("/mnt/sd/scripts/dataSync.sh");
    }

#ifdef DEBUG
    Serial.print(secondTimeStamp() + ": ");
    Serial.print(F("Main loop took "));
    Serial.print(millis() - timer);
    Serial.println(F(" ms"));
#endif


  }
}
