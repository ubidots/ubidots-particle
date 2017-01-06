// This example is to save multiple variables to the Ubidots API with TCP method

#include "Ubidots/Ubidots.h"


#define TOKEN "Your Ubidots TOKEN"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value1 = analogRead(A0);
    long t = 1451518741L;
    char* timestamp = (char *) malloc(sizeof(char) * 700);
    sprintf(timestamp, "%d%s", t, "000");
    Serial.print("timestamp:");
    Serial.println(t);
    ubidots.add("test", value1, NULL, timestamp);  // Change for your variable name
    ubidots.sendAll();
    timestamp = 0L;
    delay(10000);
}