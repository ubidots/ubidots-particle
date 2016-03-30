// This example is to get the last value of variable from the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_token_here"  // Put here your Ubidots TOKEN
#define DATASOURCENAME "Your_datasource_name"  // Put here your Ubidots datasource name 

Ubidots ubidots(TOKEN, DATASOURCENAME);

void setup() {
    Serial.begin(115200);
}
void loop() {    
    float value;
    value = ubidots.getValueWithDatasource(DATASOURCENAME, "Name_of_your_variable");
    ubidots.getUbidotsValue(ID);
}