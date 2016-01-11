#pragma once

#include "Globals.h"
#include "Utility.h"
#include <OBD.h>
#include <FileIO.h>

#define VIN_LENGTH 64 //# bytes reserved for the VIN

//Initializes the obd adapter
void OBDSetup();

//For each valid PID, sends an OBD request and writes the result to the SD card.
//File format is: one byte for seconds since this minute started, two bytes for each data point read
//Each file stores one minute of time, and the filename represents minutes since epoch
void readAllPIDs();

//Creates a vehicle info file or loads if from /mnt/sd/VI.txt
//File format is: <VIN_LENGTH> bytes for the VIN, followed by one byte per valid PID in this vehicle
void checkVehicleInfo();

//Create a vehicle info file
void createVehicleInfo();

//Loads vehicle info from /mnt/sd/VI.txt
void loadVehicleInfo();

//Debug mode writes filler data to VI.txt. This checks for when we change the 
//compile flags between debug and not-debug and removes that file, so it's recreated properly
void checkDebugModeSwitch();
