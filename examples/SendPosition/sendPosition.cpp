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

    /* Coordinates */
    float latitude = 37.773;
    float longitude = -6.2345;

    /* Reserves memory to store context keys values, add as much as you need */
    char* str_lat = (char *) malloc(sizeof(char) * 10);
    char* str_lng = (char *) malloc(sizeof(char) * 10);

    /* Saves as char the coordinates */
    sprintf(str_lat, "%f", latitude);
    sprintf(str_lng, "%f", longitude);

    /* Reserves memory to store context array */
    char* context = (char *) malloc(sizeof(char) * 30);

    /* Adds context key-value pairs */
    ubidots.addContext("lat", str_lat);
    ubidots.addContext("lng", str_lng);

    /* Builds the context with the coordinates to send to Ubidots */
    ubidots.getContext(context);

    /* Sends the position */
    ubidots.add("position", value, context);  // Change for your variable name

    if (ubidots.send()) {  // Sends position to a device with label that matches the Particle Device Id
      Serial.println("Position sent");
    }

    /* frees memory */
    free(str_lat);
    free(str_lng);
    free(context);
    
    delay(5000);
}
