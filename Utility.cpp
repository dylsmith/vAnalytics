#include "Utility.h"

unsigned long timer_t = 0;
void t_start(){
  #ifdef DEBUG
  timer_t = millis();
  #endif
}

void t_end(){
  #ifdef DEBUG
  Serial.print(timer_t = millis() - timer_t);
  #endif
}

void error(String msg){
  #ifdef DEBUG
    Serial.println(msg);
  #endif
  
  File errorFile = FileSystem.open("/mnt/sd/errors.txt", FILE_APPEND);
  errorFile.println(msg);
  errorFile.close();
  
  while (1);
}

//TODO: Remove this and its header proto once DS1307 is implemented
/*
uint32_t ts = 10000;
uint32_t getTimeStamp(){
  //TODO: replace ts with seconds since epoch
  return ts++; //Every call, one 'second' will pass
}
*/

int dt = 0;
int getData(){
  return 0x3030;
  //return dt++;
}

//Make sure the environment is sane..
void environmentChecks(){
  /*
   * Assumptions:  
   * -A byte is.. one byte. Lots of byte-arrays need this
   * -An int is 2 bytes. Write them as two bytes, bitshifted. This probably 
   *   isnt 100% necessary as the higherbits just shoulen't be used anyways
   * -Bytes are unsigned chars (not signed chars)
   */
  if( sizeof(int) != 2 || 
      sizeof(byte) != 1 || 
      (byte)130 != (unsigned char)130
    )
    error(F("Environment checks failed"));
}

//runs a shell command and prints the response
void execShell(String command){
  Process p;
  p.runShellCommand(command);

  #ifdef DEBUG
    bool printed = p.available() > 0;
    while (p.available() > 0)
      Serial.print(char(p.read()));
    if(printed) Serial.println();
  #endif
}



