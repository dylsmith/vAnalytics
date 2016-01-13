/*
   
   Todo:
     -Remove the code that switches back to AP mode when wifi can't connect (wifi-alive-or-dead.py or something). 
         Make the arduino wait for wifi instead. Should be easy
         
     -Implement an RTC 
        //Linux side: write wifi connected/disconnected status to a file
        Arduino side: 
          When a sync is needed, monitor internet connectivity status
          When connected, request internet time from linux, store to RTC
            Account for the fact that time may jump backwards, and data points may be repeated
              May even roll back to a previous minute
              Check the difference between RTC and internet time before setting. 
              If it rolls back, maybe just ignore repeated seconds to avoid problems
          Wait a while after syncing, then repeat

      -Update a 'size' file after each successful write of a buffer to a data file,
        to ensure we're power-loss safe

      -Account for storage edge-cases
        Check available space before writing a new file
        If it's too small delete the oldest file

      -Monitor the status of connected.sh and dataSync.sh. Ensure they're always runnig
        
     -Build a system to automatically connect to WiFi networks when in range (later)
        Use modified existing shell scripts, maybe
        Ping something (google), to check connectivity
          If connected, great
          If not, try to connect to networks. Pause for a while then repeat from the ping

     -When the VI.txt file changes, upload a new copy to the server
        Create a 'VIChanged' file, for use by the linux script below
        Use this to identify each vehicle by VIN
        When receiving data files, the validPIDs in the VI file are used to decode the data file

     //-Linux script:
        Every few seconds, check internet connectivity
        If connected:
          Write connection status to a file, for use by the arduino side
          If VIChanged exists, rupload the VI file to the server, and remove VIChanged
            This shouldn't happen other than at first creation
          Try to upload each data file to a temp directory on the server
            Append VIN (or some unique identifier)
            Once file copy is complete:
              Move the remote copy to a finished directory
              Delete the local copy
            After each upload, check internet connection (or maybe stop when a transfer fails)
        If not connected:
          Write connection status to a file

      -Server code:
        Monitors the finished directory, inserts finished files into the database

        
        File transfer options:
          SCP files onto server, monitore for new files and use them
          Use https directly (slow)
          libssh, sftp should work for now
        
        

     VehicleInfo.txt (VI.txt) format:
     first VINLength bytes: VIN (unsure of exact length)
     UNIMPLEMENTED: next byte: OBD protocol used (arbitrary, from the adapter's code) not sure if this can be extracted from the obd adapter easily
     next bytes, to end: each supported PID value

     data file format:
     title: minutes since epoch (second since epoch / 60)
     contents:
       timestamp: 1 byte, represents seconds since start of timestamp
       validPIDCount ints (either 2 or 4 bytes), in the order listed in VI.txt
       repeat 60 times

     -Linux script:
        Every few seconds, check internet connectivity
        If connected:
          Write connection status to a file, for use by the arduino side
          If VIChanged exists, rupload the VI file to the server, and remove VIChanged
            This shouldn't happen other than at first creation
          Try to upload each data file to a temp directory on the server
            Append VIN (or some unique identifier)
            Once file copy is complete:
              Move the remote copy to a finished directory
              Delete the local copy
            After each upload, check internet connection (or maybe stop when a transfer fails)
        If not connected:
          Write connection status to a file

*/

