// This example is to get the last value of variable from the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define DATA_SOURCE_TAG "Your_Data_Source_Tag"  // Put here your data source name

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value;
    value = ubidots.getValueWithDatasource(DATA_SOURCE_TAG, "Variable_Name");
    Serial.println(value);
    delay(5000);
}