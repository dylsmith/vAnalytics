#include "VehicleInfo.h"
#include "DSTime.h"

//OBD comms are on SoftwareSerial(A10,A11)
COBD obd;

//All PIDs apparently made available by the OBD-II UART adapter I'm using
byte allPIDs[] = {
0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 
0x0E, 0x0F, 0x10, 0x11, 0x1E, 0x1F, 0x21, 0x2C, 0x2D, 0x2E, 
0x2F, 0x30, 0x31, 0x32, 0x33, 0x3C, 0x3D, 0x3E, 0x3F, 0x42, 
0x43, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 
0x4E, 0x52, 0x59, 0x5B, 0x5C, 0x5D, 0x5E, 0x61, 0x62, 0x63 };                  
#define NUM_PIDS sizeof(allPIDs)

byte *validPIDs;    //A list of PIDs that this vehicle can actually give
byte validPIDCount; 

byte *dataBuffer;   //Holds all the data received in one loop
#define BUFFER_SIZE validPIDCount * 2 + 1 //One byte for seconds, validPIDCount*2 for each data point (2 bytes)

void OBDSetup(){
    obd.begin();
    while(!obd.init());
}

void readAllPIDs(){
  //Open the file, with name <minutes since epoch>
  String timestamp = minuteTimeStamp();
  
  String fileName = "/mnt/sd/tmp/" + timestamp;
  #ifdef DEBUG
    //Serial.println("Writing to " + timestamp);
  #endif
  
  File output = FileSystem.open(fileName.c_str(), FILE_APPEND);
  if(!output)
    error("Error opening " + fileName);
    
  //Record seconds since the start of the minute
  dataBuffer[0] = tm.Second;

  //Record each data point
  int result;  
  for(byte i = 0; i < validPIDCount; i++){
    #ifdef DEBUG
      result = getData();
    #else
      obd.read(validPIDs[i], result);
    #endif
    dataBuffer[i * 2 + 1] = result >> 8;
    dataBuffer[i * 2 + 2] = result;
  }

  //Write data buffer to the file
  digitalWrite(13, HIGH);
  int writeSize = output.write(dataBuffer, BUFFER_SIZE);
  digitalWrite(13, LOW);
  output.close();

  if(writeSize != BUFFER_SIZE)
    error(F("Wrote wrong size data"));

  //TODO: Update a second file, counting the number of successful buffer
  //writes to the temp file. When starting up, trim the file in the tmp
  //directory based on this, and move it to the data directory to be sent out
  
}

void createVehicleInfo(){
  //Open VI. Once complete, rename VI.txt. 
  File vinfo = FileSystem.open("/mnt/sd/VI", FILE_APPEND);

  //Get VIN
  byte vin[VIN_LENGTH];
  #ifdef DEBUG
    for(byte i = 0; i < VIN_LENGTH; i++)
      vin[i] = 48; //'a'
  #else
    if(!obd.getVIN(vin, VIN_LENGTH))
      error(F("Could not read VIN"));
  #endif

  //Write VIN to file. 
  //byte writeSize = vinfo.write(reinterpret_cast<const byte*>(vin), VIN_LENGTH); //Convert from char (from OBD reader) to const unsigned char (for file write)
  byte writeSize = vinfo.write((byte*)vin, VIN_LENGTH); //Convert from char (from OBD reader) to const unsigned char (for file write)
  vinfo.flush();
  if(writeSize != VIN_LENGTH)
    error(F("VIN not printed to file properly"));
    
  //for each possible PID, check if the request is successful.
  //if so, store it in validPIDs and increment validPIDCount
  validPIDs = (byte*)malloc(NUM_PIDS); 
  int result;
  validPIDCount = 0;
  for(byte i = 0; i < NUM_PIDS; i++){
    byte PID = allPIDs[i];
    #ifdef DEBUG
      vinfo.write(PID);
      validPIDs[validPIDCount] = PID;
      ++validPIDCount;
    #else
      if(obd.read(PID, result)){
        vinfo.write(PID);
        validPIDs[validPIDCount] = PID;
        ++validPIDCount;
      }
    #endif
  }
  
  vinfo.close();
  
  if(validPIDCount == 0)
    error(F("No valid PIDs on vinfo creation"));

    
  execShell("mv /mnt/sd/VI /mnt/sd/VI.txt");
}

void checkDebugModeSwitch(){
  #ifdef DEBUG
    bool debug = true;
  #else
    bool debug = false;
  #endif;

  //Empty file debug exists if we last ran in debug mode
  bool prevState = FileSystem.exists("/mnt/sd/debug");

  if(debug == true && prevState == false){
    execShell(F("touch /mnt/sd/debug"));
    FileSystem.remove("/mnt/sd/VI.txt");
  }
  else if(debug == false && prevState == true){
    FileSystem.remove("/mnt/sd/debug");
    FileSystem.remove("/mnt/sd/VI.txt");
  }      
}

void loadVehicleInfo(){
  File vinfo = FileSystem.open("/mnt/sd/VI.txt", FILE_READ);
  
  //TODO: change this calculation if the vinfo file is changed
  validPIDCount = vinfo.size() - VIN_LENGTH;  //File is 1 byte per valid PID and VIN_LENGTH bytes for the VIN.
  
  if(validPIDCount <= 0)
    error(F("No valid PIDs on vinfo load"));
  
  validPIDs = validPIDs = (byte*)malloc(validPIDCount);

  //TODO: Check that the loaded VIN is identical to obd.getVIN(..)
  
  vinfo.seek(VIN_LENGTH);
  for(byte i = 0; i < validPIDCount; i++)
    validPIDs[i] = vinfo.read();
    
  vinfo.close();
}

void checkVehicleInfo(){
  checkDebugModeSwitch(); 
  if (FileSystem.exists("/mnt/sd/VI.txt"))
    loadVehicleInfo();
  else
    createVehicleInfo();
 
  dataBuffer = (byte*)malloc(BUFFER_SIZE); //2 bytes per data point + 1 for current second
  
}

