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
  // ubidots.setDebug(true);  // Uncomment this line for printing debug messages
}

void loop() {
  float value = analogRead(A0);
  Serial.println("Adding value");

  /* Adds context key-value pairs */
  ubidots.addContext("weather-status", "sunny");
  ubidots.addContext("time", "11:40:56 pm");

  /* Reserves memory to store context array */
  char* context = (char*)malloc(sizeof(char) * 60);

  /* Builds the context with the array above to send to Ubidots */
  // IMPORTANT: The IoT Protocol must match with the one chosen for the gateway
  ubidots.getContext(context, UBI_HTTP);
  Serial.printlnf("Context: %s", context);
  ubidots.add("variable-label", value, context);

  /* Will send data to a device label that matches the device ID */
  ubidots.meshPublishToUbidots();
  free(context);
  Serial.println("finished");

  /* WARNING a very high sample rate may give you issues in your gateway routine
   */
  delay(20000);
}
