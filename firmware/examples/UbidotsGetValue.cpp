// This example is to get the last value of variable from the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_token_here"  // Put here your Ubidots TOKEN
#define DATA_SOURCE_NAME "Your_Data_Source_Name"  // Put here your data source name

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value;
    value = ubidots.getValueWithDatasource(DATA_SOURCE_NAME, "Name_of_your_variable");
    Serial.println(value);
}