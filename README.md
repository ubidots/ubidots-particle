# ubidots-particle
A Ubidots library for the Particle Core.

## A Quick Look...

To use the library you will need:

1. Initialize the library with your personal token [how to get your personal token](http://ubidots.com/docs/get_started/quickstart/tutorial.html).
2. Put in the void loop the function send_ubidots
3. Send the quantity of variables,  variable_name_1, variable_value_1, variable_name_2, variable_value_2 ... 

Here's a quick example of how to use the library to send 3 values:

``` cpp
# "Ubidots.h"

Ubidots ubidots("CCN8FrVulGRBulATkbaiR9Myx8qN2o"); // change this with your token

void setup() {
    Serial.begin(115200);   // initialize serial of Particle
}
void loop() {

    ubidots.sendUbidots( 3, "Temperature",2.3,"Servo", 3.2, "Humedity", 4.2 ); 
}
```
You can put more variables in the send function following the order of (..."variable_name", "value", "variable_name", "value").




