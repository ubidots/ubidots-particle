// This example is to get the last value of variable from the Ubidots API

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "...."  // Put here your Ubidots TOKEN
#endif
#ifndef VAR_ID
#define VAR_ID "58d9153e762542576b721820"  // Put here your data source name
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
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    char* context;
    context = ubidots.getVarContext(VAR_ID);
    if(context!=NULL){
    	// Do something if context is obtained properly
    	Serial.println(context);
    }
    delay(5000);
}
