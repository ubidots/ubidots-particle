# ubidots-particle
An Ubidots library for the Particle Core.

## A Quick Look...
A quick example of how to use the library follows:

To use the library you will need to initialize it with your personal token, and in the void loop you use the send_ubidots function, pay attention with this function the form to send datas is: number of variable, name of variable, value of variable, name of variable, value of variable... continue, the minimun quantity of variables is one. The next example is with 3 variables, follow this example to send data to Ubidots

```
# "Ubidots.h"

ubidots Ubidots("CCN8FrVulRYGulATkbaiR9Myx8qN2o"); // change this for your token

void setup() {
    Serial.begin(115200);                          // initialize serial of Particle
}
void loop() {

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


