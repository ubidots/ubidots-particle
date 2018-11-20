// This example is to get the last value of variable from the Ubidots API

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
    float value = analogRead(A0);

    /* Adds context key-value pairs */
    ubidots.addContext("weather-status", "sunny");
    ubidots.addContext("time", "11:40:56 pm");

    /* Reserves memory to store context array */
    char* context = (char *) malloc(sizeof(char) * 60);

    /* Builds the context with the coordinates to send to Ubidots */
    ubidots.getContext(context);

    /* Sends the position */
    ubidots.add("temperature", value, context);  // Change for your variable name

    if (ubidots.send("weather-station")) {  // Sends position to a device with label that matches the Particle Device Id
      Serial.println("Values sent");
    }

    free(context);
    
    delay(5000);
}
