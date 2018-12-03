// This example sends data to multiple variables to 
// Ubidots using Particle Webhooks

// IMPORTANT NOTE: Be aware you need to first create a webhook
// on Particle Console according to steps given in:
// https://help.ubidots.com/connect-your-devices/connect-your-particle-device-to-ubidots-using-particle-webhooks

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Instances and Constants
 ****************************************/

const char* WEBHOOK_NAME = "my-webhook";

Ubidots ubidots("webhook", UBI_PARTICLE);


/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
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
  bufferSent = ubidots.send(WEBHOOK_NAME, PUBLIC);  // Will use particle webhooks to send data

  if(bufferSent){
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }

  delay(5000);
}
