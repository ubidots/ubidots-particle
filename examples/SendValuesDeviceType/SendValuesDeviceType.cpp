// This example sends data to multiple variables to 
// Ubidots through HTTP protocol, and uses Device Types feature

// IMPORTANT: Device type are only supported through HTTP

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Instances and Constants
 ****************************************/

const char * device_label = "my-device"; // edit here your device label
const char * device_type = "my-type"; // edit here your device type label
char * TOKEN = "Your_Token"; // edit here your account token
char * device;

Ubidots ubidots(TOKEN, UBI_HTTP);


/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  sprintf(device, "%s/?type=%s", device_label, device_type);
  //ubidots.setDebug(true);  // Uncomment this line for printing debug messages
}

void loop() {
  float value1 = analogRead(A0);
  float value2 = analogRead(A1);
  float value3 = analogRead(A2);
  ubidots.add("Variable_Name_One", value1);  // Change for your variable name
  ubidots.add("Variable_Name_Two", value2);
  ubidots.add("Variable_Name_Three", value3);

  bool bufferSent = false;
  bufferSent = ubidots.send(device);  // Will send data to a device label that matches the device Id

  if(bufferSent){
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  delay(5000);
}
