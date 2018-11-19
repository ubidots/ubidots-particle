// This example is to get the last value of variable from the Ubidots API

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

#ifndef UBIDOTS_TOKEN
#define UBIDOTS_TOKEN "...."  // Put here your Ubidots TOKEN
#endif

/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/


Ubidots ubidots(UBIDOTS_TOKEN);

void setup() {
    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    /*
    * Obtains values using TCP according to structure specified at
    * https://ubidots.com/docs/hw
    */
    float value = ubidots.get("weather-station", "temperature");

    // Evaluates the results obtained
    if(value!=ERROR_VALUE){
      Serial.print("value:");
      Serial.println(value);
    }
    delay(5000);
}
