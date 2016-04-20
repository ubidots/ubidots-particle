// This example is to save multiple variables to the Ubidots API

#include "Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup() {	
	Serial.begin(115200);
	/* 
	TYPE_UDP for UDB method and TYPE_TCP for tcp method, 
	if you don't call this function, the library automatically 
	set as TCP
	*/
	ubidots.setMethod(TYPE_UDP); 
}
void loop() {
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    ubidots.add("Variable_name_one", value1);  // Change for your variable name
    ubidots.add("Variable_name_two", value2);
    ubidots.add("Variable_name_three", value3);
    ubidots.sendAll();
    delay(60000);  // Delay of one minute
}