/*

  Bash:

  Once a second:
    Check internet connectivity (ping google)
    If not connected:
        Remove file /mnt/sd/connected
    If connected:
        Create file /mnt/sd/connected
        If "mnt/sd/VIChanged exists:
            Upload /mnt/sd/VI.txt to server (scp)
            Remove /mnt/sd/VIChanged
        For each file in /mnt/sd/data (excluding subfolder /tmp):
            Upload file to server's temp directory (scp)
            Move to finished directory on server
            Delete local copy
            Break out of loop when an scp tranfer fails, or check internet connetivity after each transfer



(Once a second)
  wget -q --tries=10 --timeout=20 --spider google.com
  if [[ $? -eq 0 ]]; then
    touch /mnt/sd/connected
    (If /mnt/sd/VIChanged exists); then
      scp /mnt/sd/VI.txt (server):/data/tmp/VI.txt
      ssh (server) 'mv /data/tmp/VI.txt /data/VI.txt'
    fi
    (for each file in /mnt/sd/data, excluding /tmp subdirectory):
      scp file (server):/data/tmp/file
      ssh (server) 'mv /data/tmp/file /data/file'
      rm file
  else
    rm /mnt/sd/connected
  fi
      
      Then either:
      if (not connected) break;
      or break when scp/ssh commands fail somehow






watch -n 1
  wget -q --tries=10 --timeout=20 --spider google.com
  if [[ $? -eq 0 ]]; then
    touch /mnt/sd/connected
    if [ -e /mnt/sd/VIChanged ]; then
      scp /mnt/sd/VI.txt (server):/data/tmp/VI.txt
      ssh $1 'mv /data/tmp/VI.txt /data/VI.txt'
    fi
    for file in /mnt/sd/data/*
      scp file $1:/data/tmp/file
    ` if [ $? -ne 0]; then
        break
      fi
      ssh $1 'mv /data/tmp/file /data/file'
      rm file
  else
    rm /mnt/sd/connected
  fi
      
      Then either:
      if (not connected) break;
      or break when scp/ssh commands fail somehow

//Send file to server. If sent succesfully, move it out of them temp directory. If moved successfully, delete local copy
sendFile.sh (filename):
  scp $1 (server):/tmp/$1
  if [ $? = 0 ]; then
    ssh (server) 'mv /tmp/$1 /$1'
    if[ $? = 0 ]; then
      rm $1
    fi
  fi


connection.sh:
  watch -n 1
    wget -q --tries=2 --timeout=2 --spider google.com
    if [[ $? -eq 0 && ! -e /mnt/sd/connected ]]; then   #if connected and previously wasn't
      touch /mnt/sd/connected
      ./filesend.sh
      
    
    
      
      

      
    
       
 */




/*
PIDS:

0x04 PID_ENGINE_LOAD
0x05 PID_COOLANT_TEMP
0x06 PID_SHORT_TERM_FUEL_TRIM_1
0x07 PID_LONG_TERM_FUEL_TRIM_1
0x08 PID_SHORT_TERM_FUEL_TRIM_2
0x09 PID_LONG_TERM_FUEL_TRIM_2
0x0A PID_FUEL_PRESSURE
0x0B PID_INTAKE_MAP
0x0C PID_RPM
0x0D PID_SPEED
0x0E PID_TIMING_ADVANCE
0x0F PID_INTAKE_TEMP
0x10 PID_MAF_FLOW
0x11 PID_THROTTLE
0x1E PID_AUX_INPUT
0x1F PID_RUNTIME
0x21 PID_DISTANCE_WITH_MIL
0x2C PID_COMMANDED_EGR
0x2D PID_EGR_ERROR
0x2E PID_COMMANDED_EVAPORATIVE_PURGE
0x2F PID_FUEL_LEVEL
0x30 PID_WARMS_UPS
0x31 PID_DISTANCE
0x32 PID_EVAP_SYS_VAPOR_PRESSURE
0x33 PID_BAROMETRIC
0x3C PID_CATALYST_TEMP_B1S1
0x3D PID_CATALYST_TEMP_B2S1
0x3E PID_CATALYST_TEMP_B1S2
0x3F PID_CATALYST_TEMP_B2S2
0x42 PID_CONTROL_MODULE_VOLTAGE
0x43 PID_ABSOLUTE_ENGINE_LOAD
0x45 PID_RELATIVE_THROTTLE_POS
0x46 PID_AMBIENT_TEMP
0x47 PID_ABSOLUTE_THROTTLE_POS_B
0x48 PID_ABSOLUTE_THROTTLE_POS_C
0x49 PID_ACC_PEDAL_POS_D
0x4A PID_ACC_PEDAL_POS_E
0x4B PID_ACC_PEDAL_POS_F
0x4C PID_COMMANDED_THROTTLE_ACTUATOR
0x4D PID_TIME_WITH_MIL
0x4E PID_TIME_SINCE_CODES_CLEARED
0x52 PID_ETHANOL_FUEL
0x59 PID_FUEL_RAIL_PRESSURE
0x5B PID_HYBRID_BATTERY_PERCENTAGE
0x5C PID_ENGINE_OIL_TEMP
0x5D PID_FUEL_INJECTION_TIMING
0x5E PID_ENGINE_FUEL_RATE
0x61 PID_ENGINE_TORQUE_DEMANDED
0x62 PID_ENGINE_TORQUE_PERCENTAGE
0x63 PID_ENGINE_REF_TORQUE

*/
