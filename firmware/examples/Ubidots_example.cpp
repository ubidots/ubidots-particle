
#include "Ubidots.h"

ubidots Ubidots("CCN8FrVulRYGulPTkbaiR9Myx8qN2o");


void setup() {
    Serial.begin(115200);
}
void loop() {
    #include "application.h"
#include "Ubidots.h"

#define TOKEN "YOUR_TOKEN_OF_UBIDOTS_HERE"

#define ID "YOUR_ID_HERE" 

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
    
}
void loop() {
    int quantityOfVariables
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    // First arg is to send the quantity of variables, and the next arg is the name of the first
    // variable between first variable value
    ubidots.send_ubidots( quantityOfVariables, "Temperatura", value1,"Servomotor", value2, "Humedad", value3 );
    float n = ubidots.get_ubidots(ID);
    Serial.println(n);
    delay(300);
}
