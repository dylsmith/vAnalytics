#pragma once 

#include <FileIO.h>
#include "Globals.h"

//t_end() prints the time since the last t_start / t_end call
void t_start();
void t_end();

//Prints the error to Serial, /mnt/sd/errors.txt, and loops forever
void error(String msg = "");

//{laceholders to emulate getting real data
uint32_t getTimeStamp();
int getData();

//Make sure the environment is sane..
//Types of the expected size, etc.
void environmentChecks();

//Runs a shell command and prints the response
void execShell(String command);



