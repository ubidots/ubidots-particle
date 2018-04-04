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


This library creates by default new Data Source. The name of this data source will be "Particle" by default, and his label will be you Particle Core ID.

The default method is UDP, if you want to change it go to the features sections and follow the example.

# Documentation

## Constructor

### Ubidots

```
Ubidots(char* token)
```
> Creates an Ubidots instance, you must setup as input your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account).

## Methods

```
add(char* variableLabel, float value, char *context, char *timestamp);
```
> Add a variable with a value, context and timestamp to be sent to a certain data source, once you use add() you can publish your variable using the ubidotsPublish() method. You can add 5 variables maximum before of publish them. 

**Important:** The max payload lenght is 700 bytes, if your payload is greater it will not be properly sent. You can see on your serial console the payload to send if you call the ```setDebug(bool debug)``` method and pass a true value to it.

```
getValue(char *id);
```
> Returns as float the last value of the variable with the ID passed as argument. The value is retrieved through TCP.

```
getValueWithDatasource(char* device, char* variable);
```
> Returns as float the last value of the variable with the label specified as argument that belongs to the device label specified also as argument.

```
getValueHTTP(char* id);
```
> Returns as float the last value of the variable with the ID passed as argument. The value is retrieved through HTTP.

```
getVarContext(char* id);
```
> Returns as char array the last value of the variable with the ID passed as argument. The context is retrieved through TCP.

```
isDirty();
```
> Returns true if the array used by the add() method for sending data is not empty.

```
setDebug(bool debug);;
```

> Make available debug messages through the serial port.

```
sendAll(unsigned long timestamp_global);
```
> Sends all the data added using the add() method. Global timestamp is optional, if added, it must be in seconds and ALL the variables will be sent with be stored with that timestamp.

```
setDeviceName(char* deviceName);
```
> Sets the device name to be created or updated.

```
setDeviceLabel(char* deviceLabel);
```
> Sets the device label to be created or updated.

```
setMethod(uint8_t method);
```
> Sets the method for sending data:
> * TYPE_UDP : set this for sending data through UDP, this is the default one.
> * TYPE_TCP : set this for sending data through TCP

```
setTimeout(int timeout);
```

> Sets the max timeout (in milliseconds) to wait for an answer from the serve. 5000 milliseconds are set as default.

# Examples

## Send values to Ubidots

You can send values to Ubidots using the [**variable label**](http://help.ubidots.com/faqs-and-troubleshooting/automatically-provision-devices-and-variables-with-ubidots-api-labels), also you can send values with **timestamp** and **context**

### Send values using variable label

The following example is to send values to Ubidots, it will create the variable automatically with the label assign by you on the code.

The library allows you send 5 values maximum. If you desire send more values just add this line `ubidots.add("variable_name", value)`; to your code with the parameters needed.

Also, you can find the examples code on the **Particle IDE**, go to **Included Libraries** click on **UBIDOTS** and select **UbidotsSendValues.cpp**, copy it and paste to **MYAPP.ino**.

Do not forget add your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account) where indicated.

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

The following example is to send one value with context to Ubidots, it will create the variable automatically with the [**label**](http://help.ubidots.com/faqs-and-troubleshooting/automatically-provision-devices-and-variables-with-ubidots-api-labels) assign by you on the code.

Also, you can find the examples code on the **Particle IDE**, go to **Included Libraries** click on **UBIDOTS** and select **UbidotsSendValuesWithContext.cpp**, copy it and paste to **MYAPP.ino**.

Do not forget add your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account) where indicated.

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
    if(ubidots.sendAll()){
        // Do something if values were sent properly
        Serial.println("Values sent by the device");
    }
    delay(5000);
}
```


### Send values with timestamp

The following example is to send one value with timestamp to Ubidots, it will create the variable automatically with the [**label**](http://help.ubidots.com/faqs-and-troubleshooting/automatically-provision-devices-and-variables-with-ubidots-api-labels) assign by you on the code.

Also, you can find the examples code on the **Particle IDE**, go to **Included Libraries** click on **UBIDOTS** and select **UbidotsSendValuesWithTimeStamp.cpp**, copy it and paste to **MYAPP.ino**.

Do not forget add your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account) where indicated.

```c++
/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"

/****************************************
 * Define Constants
 ****************************************/

#ifndef TOKEN
#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#endif

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
  unsigned long t = Time.now(); // calculates your actual timestamp in SECONDS

  ubidots.add("test-1", value1);  // Change the first argument for your var's label
  ubidots.add("test-2", value1, NULL, t-3600);  // Set custom timestamp, -3600 seconds (1hr)
  ubidots.add("test-3", value1);  // Default timestamp applied at SEND time

  bool bufferSent = false;

  if(ubidots.isDirty()){  // There are stored values in buffer
    // Variables 'test-1' and 'test-3' will be sent with time-sent timestamp
    // Variable 'test-2' will be sent with previously applied custom timestamp
    bufferSent = ubidots.sendAll();
  }

  if(bufferSent){
      // Do something if values were sent properly
      Serial.println("Values sent by the device");
  }

  delay(5000);
}
```

## Get values from Ubidots

This library let get the last value of a variable from Ubidots using the [**variable label**](http://help.ubidots.com/faqs-and-troubleshooting/automatically-provision-devices-and-variables-with-ubidots-api-labels) or [**variable ID**](http://help.ubidots.com/faqs-and-troubleshooting/where-can-i-find-the-variable-id), also you can get the context of it.

* To get the last value using the variable ID you should use the following line:

> getValue(VAR_ID) 

* To get the last value using the variable label you should use the following line:

> getValueWithDatasource(DEVICE_LABEL, VAR_LABEL) 

The following example is to get the last value of a variable from Ubidots.

Do not forget add your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account) where indicated, also the device and **variable label** or **variable ID**.

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

### Get variable context

The following example is to get the context of a variable from Ubidots.


To get the context from a variable stored in ubidots , go to **Included Libraries** and click on **UBIDOTS** and select **UbidotsGetContext.cpp**, copy it and paste to **MYAPP.ino**

Do not forget add your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account) where indicated, also the [variable ID](http://help.ubidots.com/faqs-and-troubleshooting/where-can-i-find-the-variable-id).

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

## Obtain timestamp

If you need to obtain the timestamp you can use the NTP server made by Francesco Potorti ported to this library, simply code something like this:

>Get timestamp

```c
unsigned long timestamp = Time.now(); // calculates your actual timestamp in SECONDS

ubidots.add("test-1", 1);
ubidots.sendAll(timestamp);
```

### Change Device Label

>Sets the device label

```c
ubidots.setDeviceLabel(char* deviceLabel);
```

>Example using setDeviceLabel method

```c
```cpp
// This example is to save values with a fixed device label

#include "Ubidots/Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define DEVICE_NAME "Your_Device_Name"

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
    ubidots.setDeviceLabel(DEVICE_LABEL);
    if(ubidots.sendAll()){
        //Do something if values were sent
        Serial.println("values sent");
    }
    delay(5000);
}
```

### Change Device Name

>Sets the device name to display in Ubidots

```c
ubidots.setDeviceName(char* deviceName);
```

>Example using setDatasourceName Function

```c
```cpp
// This example is to save values with a fixed device name

#include "Ubidots/Ubidots.h"


#define TOKEN "Your_Token_Here"  // Put here your Ubidots TOKEN
#define DEVICE_NAME "Your_Device_Name"

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
    ubidots.setDeviceName(DEVICE_NAME);
    if(ubidots.sendAll()){
        //Do something if values were sent
        Serial.println("data sent");
    }
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