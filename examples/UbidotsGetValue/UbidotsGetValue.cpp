// This example is to get the last value of variable from the Ubidots API

/****************************************
 * Include Libraries
 ****************************************/

#include <Ubidots.h>

/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "...."  // Put here your Ubidots TOKEN
#define VAR_ID "58d9153e762542576b721820"  // Put here your data source name
#define VAR_LABEL "...." // Put here your variable api label
#define DEVICE_LABEL "...." // Put here your device api label

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
    float value1;
    value1 = ubidots.getValue(VAR_ID);
    float value2;
    value2 = ubidots.getValueWithDatasource(DEVICE_LABEL, VAR_LABEL);
    if(value1!=NULL){
      Serial.println(value1);
    }
    if(value2!=NULL){
      Serial.println(value2);
    }
    delay(5000);
}
