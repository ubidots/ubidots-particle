// This example is to save multiple variables to the Ubidots API with TCP method

#include "Ubidots/Ubidots.h"


#define TOKEN "Your Ubidots TOKEN"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value1 = analogRead(A0);
    unsigned long t = ubidots.ntpUnixTime(); // calculates your actual timestamp sending a NTP packet
    char* timestamp = ubidots.timeToChar(t);
    ubidots.add("test", value1, NULL, timestamp);  // Change for your variable name
    ubidots.sendAll();
    memset(timestamp, 0, sizeof timestamp);
    delay(3000);
}