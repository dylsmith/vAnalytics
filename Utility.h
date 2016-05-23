#pragma once 

#include <FileIO.h>

//t_end() prints the time since the last t_start / t_end call
extern unsigned long t_timer;
void t_start();
void t_end();

//Prints the error to Serial, /mnt/sd/errors.txt, and loops forever
void error(String msg = "");

//Make sure the environment is sane..
//Types of the expected size, etc.
void environmentChecks();

//Runs a shell command and prints the response
void execShell(String command);

//Print a message to a log file
void logprint(String s);
void logprintln(String s);



