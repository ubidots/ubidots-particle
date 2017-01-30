# Particle

Here you will learn how to send multiple values to the Ubidots API, you just need the name and the value that you want to send. In addition you are able to get the last value from a variable of your Ubidots account.

## Requirements

* [Particle Photon, Electron, Core](https://store.particle.io/)
* Micro USB cable
* Internet connection
* Note: For the last version of the library is necessary to have your electron with firmware version 0.5.3 or above.

## Setup

1. Set up WiFi connection to the photon. There are two ways to do this:
    * [Using your smart phone](https://docs.particle.io/guide/getting-started/start/core/).
    * [Connecting it to your computer over USB](https://docs.particle.io/guide/getting-started/connect/core/).
2. After claiming your Photon and setting up your Ubidots account, let's go to [Particle's Web IDE](https://build.particle.io/build).
    * In the Particle's Web IDE create a new app and set the name.
    * Go to the library tab.
    * In contributed library write Ubidots and select the Ubidots library.
    * Click on **INCLUDE IN APP**. And return to "MYAPP.ino"

<aside class="warning">
This library create by default new Data Source. The name of this data source will be "Particle" by default, and his label will be you Particle Core ID.
</aside>
<aside class="notice">
The default method is UDP, if you want to change it go to the features sections and follow the example.
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

## Send values with custom different timestamp

To send a value with a custom timestamp to Ubidots, go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsSendValuesWithTimestamp.cpp**, copy it and paste to MYAPP.ino

```c++
****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"


/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "Your Ubidots TOKEN"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);


/****************************************
 * Auxiliar Functions
 ****************************************/

//

/****************************************
 * Main Functions
 ****************************************/


void setup() {
    Serial.begin(115200);
}


void loop() {
    float value1 = analogRead(A0);
    unsigned long t = ubidots.ntpUnixTime(); // calculates your actual timestamp in SECONDS

    ubidots.add("test-1", value1);  // Change the first argumento for your var's label
    ubidots.add("test-2", value1, NULL, t-20000);  // Sends a value with a custom timestamp
    ubidots.add("test-3", value1);

    // Sends variables 'test-1' and 'test-2' with your actual timestamp,
    // variable 'test-2' will be send with its custom timestamp
    ubidots.sendAll(t);
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

## Obtain timestamp

If you need to obtain the timestamp you can use the NTP server made by Francesco Potorti ported to this library, simply code something like this:

>Get timestamp

```c
unsigned long timestamp = ubidots.ntpUnixTime(); // calculates your actual timestamp in SECONDS

ubidots.add("test-1", 1);
ubidots.sendAll(timestamp);
```

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
#define DATA_SOURCE_TAG "Your_Data_Source_Tag"

Ubidots ubidots(TOKEN);

void setup() {  
    Serial.begin(115200);
    ubidots.setDatasourceTag(DATA_SOURCE_Tag);
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

### Change Data Source Name

>Set data source name Function

```c
ubidots.setDatasourceName(DATA_SOURCE_Name);
```

>Example using setDatasourceName Function

```c
```cpp
// This example is to save values with a setted data source name

#include "Ubidots/Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define DATA_SOURCE_NAME "Your_Data_Source_Name"

Ubidots ubidots(TOKEN);

void setup() {  
    Serial.begin(115200);
    ubidots.setDatasourceName(DATA_SOURCE_Name);
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

### Use TCP method to send values

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
    TYPE_TCP is used to send data using TCP method
    if you don't call this function, the library automatically 
    set as UDP
    */
    ubidots.setMethod(TYPE_TCP); 
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

Ubidots Particle library has a function to set transmission method of data, with this function you can change from UDP method to TCP method.
