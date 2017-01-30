// This example is to get the last value of variable from the Ubidots API

// This example is to save multiple variables to the Ubidots API with TCP method

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"


/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "Your_Token"  // Put here your Ubidots TOKEN

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
}

void loop() {
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.add("Variable_Name_Two", value2);
    ubidots.add("Variable_Name_Three", value3);
    ubidots.sendAll();
    delay(5000);
}