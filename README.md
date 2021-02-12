# Particle

Here you will learn how to send data to Ubidots using Particle Photon or Electron devices. You just need the variable label and value that you want to send along with an Ubidots valid TOKEN. In addition you're able to get the last value of a variable from your Ubidots account.

## Requirements

- [Particle Device](https://store.particle.io/)
- Micro USB cable
- Internet connection
- Note: For the last version of the library is necessary to have your particle device with firmware version 0.7.0 or above.

## Setup

1. Set up WiFi connection to the photon. There are two ways to do this:
   - [Using your smart phone](https://docs.particle.io/guide/getting-started/start/core/).
   - [Connecting it to your computer over USB](https://docs.particle.io/guide/getting-started/connect/core/).
2. After claiming your Photon and setting up your Ubidots account, let's go to [Particle's Web IDE](https://build.particle.io/build).
   - In the Particle's Web IDE create a new app and set the name.
   - Go to the library tab.
   - In contributed library write Ubidots and select the Ubidots library.
   - Click on **INCLUDE IN APP**. And return to "MYAPP.ino"

This library creates by default new Data Source. The name of this data source will be "Particle" by default, and his label will be you Particle ID.

The default method is TCP, if you want to change it go to the features sections and follow the example.

## Warning: About Particle Mesh

After the [announcement](https://blog.particle.io/mesh-deprecation/) of the deprecation of the Particle Mesh IoT protocol, as Particle is no longer producing Mesh units, Ubidots has decided to stop to support it out of the box in this library. If you need to still use the Mesh IoT protocol in your project in conjunction with our library, please compile your project through the Particle CLI with the version [v3.1.4](https://github.com/ubidots/ubidots-particle/releases/tag/3.1.4) of this library.

# Documentation

## Constructor

### Ubidots

```
Ubidots(char* token, UbiServer server, IotProtocol iot_protocol)
```

- @token, [Required]. Your Ubidots unique account [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account).  
- @server, [Optional], [Options] = [`UBI_INDUSTRIAL`], [Default] = `UBI_INDUSTRIAL`.  
- @iot_protocol, [Optional], [Options] = [`UBI_HTTP`, `UBI_TCP`, `UBI_UDP`, `UBI_PARTICLE`], [Default] = `UBI_TCP`. The IoT protocol that you will use to send or retrieve data.

Creates an Ubidots instance.

**NOTES**

- `UBI_PARTICLE` sends data using webhooks, so make sure to follow the instructions to set up your webhook properly [here](https://help.ubidots.com/connect-your-devices/connect-your-particle-device-to-ubidots-using-particle-webhooks).

## Methods

```
void add(char *variable_label, float value, char *context, unsigned long dot_timestamp_seconds, unsigned int dot_timestamp_millis)
```

- @variable_label, [Required]. The label of the variable where the dot will be stored.
- @value, [Required]. The value of the dot.  
- @context, [Optional]. The dot's context.  
- @dot_timestamp_seconds, [Optional]. The dot's timestamp in seconds.  
- @dot_timestamp_millis, [Optional]. The dot's timestamp number of milliseconds. If the timestamp's milliseconds values is not set, the seconds will be multplied by 1000.

Adds a dot with its related value, context and timestamp to be sent to a certain data source, once you use add().


**Important:** The max payload lenght is 700 bytes, if your payload is greater it won't be properly sent. You can see on your serial console the payload to send if you call the `setDebug(bool debug)` method and pass a true value to it.

```
float get(const char* device_label, const char* variable_label)
```

- @device_label, [Required]. The device label which contains the variable to retrieve values from.  
- @variable_label, [Required]. The variable label to retrieve values from.

Returns as float the last value of the dot from the variable.
IotProtocol getCloudProtocol()

```
std::map<int, float> tcpMap getMultipleValues(const char* deviceLabel, const char* variableLabels)
```

- @deviceLabel [Mandatory] pointer that stores the label of the device to retrieve values from.
- @variableLabels [Mandatory] comma separated variable labels to retrieve values from

Returns an ordered map list with the values retrieved for the provided variable labels. The values can be accesed by index, starting at position 0, and are mapped to every variable label set in the comma separated array.

```
void addContext(char *key_label, char *key_value)
```

- @key_label, [Required]. The key context label to store values.  
- @key_value, [Required]. The key pair value.

Adds to local memory a new key-value context key. The method inputs must be char pointers. The method allows to store up to 10 key-value pairs.

```
void getContext(char *context)
```

- @context, [Required]. A char pointer where the context will be stored.

Builds the context according to the chosen protocol and stores it in the context char pointer.

```
void setDebug(bool debug);;
```

- @debug, [Required]. Boolean type to turn off/on debug messages.

Make available debug messages through the serial port.

```
bool send(const char* device_label, const char* device_name, PublishFlags flags);
```

- @device_label, [Optional], [Default] = PARTICLE DEVICE ID. The device label to send data. If not set, the PARTICLE DEVICE ID will be used.  
- @device_name, [Optional], [Default] = @device_label. The device name otherwise assigned if the device doesn't already exist in your Ubidots account. If not set, the device label parameter will be used. **NOTE**: Device name is only supported through TCP/UDP, if you use another protocol, the device name will be the same as device label.  
- @flags, [Optional], [Options] = [`PUBLIC`, `PRIVATE`, `WITH_ACK`, `NO_ACK`]. Particle webhook flags.

Sends all the data added using the add() method. Returns true if the data was sent.

```
IotProtocol getCloudProtocol();
```

Retrieves the actual cloud protocol used to send data from the Mesh gateway.

# Examples

Refer to the examples folder
