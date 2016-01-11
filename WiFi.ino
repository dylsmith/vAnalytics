/*
void connectToWifi(String ssid, String key, String encryption)
{
  execShell("wifi down");
  if (FileSystem.exists("/etc/config/wireless"))
    FileSystem.remove("/etc/config/wireless");
  execShell("cp wirelessConfig /etc/config/wireless");
  execShell("/sbin/uci \"set\" \"wireless.@wifi-iface[0].ssid=" + ssid + "\"");
  execShell("/sbin/uci \"set\" \"wireless.@wifi-iface[0].key=" + key + "\"");
  execShell("/sbin/uci \"set\" \"wireless.@wifi-iface[0].encryption=" + encryption + "\"");
  execShell("/sbin/uci commit wireless");
  execShell("wifi");

}


  //connectToWifi("aaaahhh", "turtles?", "psk");
  //execShell("ifconfig wlan0");
  //execShell("cat /etc/config/wireless");
  //execShell("cp /etc/config/wireless wirelessConfig");
  //execShell("cat wirelessConfig");
  //execShell("/usr/bin/pretty-wifi-info.lua");
*/

/*
 *   
  if(FileSystem.exists("/mnt/sd/pt"))
    while(1);
  execShell("rm /mnt/sd/powerlosstest");
  execShell("touch /mnt/sd/pt");
  File t = FileSystem.open("/mnt/sd/powerlosstest", FILE_APPEND);

  int sizee = 26 * 2 + 5;
  byte *s = (byte*)malloc(sizee);
  for(int i = 0; i < sizee; i++) s[i] = i + 65;
  Serial.write(s, sizee);
  while(1)
  {
    digitalWrite(13, HIGH);
    t.write(s, sizee);
    digitalWrite(13, LOW);
  }

  */
