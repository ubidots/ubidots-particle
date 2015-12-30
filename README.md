# ubidots-particle
An Ubidots library for the Particle Core.

## A Quick Look...
A quick example of how to use the library follows:

```
 #include "Ubidots.h"

ubidots Ubidots("token");

void setup() {
    Serial.begin(115200);
}
void loop() {
    Ubidots.send_ubidots( Number_of_variables, "Name_of_variable_1", Value_variable_1, "Name_of_variable_2", Value_variable_1, "Name_of_variable_3", Value_variable_1 );
}
```
Note: You can put more variables, but the first declaration (Number_of_varables) is the quantity of variables that you will send

## API Reference

### Ubidots()

```
ubidots Ubidots(char* token);

```
Initialize the library with your token


