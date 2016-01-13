#include <SoftwareSerial.h>
#include "Globals.h"
#include "VehicleInfo.h"
#include "Utility.h"
#include "DSTime.h"


void setup() {
  delay(500);

  Bridge.begin();
  FileSystem.begin();
  
  #ifdef DEBUG
    Serial.begin(9600);
    while (!Serial);
  #else
    OBDSetup();
  #endif

  Serial.println(F("Filesystem, serial, bridge started"));
  execShell("rm /mnt/sd/tmp/*");
  //Process p; p.begin("/mnt/sd/scripts/startup.sh"); p.runAsynchronously();
  Process startup; startup.runShellCommandAsynchronously("/mnt/sd/scripts/startup.sh");

  environmentChecks();
  pinMode(13, OUTPUT);     

  checkVehicleInfo();  
  StartDS1307(); 
}

void loop() {

  //Wait for the start of a new second
  if(secondChanged()){
    unsigned long timer = millis();
  
    readAllPIDs();
    uint8_t prevMinute;
    if(minuteChanged(&prevMinute)){
      String Name = minuteTimeStamp(prevMinute);
      #ifdef DEBUG
        Serial.println(Name + " done");
      #endif
      execShell("mv /mnt/sd/tmp/" + Name + " /mnt/sd/data/" + Name);
    }
  
    #ifdef DEBUG
      Serial.print(F("Main loop took "));
      Serial.print(millis() - timer);
      Serial.println(F(" ms"));
    #endif

  
  }
}
