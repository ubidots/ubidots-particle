# ubidots-particle
An Ubidots library for the Particle Core.

## A Quick Look...
A quick example of how to use the library follows:

```
#include "Ubidots.h"

ubidots Ubidots("CCN8FrVulRYGulATkbaiR9Myx8qN2o"); // change this for your token

void setup() {
    Serial.begin(115200);                          // initialize serial of Particle
}
void loop() {
                                                   // the first value 3, is the number of variables that you will send
                                                   // Temperature is the name of the first variable that you will send
                                                   // 2.3 is the value of the first variable that you will send
                                                   // Servo is the name of the second variable that you will send
                                                   // 3.2 is the value of the second variable that you will send
                                                   // Humedity is the name of the third variable that you will send
                                                   // 4.2 is the value of the third variable that you will send
    Ubidots.send_ubidots( 3, "Temperature",2.3,"Servo", 3.2, "Humedity", 4.2 ); 
}
```
Note: You can put more variables in send function, rember you put first the name of your variable, and next of this one you put the value of the variable

## API Reference

### Ubidots()

```
ubidots Ubidots(char* token);

```
Initialize the library with your token


