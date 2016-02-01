// This example is to save multiple variables to the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_token_here"  // Put here your Ubidots TOKEN

ubidots Ubidots(TOKEN);


void setup() {
    Serial.begin(115200);
}
void loop() {
	int quantityOfVariables = 3; // Put here the quantity of variables that you will send
	float value1 = analogRead(A0);
	float value2 = analogRead(A0);
	float value3 = analogRead(A0);    
    Ubidots.send_ubidots( quantityOfVariables, "variable_name_1",value1,"variable_name_2", value2, "variable_name_3", value3 );
    

}
