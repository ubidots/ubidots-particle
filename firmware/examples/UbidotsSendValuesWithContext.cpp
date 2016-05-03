// This example is to save multiple variables to the Ubidots API with TCP method

#include "Ubidots/Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    String context = "lat=1.2345$lng=1543.1234";  // To send latitude and longitude to Ubidots and see it in a map widget
    String context_2 = "Name_Of_Context=Value_Of_Context";  // The format of the context is like this, you must send it like this example
    // You can send multiple context in one variable, to send it you must add a "$" symbol between every context
    String context_3 = "Name_Of_Context=Value_Of_Context$Name_Of_Context_2=Value_Of_Context_2$Name_Of_Context_3=Value_Of_Context_3";
    ubidots.add("Variable_Name_One", value1, context);  // Change for your variable name
    ubidots.add("Variable_Name_Two", value2, context_2);
    ubidots.add("Variable_Name_Three", context_3);
    ubidots.sendAll();
    delay(5000);
}
