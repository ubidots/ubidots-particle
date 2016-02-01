// This example is to get a value of variable from the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_token_here"
#define ID "Your_ID_here"

Ubidots ubidots(TOKEN);


void setup() {
    Serial.begin(115200);
}
void loop() {    
    ubidots.getUbidotsValue(ID);
}
