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

This library create by default new Data Source. The name of this data source will be "**Particle**" by default, and his label will be you Particle Core ID.

The default method is UDP, if you want to change it go to the features sections and follow the example.

## Send values to Ubidots

You can send values to Ubidots using the variable label, also you can send values with timestamp and context.

### Send values using variable label

The following example is to send values to Ubidots, it will create the variable automatically with the label assign by you on the code. 

The library allows you send 5 values maximun. If you desire send more values just add this line ```ubidots.add("variable_name", value);``` to your code with the parameters needed.

Also, you can find the examples code on the **Particle IDE**, go to **Included Libraries** clic on **UBIDOTS** and select **UbidotsSendValues.cpp**, copy it and paste to **MYAPP.ino.**

Do not forget add your Ubidots TOKEN where indicated.

```c++
// This example is to get the last value of variable from the Ubidots API

// This example is to save multiple variables to the Ubidots API with TCP method

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"


/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "Your_Token"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);


/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

void setup() {
    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
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

### Send values with context

The following example is to send one value with context to Ubidots, it will create the variable automatically with the label assign by you on the code.

Also, you can find the examples code on the **Particle IDE**, go to **Included Libraries** clic on **UBIDOTS** and select **UbidotsSendValuesWithContext.cpp**, copy it and paste to **MYAPP.ino.**

Do not forget add your Ubidots TOKEN where indicated.

```c++
// This example is to save multiple variables with context to the Ubidots API with TCP method


/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"


/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "Your_Token"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);


/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/

void setup() {
    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    float value1 = analogRead(A0);
    char context[25];
    sprintf(context, "lat=1.2343$lng=132.1233"); //Sends latitude and longitude for watching position in a map
    ubidots.add("Variable_Name_One", value1, context);  // Change for your variable name
    ubidots.sendAll();
    delay(5000);
}
```


### Send values with timestamp

The following example is to send one value with timestamp to Ubidots, it will create the variable automatically with the label assign by you on the code.

Also, you can find the examples code on the **Particle IDE**, go to **Included Libraries** clic on **UBIDOTS** and select **UbidotsSendValuesWithTimeStamp.cpp**, copy it and paste to **MYAPP.ino.**

Do not forget add your Ubidots TOKEN where indicated.

```c++
/****************************************
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
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
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


## Get values from Ubidots

This library let get the last value of a variable from Ubidots using the variable label or variable ID, also you can get the context of it.

* To get the last value using the variable ID  you should use the following line -> ```getValue(VAR_ID)```

* To get the last value using the variable label you should use the following line -> ```getValueWithDatasource(DEVICE_LABEL, VAR_LABEL)```

The following example is to get the last value of a variable from Ubidots.

Do not forget add your Ubidots TOKEN where indicated, also the device and variable label or variable ID. 

```c++
// This example is to get the last value of variable from the Ubidots API

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "...."  // Put here your Ubidots TOKEN
#define VAR_ID "58d9153e762542576b721820"  // Put here your data source name
#define VAR_LABEL "...." // Put here your variable api label
#define DEVICE_LABEL "...." // Put here your device api label

/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/


Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    float value1;
    value1 = ubidots.getValue(VAR_ID);
    float value2;
    value2 = ubidots.getValueWithDatasource(DEVICE_LABEL, VAR_LABEL);
    if(value1!=NULL){
      Serial.println(value1);
    }
    if(value2!=NULL){
      Serial.println(value2);
    }
    delay(5000);
}
```

### getVarContext(VARIABLE_ID);

The following example is to get the context of a variable from Ubidots.

To get the context from a variable stored in ubidots , go to **Included Libraries** and clic on **UBIDOTS** and select **UbidotsGetContext.cpp**, copy it and paste to **MYAPP.ino**

Do not forget add your Ubidots TOKEN where indicated, also the variable ID. 

```c++
// This example is to get the last value of variable from the Ubidots API

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "...."  // Put here your Ubidots TOKEN
#define VAR_ID "58d9153e762542576b721820"  // Put here your data source name

/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/


Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    char* context;
    ubidots.getVarContext(VAR_ID);
    Serial.println(context);
    delay(5000);
}
```


## Special features

### Obtain timestamp

If you need to obtain the timestamp you can use the NTP server made by Francesco Potorti ported to this library, simply code something like this:

>Get timestamp

```c++
unsigned long timestamp = ubidots.ntpUnixTime(); // calculates your actual timestamp in SECONDS

ubidots.add("test-1", 1);
ubidots.sendAll(timestamp);
```

### Change Data Source Tag

>Set data source tag Function

```c++
ubidots.setDatasourceTag(DATA_SOURCE_Tag);
```

>Example using setDatasourceTag Function

```c++
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

```c++
ubidots.setDatasourceName(DATA_SOURCE_Name);
```

>Example using setDatasourceName Function

```c++
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

```c++
ubidots.setMethod(TYPE_UDP);
```

>Example using setMethod Function

```c++
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
