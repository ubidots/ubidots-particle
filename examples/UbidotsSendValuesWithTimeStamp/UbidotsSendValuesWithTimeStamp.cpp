/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#endif

Ubidots ubidots(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

//

/****************************************
 * Main Functions
 ****************************************/


void setup() {
  Serial.begin(115200);
  //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}


void loop() {
  float value1 = analogRead(A0);
  unsigned long t = Time.now(); // calculates your actual timestamp in SECONDS

  ubidots.add("test-1", value1);  // Change the first argument for your var's label
  ubidots.add("test-2", value1, NULL, t-3600);  // Set custom timestamp, -3600 seconds (1hr)
  ubidots.add("test-3", value1);  // Default timestamp applied at SEND time

  bool bufferSent = false;
  
  if(ubidots.isDirty()){  // There are stored values in buffer
    // Variables 'test-1' and 'test-3' will be sent with time-sent timestamp
    // Variable 'test-2' will be sent with previously applied custom timestamp
    bufferSent = ubidots.sendAll(); 
  }

  if(bufferSent){
      // Do something if values were sent properly
      Serial.println("Values sent by the device");
  }

  delay(5000);
}
