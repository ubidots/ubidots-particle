# Particle

Here you will learn how to send multiple values to the Ubidots API, you just need the name and the value that you want to send. In addition you are able to get the last value from a variable of your Ubidots account.

## Requirements

* [Particle Photon](https://store.particle.io/)
* Micro USB cable
* Internet connection

## Setup

1. Set up WiFi connection to the photon. There are two ways to do this:
    * [Using your smart phone](https://docs.particle.io/guide/getting-started/start/core/).
    * [Connecting it to your computer over USB](https://docs.particle.io/guide/getting-started/connect/core/).
2. After claiming your Photon and setting up your Ubidots account, let's go to [Particle's Web IDE](https://build.particle.io/build).
    * In the Particle's Web IDE create a new app and set the name.
    * Go to the library tab.
    * In contributed library write Ubidots and select the Ubidots library.
    * Click on **INCLUDE IN APP**. And return to "MYAPP.ino"

<aside class="notice">
    While method #1 is faster, we like method #2 because it will install a Particle driver in your computer, which is very useful for firmware upgrades, creating webhooks or changing the owner of your Photon.
</aside>
<aside class="notice">
To use this library, put your Ubidots token and variable ID where indicated.
Upload the code, open the Serial monitor to check the results. If no response is seen, try reset your Particle Photon. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.
</aside>


## Send one value to Ubidots

To send a value to Ubidots, go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsSendValues.cpp**, copy it and paste to MYAPP.ino
Put your Ubidots token, in **quantityOfVariables** put "1" and set the name of your variable.

```c++
// This example is to save multiple variables to the Ubidots API

#include "Ubidots.h"


#define TOKEN "Your_token_here"  // Put here your Ubidots TOKEN
#define DATASOURCENAME "Your_datasource_name"  // Put here your Ubidots datasource name 
Ubidots ubidots(TOKEN, DATASOURCENAME);

void setup(){
    Serial.begin(115200);
}

void loop(){
    float value1 = analogRead(A0); 
    ubidots.add("Variable_name_one", value1);  // Change for your variable name
    ubidots.sendAll();
    delay(60000);  // Delay of one minute
}
```


## Get one value from Ubidots

To get the last value of a variable from Ubidots,  go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsGetValue.cpp**, copy it and paste to MYAPP.ino

```c++
// This example is to get the last value of variable from the Ubidots API

#include "Ubidots.h"

#define TOKEN "Your_token_here"  // Put here your Ubidots TOKEN
#define DATASOURCENAME "Your_datasource_name"  // Put here your Ubidots datasource name 

Ubidots ubidots(TOKEN, DATASOURCENAME);

void setup() {
    Serial.begin(115200);
}
void loop() {    
    float value;
    value = ubidots.getValueWithDatasource(DATASOURCENAME, "Name_of_your_variable");
    Serial.println(value);
    delay(60000);  // Delay of one minute
}

```

## Send multiple values to Ubidots 

To send a value to Ubidots, go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsSendValues.cpp**, copy it and paste to MYAPP.ino
Put your Ubidots token, in **quantityOfVariables** put the quantity of variables that you will send and set the name of yours variables.

```c++
// This example is to save multiple variables to the Ubidots API

#include "Ubidots.h"


#define TOKEN "Your_token_here"  // Put here your Ubidots TOKEN
#define DATASOURCENAME "Your_datasource_name"  // Put here your Ubidots datasource name 

Ubidots ubidots(TOKEN, DATASOURCENAME);

void setup(){
    Serial.begin(115200);
}

void loop(){
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2); 
    ubidots.add("Variable_name_one", value1);  // Change for your variable name
        ubidots.add("Variable_name_two", value2);
        ubidots.add("Variable_name_three", value3);
    ubidots.sendAll();
    delay(60000);  // Delay of one minute
}
```


