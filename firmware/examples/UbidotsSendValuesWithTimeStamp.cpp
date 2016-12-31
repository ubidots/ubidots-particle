// This example is to save multiple variables to the Ubidots API with TCP method

#include "Ubidots.h"


#define TOKEN "Your Ubidots TOKEN"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value1 = analogRead(A0);
    ubidots.add("test", value1, "", "1451518741000");  // Change for your variable name
    ubidots.sendAll();
    delay(5000);
}