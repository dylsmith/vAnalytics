#include "Globals.h"
#include "Utility.h"
#include "DSTime.h"

unsigned long t_timer = 0;
void t_start(){
  t_timer = millis();
}

void t_end(){
  logprintln(String(millis() - t_timer));
  t_timer = millis();
}

void error(String msg){
  #ifdef DEBUG
    Serial.println("Error: " + msg);
  #endif
  
  File errorFile = FileSystem.open("/mnt/sd/errors.txt", FILE_APPEND);
  errorFile.println(secondTimeStamp() + ": " + msg);
  errorFile.close();
  
  while (1);
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
   #ifdef DEBUG
  if( sizeof(int) != 2 || 
    sizeof(byte) != 1 || 
    (byte)130 != (unsigned char)130 ){
    error(F("Environment checks failed"));
  }
  #endif
}

//runs a shell command and prints the response
void execShell(String command){
  Process p;
  p.runShellCommand(command);

  #ifdef DEBUG
  if(p.available() > 0){
    String s = "";
    while (p.available() > 0)
      s += p.read();
    logprintln(s);
  }
  #endif
}

void logprint(String s){

  #ifdef DEBUG
    Serial.print(s);
  #endif
  File f = FileSystem.open("/mnt/sd/log.txt", FILE_APPEND);
  f.print(s);
  f.close();
}
void logprintln(String s){
  logprint(s + "\n");
}



