# Particle

Here you will learn how to send multiple values to the Ubidots API, you just need the name and the value that you want to send. In addition you are able to get the last value from a variable of your Ubidots account.

## Requirements

* [Particle Photon, Electron, Core](https://store.particle.io/)
* Micro USB cable
* Internet connection
* Note: For the last version of the library is necessary to have your particle device with firmware version 0.7.0 or above.

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

The default method is TCP, if you want to change it go to the features sections and follow the example.

# Documentation

## Constructor

### Ubidots

```
Ubidots(char* token, UbiServer server, IotProtocol iot_protocol)
```
> @token, [Mandatory]. Your Ubidots unique account [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account).  
@server, [Optional], [Options] = [`UBI_INDUSTRIAL`, `UBI_EDUCATIONAL`], [Default] = `UBI_INDUSTRIAL`. The server to send data, set `UBI_EDUCATIONAL` if your account is educational type.  
@iot_protocol, [Optional], [Options] = [`UBI_HTTP`, `UBI_TCP`, `UBI_UDP`, `UBI_PARTICLE`], [Default] = `UBI_TCP`. The IoT protocol that you will use to send or retrieve data. UBI_PARTICLE sends data using webhooks, so make sure to follow the instructions to set up your webhook properly [here](https://help.ubidots.com/connect-your-devices/connect-your-particle-device-to-ubidots-using-particle-webhooks).  

Creates an Ubidots instance.

## Methods

```
void add(char *variable_label, float value, char *context, unsigned long dot_timestamp_seconds, unsigned int dot_timestamp_millis)
```
> @variable_label, [Mandatory]. The label of the variable where the dot will be stored.
@value, [Mandatory]. The value of the dot.  
@context, [Optional]. The dots' context.  
@dot_timestamp_seconds, [Optional]. The dots' timestamp in seconds. If the timestamp's milliseconds values is not set, the seconds will be multplied by 1000.  
@dot_timestamp_millis, [Optional]. The dots' timestamp number of milliseconds.  

Adds a dot with its related value, context and timestamp to be sent to a certain data source, once you use add(). You can add up to 10 dots before to publish them. 

**Important:** The max payload lenght is 700 bytes, if your payload is greater it will not be properly sent. You can see on your serial console the payload to send if you call the ```setDebug(bool debug)``` method and pass a true value to it.

```
float get(const char* device_label, const char* variable_label)
```
> @device_label, [Mandatory]. The device label where the variable to retrieve values is.  
@variable_label, [Mandatory]. The variable label to retrieve values.  

Returns as float the last value of the dot from the variable.

```
void addContext(char *key_label, char *key_value)
```
> @key_label, [Mandatory]. The key context label to store values.  
@key_value, [Mandatory]. The key pair value.  

Adds to local memory a new key-value context key. The method inputs must be char pointers. The method allows to store up to 10 key-value pairs.

```
void getContext(char *context)
```
> @context, [Mandatory]. A char pointer where the context will be stored.  

Builds the context according to the chosen protocol and stores it in the context char pointer.

```
void setDebug(bool debug);;
```

> @debug, [Mandatory]. Boolean type to turn off/on debug messages.

Make available debug messages through the serial port.

```
bool send(const char* device_label, const char* device_name, PublishFlags flags);
```
> @device_label, [Optional]. The device label to send data. If not set, the Particle device Id will be used.  
@device_name, [Optional]. The device name that will be created if the device does not exist in your Ubidots account. If not set, the device_labe input parameter will be used. NOTE: Device name are supported right now only through UDP/TCP, if you use another method, the device name will be the same already setup as device label.  
@flags, [Optional], [Options] = [`PUBLIC`, `PRIVATE`, `WITH_ACK`, `NO_ACK`]. Particle webhook flags.  

Sends all the data added using the add() method. Returns true if the data was sent.

# Examples

Refer to the examples folder