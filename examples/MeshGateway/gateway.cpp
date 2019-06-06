/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Instances and Constants
 ****************************************/

#ifndef UBIDOTS_TOKEN
#define UBIDOTS_TOKEN "..."  // Put here your Ubidots TOKEN
#endif

Ubidots ubidots(UBIDOTS_TOKEN, UBI_INDUSTRIAL, UBI_MESH);
//Ubidots ubidots(UBIDOTS_TOKEN, UBI_EDUCATIONAL, UBI_MESH); Replace the above line if you're an Ubidots for Education user.
/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  // Uncomment this line to choose a different cloud Protocol to send data
  // ubidots.setCloudProtocol(UBI_HTTP);

  // Uncomment this line for printing debug messages
  // ubidots.setDebug(true);
}

void loop() { ubidots.meshLoop(); }
