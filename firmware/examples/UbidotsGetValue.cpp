// This example is to get the last value of variable from the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_token_here"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value;
    value = ubidots.getValueWithDatasource(DATASOURCE_NAME, "Name_of_your_variable");
    Serial.println(value);
    Delay(60000);  // Delay of one minute
}
