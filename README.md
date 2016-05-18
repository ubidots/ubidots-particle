# Particle

Here you will learn how to send multiple values to the Ubidots API, you just need the name and the value that you want to send. In addition you are able to get the last value from a variable of your Ubidots account.

## Requirements

* [Particle Photon, Electron, Core](https://store.particle.io/)
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
To use this library, put your Ubidots token, datasource name and variable name where indicated.
Upload the code, open the Serial monitor to check the results. If no response is seen, try reset your Particle Photon. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.
</aside>

<aside class="warning">
You only could change the datasource name in the first iteration of code or in the Ubidots API, this code will put "Particle" in your datasource name and your particle core ID in the Tag ID of datasource by default.
</aside>


## Send one value to Ubidots

To send a value to Ubidots, go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsSendValues.cpp**, copy it and paste to MYAPP.ino.

```c++
// This example is to save a value to the Ubidots API with TCP method

#include "Ubidots/Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value1 = analogRead(A0);
    ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.sendAll();
    delay(5000);
}
```


## Get one value from Ubidots

To get the last value of a variable from Ubidots,  go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsGetValue.cpp**, copy it and paste to MYAPP.ino

```c++
// This example is to get the last value of variable from the Ubidots API

#include "Ubidots/Ubidots.h"

#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define DATA_SOURCE_TAG "Your_Data_Source_Tag"  // Put here your data source name

Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value;
    value = ubidots.getValueWithDatasource(DATA_SOURCE_TAG, "Variable_Name");
    Serial.println(value);
    delay(5000);
}
```

## Send multiple values to Ubidots 

To send a value to Ubidots, go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsSendValues.cpp**, copy it and paste to MYAPP.ino

```c++
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
    ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.add("Variable_Name_Two", value2);
    ubidots.add("Variable_Name_Three", value3);
    ubidots.sendAll();
    delay(5000);
}
```

## Send multiple values with context

To send values with context you can follow our example in the library, select ***UbidotsSendValuesWithContext.cpp** or copy this code

```cpp
// This example is to save multiple variables with context to the Ubidots API with TCP method

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
    char context[25];
    char context_2[25];
    char context_3[45];
    sprintf(context, "lat=1.2343$lng=132.1233");
    // To send latitude and longitude to Ubidots and see it in a map widget
    sprintf(context_2, "Name_Of_Context=Value_Of_Context");
    // The format of the context is like this, you must send it like this example
    sprintf(context_3, "Name_Of_Context=Value_Of_Context$Name_Of_Context_2=Value_Of_Context_2$Name_Of_Context_3=Value_Of_Context_3");
    // You can send multiple context in one variable, to send it you must add a "$" symbol between every context
    ubidots.add("Variable_Name_One", value1, context);  // Change for your variable name
    ubidots.add("Variable_Name_Two", value2, context_2);
    ubidots.add("Variable_Name_Three", value3,  context_3);
    ubidots.sendAll();
    delay(5000);
}
```

## Special features

### Change Data Source Tag

>Set data source tag Function

```c
ubidots.setDatasourceTag(DATA_SOURCE_Tag);
```

>Example using setDatasourceTag Function

```c
```cpp
// This example is to save values with a setted data source name

#include "Ubidots/Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define DATA_SOURCE_Tag "Your_Data_Source_Tag"

Ubidots ubidots(TOKEN);

void setup() {  
    Serial.begin(115200);
    ubidots.setDataSourceTag(DATA_SOURCE_Tag);
}
void loop() {
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.add("Variable_Name_Two", value2);
    ubidots.add("Variable_Name_Three", value3);
    ubidots.sendAll();
    delay(5000);
}
```
Ubidots Particle library has a special function to set a Data Source Tag. This Data Source tag is where the values will be saves in Ubidots. 
<aside class="warning">With your Particle device you can't change the tag of created data source. To do that you will do it from front end of Ubidots</aside>
### Change Data Source Name

>Set data source name Function

```c
ubidots.setDataSourceName(DATA_SOURCE_Name);
```

>Example using setDataSourceName Function

```c
```cpp
// This example is to save values with a setted data source name

#include "Ubidots/Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define DATA_SOURCE_Name "Your_Data_Source_Name"

Ubidots ubidots(TOKEN);

void setup() {  
    Serial.begin(115200);
    ubidots.setDataSourceName(DATA_SOURCE_Name);
}
void loop() {
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.add("Variable_Name_Two", value2);
    ubidots.add("Variable_Name_Three", value3);
    ubidots.sendAll();
    delay(5000);
}
```
Ubidots Particle library has a special function to set a Data Source name. This Data Source name is where the values will be saves in Ubidots. 
<aside class="warning">With your Particle device you can't change the name of created data source. To do that you will do it from front end of Ubidots</aside>

### Use UDP method to send values
>Set transmission function

```c
ubidots.setMethod(TYPE_UDP);
```

>Example using setMethod Function

```c
// This example allow send data using UDP protocol

#include "Ubidots/Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

void setup() {  
    Serial.begin(115200);
    /* 
    TYPE_UDP is used to send data using UDP method
    if you don't call this function, the library automatically 
    set as TCP
    */
    ubidots.setMethod(TYPE_UDP); 
}
void loop() {
    float value1 = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.add("Variable_Name_Two", value2);
    ubidots.add("Variable_Name_Three", value3);
    ubidots.sendAll();
    delay(5000);
}
```

Ubidots Particle library has a function to set transmission method of data, with this function you can change from TCP method to UDP method.

### Change TCP Server

>Example to change TCP Server

```c
// This example is to save a value to the Ubidots API with TCP method

#include "Ubidots/Ubidots.h"

#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define NEW_SERVER "Your_Server.com"

Ubidots ubidots(TOKEN, NEW_SERVER);

void setup() {
    Serial.begin(115200);
}
void loop() {
    float value1 = analogRead(A0);
    ubidots.add("Variable_Name_One", value1);  // Change for your variable name
    ubidots.sendAll();
    delay(5000);
}
```

With Ubidots Particle library you can change the server where you will save the data, default server is our server.

