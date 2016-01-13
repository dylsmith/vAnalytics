#include <SoftwareSerial.h>
#include "Globals.h"
#include "VehicleInfo.h"
#include "Utility.h"

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

  execShell("rm /mnt/sd/tmp/*");
  Process p; p.begin("/mnt/sd/scripts/startup.sh"); p.runAsynchronously();

  environmentChecks();
  pinMode(13, OUTPUT);     
  Serial.println(F("Filesystem, serial, bridge started"));


  checkVehicleInfo();  
}

void loop() {
  unsigned long timer = millis() - 1000;  
  while (1)
  {
    if (millis() - timer >= 1000) //TODO: change this to monitor the RTC instead
    {
      unsigned long startOfLoop = millis();
      timer += 1000;

      //do stuff here
      readAllPIDs();

      #ifdef DEBUG
      Serial.print(F("Collecting data took "));
      Serial.print(millis() - startOfLoop);
      Serial.println(F(" ms"));
      #endif
    }
  }
}
