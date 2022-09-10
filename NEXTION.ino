void nextionInput(){

  dfd += char(Serial2.read());
  // COMMAND is 3 characters after C:C
  // RESET dfd if THREE characters received and not C:C
  if(dfd.length()>3 && dfd.substring(0,3)!="C:C") dfd="";
  else{
    // If string ends in a ? then command completed
    if(dfd.substring((dfd.length()-1),dfd.length()) == "?"){
      // Get the command
      String command = dfd.substring(3,6);
      // Get the value(int or string)
      String value = dfd.substring(6,dfd.length()-1);
      // FOR TESTING
      Serial.println(command + " : " + value);
      if(command == "LED"){
        value == "ON" ? aluw[1] = 1 : aluw[1] = 0;
      }
      if(command == "USB"){
        value == "ON" ? aluw[2] = 1 : aluw[2] = 0;
      }
      if(command == "WLS"){
        value == "ON" ? aluw[3] = 1 : aluw[3] = 0;
      }
      if(command == "ACC"){
        value == "ON" ? aluw[0] = 1 : aluw[0] = 0;
      }
      dfd="";
    }
  }
}
 
 void nextionSendFloatData(float data, char *selector)
 {
   Serial2.print(selector); Serial2.print("="); Serial2.print(data);
   Serial2.write(ndt,3);// command terminated by sending (0xff) 3 times
 }

 void nextionSendFloatAsStringData(float data, char *selector)
 {
   Serial2.print(selector);
   Serial2.print("="); Serial2.print("\""); Serial2.print(data); Serial2.print("\"");
   Serial2.write(ndt,3);// command terminated by sending (0xff) 3 times
 }

 void nextionSendIntegerData(int data, char *selector)
 {
   Serial2.print(selector); Serial2.print("="); Serial2.print(data);
   Serial2.write(ndt,3);// command terminated by sending (0xff) 3 times
 }
 
 void nextionSendIntegerAsStringData(int data, char *selector)
 {
   Serial2.print(selector); 
   Serial2.print("="); Serial2.print("\""); Serial2.print(data); Serial2.print("\"");
   Serial2.write(ndt,3);// command terminated by sending (0xff) 3 times
 }
 
 void nextionSendStringData(char* data, char *selector)
 {
   Serial2.print(selector); 
   Serial2.print("="); Serial2.print("\""); Serial2.print(data); Serial2.print("\"");
   Serial2.write(ndt,3);// command terminated by sending (0xff) 3 times
 }

  void nextionSetPictureVisibility(int data, char *selector)
 {
   Serial2.print("vis "); Serial2.print(selector); Serial2.print(","); Serial2.print(data);
   Serial2.write(ndt,3);// command terminated by sending (0xff) 3 times
 }

  void nextionSetButtonState(int data, char *selector)
 {
   Serial2.print(selector); Serial2.print(".val="); Serial2.print(data);
   Serial2.write(ndt,3);// command terminated by sending (0xff) 3 times
 }
