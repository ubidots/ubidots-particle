// This example is to save multiple variables to the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_token_here"

ubidots Ubidots(TOKEN);


void setup() {
    Serial.begin(115200);
}
void loop() {    
    Ubidots.send_ubidots( 3, "Temperatura",2.3,"Servomotor", 3.2, "Humedad", 4.2 );
    

}
