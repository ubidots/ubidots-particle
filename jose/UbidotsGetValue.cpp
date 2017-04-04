// This example is to get the last value of variable from the Ubidots API

#include "Ubidots.h"

#define TOKEN "p4uuT2OIIFJwv7ncTVfoVqcfImwRQW"  // Put here your Ubidots TOKEN
#define VAR_ID "58d9153e762542576b721820"  // Put here your data source name
#define VAR_LABEL "nivel-liquido-h"
#define DEVICE_LABEL "vemli"

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