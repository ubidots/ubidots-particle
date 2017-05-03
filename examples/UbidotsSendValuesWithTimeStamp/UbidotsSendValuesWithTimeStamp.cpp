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

//

/****************************************
 * Main Functions
 ****************************************/


void setup() {
    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}


void loop() {
    float value1 = analogRead(A0);
    unsigned long t = ubidots.ntpUnixTime(); // calculates your actual timestamp in SECONDS

    ubidots.add("test-1", value1);  // Change the first argumento for your var's label
    ubidots.add("test-2", value1, NULL, t-20000);  // Sends a value with a custom timestamp
    ubidots.add("test-3", value1);

    // Sends variables 'test-1' and 'test-2' with your actual timestamp,
    // variable 'test-2' will be send with its custom timestamp
    if(ubidots.sendAll()){
        // Do something if values were sent properly
        Serial.println("Values sent by the device");
    }
    delay(5000);
}
