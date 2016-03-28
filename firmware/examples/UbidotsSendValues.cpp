// This example is to save multiple variables to the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_token_here"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup(){
	Serial.begin(115200);
}

void loop(){
	int var_count = 3; // Put here the amount of variables you will send
	float value1 = analogRead(A0);
	float value2 = analogRead(A1);
	float value3 = analogRead(A2);
	ubidots.sendUbidots(var_count, "variable_name_1", value1, "variable_name_2", value2, "variable_name_3", value3);
}
