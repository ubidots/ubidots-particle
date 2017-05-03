// This example allow send data using UDP protocol

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


/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
    /* 
    TYPE_UDP is used to send data using UDP method
    if you don't call this function, the library automatically 
    set as TCP
    */
    ubidots.setMethod(TYPE_UDP); 
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.add("Variable_Name_Two", value2);
    ubidots.add("Variable_Name_Three", value3);
    if(ubidots.sendAll()){
        // Do something if values were sent properly
        Serial.println("Values sent by the device");
    }
    delay(5000);
}