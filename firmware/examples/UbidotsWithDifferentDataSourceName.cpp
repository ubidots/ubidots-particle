// This example is to save values with a setted data source name

#include "Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define DATA_SOURCE_NAME "Your_Data_Source_Tag"

Ubidots ubidots(TOKEN);

void setup() {	
	Serial.begin(115200);
	ubidots.setDatasourceName(DATA_SOURCE_NAME);
}
void loop() {
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    ubidots.add("Variable_name_one", value1);  // Change for your variable name
    ubidots.add("Variable_name_two", value2);
    ubidots.add("Variable_name_three", value3);
    ubidots.sendAll();
}