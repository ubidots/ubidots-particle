# ubidots-particle
An Ubidots library for the Particle Core.

## A Quick Look...
A quick example of how to use the library follows:

To use the library you will need:

1. Initialize the library with your personal token [how to get your personal token](http://ubidots.com/docs/get_started/quickstart/tutorial.html).
2. Put in the void loop the function send_ubidots
3. Send the quantity of variables,  variable_name_1, variable_value_1, variable_name_2, variable_value_2 ... 

Pay attention to the next example, in this example we send 3 variables:

``` cpp
# "Ubidots.h"

ubidots Ubidots("CCN8FrVulRYGulATkbaiR9Myx8qN2o"); // change this for your token

void setup() {
    Serial.begin(115200);   // initialize serial of Particle
}
void loop() {

    Ubidots.send_ubidots( 3, "Temperature",2.3,"Servo", 3.2, "Humedity", 4.2 ); 
}
```
Note: You can put more variables in send function, rember you put first the name of your variable, and next of this one you put the value of the variable




