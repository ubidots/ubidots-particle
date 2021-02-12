// This example retrieves last value of a variable from the Ubidots API
// using HTTP protocol.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

#ifndef UBIDOTS_TOKEN
#define UBIDOTS_TOKEN ""  // Put here your Ubidots TOKEN
#endif

// Uncomment this line to print debug messages
#define UBIDOTS_LOGGING TRUE

Ubidots ubidots(UBIDOTS_TOKEN, UBI_TCP);

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);
  ubidots.setDebug(true);
}

void loop() {
  /* Obtain multiple last values from a comma separated variables list */
  const char* deviceLabel = "demo";
  const char* variablesList = "var1,var2,var3";
  tcpMap myMap = ubidots.getMultipleValues(deviceLabel, variablesList);

  /* Access to retrieved values based on the ordered list returned */
  float var1_value = myMap[0];
  float var2_value = myMap[1];
  float var3_value = myMap[2];

  // Evaluates the results obtained
  if (var1_value != ERROR_VALUE) {
    Serial.printf("Value for variable var_1: %f", var1_value);
  }
  if (var2_value != ERROR_VALUE) {
    Serial.printf("Value for variable var_2: %f", var2_value);
  }
  if (var3_value != ERROR_VALUE) {
    Serial.printf("Value for variable var_2: %f", var3_value);
  }
  delay(5000);
}
