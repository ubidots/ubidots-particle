// This example is to save multiple variables with context to the Ubidots API with TCP method


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

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

void setup() {
    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    float value1 = analogRead(A0);
    char context[25];
    sprintf(context, "lat=1.2343$lng=132.1233"); //Sends latitude and longitude for watching position in a map
    ubidots.add("Variable_Name_One", value1, context);  // Change for your variable name
    if(ubidots.sendAll()){
        // Do something if values were sent properly
        Serial.println("Values sent by the device");
    }
    delay(5000);
}
