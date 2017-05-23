/*
Copyright (c) 2013-2016 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Original made by Mateo Velez - Metavix for Ubidots Inc
Modified and maintained by Jose Garcia for Ubidots Inc

*/

#include "Ubidots.h"
#include "inet_hal.h"

/***************************************************************************
CONSTRUCTOR
***************************************************************************/

/**
 * Constructor.
 * Default method is UDP
 * Default dsNmae is Particle
 */

Ubidots::Ubidots(char* token, char* server) {
    _token = token;
    _server = server;
    _method = TYPE_UDP;
    _dsName = "particle";
    _currentValue = 0;
    val = (Value *)malloc(MAX_VALUES * sizeof(Value));
    String str = System.deviceID();
    _pId = new char[str.length() + 1];
    strcpy(_pId, str.c_str());
}


/***************************************************************************
FUNCTIONS TO RETRIEVE DATA
***************************************************************************/


/**
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API, if any error occurs
    the function returns ERROR_VALUE
 */

float Ubidots::getValue(char* id) {

    String response = "";
    int timeout = 0;
    uint8_t max_retries = 0;
    float num;
    char* data = (char *) malloc(sizeof(char) * 200);
    sprintf(data, "%s/%s|GET|%s|%s|end", USER_AGENT, VERSION, _token, id);

    Spark.process(); // Cleans previous processes
    _client.connect(SERVER, PORT); // Initial connection

    while (!_client.connected()) {
        if (_debug) {
            Serial.println("Attemping to connect");
        }
        _client.connect(SERVER, PORT);
        max_retries++;
        if (max_retries > 5) {
            if (_debug) {
                Serial.println("Could not connect to server");
            }
            free(data);
            return ERROR_VALUE;
        }
        delay(5000);
    }

    if (_debug) {
        Serial.println(F("Getting your variable with request: "));
        Serial.println(F(data));
    }

    _client.print(data);

    while (!_client.available() && timeout < 2000) {
        timeout++;
        delay(1);
        if (timeout >= 2000) {
            if (_debug) {
                Serial.println(F("Error, max timeout reached"));
            }
            _client.stop();
            delay(5);
            free(data);
            return ERROR_VALUE;
        }
    }

    while (_client.available()) {
        char c = _client.read();
        if (c == -1) {
            if (_debug) {
                Serial.println(F("Error reading data from server"));
            }
            _client.stop();
            delay(5);
            free(data);
            return ERROR_VALUE;
        }
        response += c;
        delay(10);
    }

    if (_debug) {
        Serial.println(F("response:"));
        Serial.println(response);
    }

    uint8_t value_init = 3 + response.indexOf("OK|");
    if (value_init != 3) {
        if (_debug) {
            Serial.println("wrong answer returned by the server");
        }
        return ERROR_VALUE;
    }

    response = response.substring(value_init);
    if (_debug) {
        Serial.println("Value obtained:");
        Serial.println(response);
    }

    num = response.toFloat();
    free(data);
    _client.stop();
    delay(5);
    return num;
}

/**
 * This function is to get value from the Ubidots API with the data source label
 * and variable label
 * @arg dsTag is the label of the device
 * @arg idName is the label of the variable
 * @return num the data that you get from the Ubidots API, if any error occurs
    the function returns ERROR_VALUE
 */

float Ubidots::getValueWithDatasource(char* device, char* variable) {

    String response = "";
    int timeout = 0;
    uint8_t max_retries = 0;
    float num;
    char* data = (char *) malloc(sizeof(char) * 300);
    sprintf(data, "%s/%s|LV|%s|%s:%s|end", USER_AGENT, VERSION, _token, device, variable);

    Spark.process(); // Cleans previous processes
    _client.connect(SERVER, PORT); // Initial connection

    while (!_client.connected()) {
        if (_debug) {
            Serial.println("Attemping to connect");
        }
        _client.connect(SERVER, PORT);
        max_retries++;
        if (max_retries > 5) {
            if (_debug) {
                Serial.println("Could not connect to server");
            }
            free(data);
            return ERROR_VALUE;
        }
        delay(5000);
    }

    if (_debug) {
        Serial.println(F("Getting your variable with request: "));
        Serial.println(F(data));
    }

    _client.print(data);

    while (!_client.available() && timeout < 2000) {
        timeout++;
        delay(1);
        if (timeout >= 2000) {
            if (_debug) {
                Serial.println(F("Error, max timeout reached"));
            }
            free(data);
            return ERROR_VALUE;
        }
    }

    while (_client.available()) {
        char c = _client.read();
        if (c == -1) {
            if (_debug) {
                Serial.println(F("Error reading from server"));
            }
            _client.stop();
            delay(5);
            free(data);
            return ERROR_VALUE;
        }
        response += c;
        delay(10);
    }

    if (_debug) {
        Serial.println(F("response:"));
        Serial.println(response);
    }

    uint8_t value_init = 3 + response.indexOf("OK|");
    if (value_init != 3) {
        if (_debug) {
            Serial.println("Error reading values from server");
        }
        free(data);
        _client.stop();
        delay(5);
        return ERROR_VALUE;
    }

    response = response.substring(value_init);
    if (_debug) {
        Serial.println("Value obtained:");
        Serial.println(response);
    }

    num = response.toFloat();
    free(data);
    _client.stop();
    delay(5);
    return num;
}


/**
 * This function obtains the variable from a variable in Ubidots using HTTP requests format
 * @arg id is the Variable ID
 * @return value as float, if any error is raised retuns ERROR_VALUE
 */

float Ubidots::getValueHTTP(char* id) {

    String response = "";
    int timeout = 0;
    uint8_t max_retries = 0;
    char* data = (char *) malloc(sizeof(char) * 300);

    sprintf(data, "GET /api/v1.6/variables/%s", id);
    sprintf(data, "%s/values?page_size=1 HTTP/1.1\r\n", data);
    sprintf(data, "%sHost: things.ubidots.com\r\nUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
    sprintf(data, "%sX-Auth-Token: %s\r\nConnection: close\r\n\r\n", data, _token);

    Spark.process(); // Cleans previous processes
    _client.connect(SERVERHTTP, PORTHTTP); // Initial connection

    while (!_client.connected()) {
        if (_debug) {
            Serial.println("Attemping to connect");
        }
        _client.connect(SERVERHTTP, PORTHTTP);
        max_retries++;
        if (max_retries > 5) {
            if (_debug) {
                Serial.println("Could not connect to server");
            }
            free(data);
            return ERROR_VALUE;
        }
        delay(5000);
    }

    if (_debug) {
        Serial.println(F("Getting your variable with request: "));
        Serial.println(F(data));
    }

    _client.print(data);

    while (!_client.available() && timeout < 2000) {
        timeout++;
        delay(1);
        if (timeout >= 2000) {
            if (_debug) {
                Serial.println(F("Error, max timeout reached"));
            }
            free(data);
            return ERROR_VALUE;
        }
    }

    while (_client.available()) {
        char c = _client.read();
        if (c == -1) {
            if (_debug) {
                Serial.println(F("Error reading from server"));
            }
            _client.stop();
            delay(5);
            free(data);
            return ERROR_VALUE;
        }
        response += c;
        delay(10);
    }

    if (_debug) {
        Serial.println(F("response:"));
        Serial.println(response);
    }

    uint8_t bodyPosinit = 4 + response.indexOf("\r\n\r\n");
    response = response.substring(bodyPosinit);

    int value_init = 9 + response.indexOf("\"value\":"); // position of "value" + one because of the space between "value": {value}
    int value_end = response.indexOf(", \"timestamp\":");

    if (value_end < value_init) {
        if (_debug) {
            Serial.println("Error reading values from server");
        }
        _client.stop();
        free(data);
        return ERROR_VALUE;
    }

    response = response.substring(value_init, value_end);
    if (_debug) {
        Serial.println("Value obtained:");
        Serial.println(response);
    }

    float num = response.toFloat();
    free(data);
    _client.stop();
    delay(5);
    return num;
}


/**
 * This function obtains the context from a variable in Ubidots
 * @arg id is the Variable ID
 * @return Context as char array
 */

char* Ubidots::getVarContext(char* id) {
    String response = "";
    int timeout = 0;
    uint8_t max_retries = 0;
    char* data = (char *) malloc(sizeof(char) * 300);

    sprintf(data, "GET /api/v1.6/variables/%s", id);
    sprintf(data, "%s/values?page_size=1 HTTP/1.1\r\n", data);
    sprintf(data, "%sHost: things.ubidots.com\r\nUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
    sprintf(data, "%sX-Auth-Token: %s\r\nConnection: close\r\n\r\n", data, _token);

    Spark.process(); // Cleans previous processes
    _client.connect(SERVERHTTP, PORTHTTP); // Initial connection

    while (!_client.connected()) {
        if (_debug) {
            Serial.println("Attemping to connect");
        }
        _client.connect(SERVERHTTP, PORTHTTP);
        max_retries++;
        if (max_retries > 5) {
            if (_debug) {
                Serial.println("Could not connect to server");
            }
            free(data);
            return NULL;
        }
        delay(5000);
    }

    if (_debug) {
        Serial.println(F("Getting your variable with request: "));
        Serial.println(F(data));
    }

    _client.print(data);

    while (!_client.available() && timeout < 2000) {
        timeout++;
        if (timeout >= 2000) {
            if (_debug) {
                Serial.println(F("Error, max timeout reached"));
            }
            _client.stop();
            free(data);
            return NULL;
        }
    }

    while (_client.available()) {
        char c = _client.read();
        if (c == -1) {
            if (_debug) {
                Serial.println(F("Error reading from server"));
            }
            _client.stop();
            free(data);
            return NULL;
        }
        response += c;
    }

    if (_debug) {
        Serial.println(F("response:"));
        Serial.println(response);
    }

    uint8_t bodyPosinit = 4 + response.indexOf("\r\n\r\n");
    response = response.substring(bodyPosinit);

    int context_init = 13 + response.indexOf("\"context\":");
    int context_end = response.indexOf(", \"created_at\":") - 1;

    if (context_end < context_init) {
        if (_debug) {
            Serial.println("Error reading values from server");
        }
        return NULL;
    }

    response = response.substring(context_init, context_end);
    if (_debug) {
        Serial.println("Value obtained:");
        Serial.println(response);
    }

    char* context = new char [response.length() + 1];
    free(data);
    _client.stop();
    return context;
}


/***************************************************************************
FUNCTIONS TO SEND DATA
***************************************************************************/

/**
 * Add a value of variable to save
 * @arg variable_id variable id or name to save in a struct
 * @arg value variable value to save in a struct
 * @arg ctext [optional] is the context that you will save, default
 * @arg timestamp_val [optional] is the timestamp for the actual value
 * is NULL
 */

void Ubidots::add(char *variable_id, double value) {
    return add(variable_id, value, NULL, NULL);
}


void Ubidots::add(char *variable_id, double value, char *ctext) {
    return add(variable_id, value, ctext, NULL);
}


void Ubidots::add(char *variable_id, double value, char *ctext, long unsigned timestamp_val) {
    (val + _currentValue)->idName = variable_id;
    (val + _currentValue)->idValue = value;
    (val + _currentValue)->contextOne = ctext;
    (val + _currentValue)->timestamp_val = timestamp_val;
    _currentValue++;
    if (_currentValue > MAX_VALUES) {
        Serial.println(F("You are sending more than the maximum of consecutive variables"));
        _currentValue = MAX_VALUES;
    }
}


/**
 * This function is to set UDP or TCP as sending data method
 * @arg method is the method that you want to use
 * @return true uppon succes
 */


void Ubidots::setMethod(uint8_t method) {
    if (method >= 0 && method <= 2) {
        _method = method;
    }
}


/**
 * This function is to set the name of your device to visualize,
 * if you don't call this method the name by default will be 'Particle'
 * @arg deviceName is the name to display in Ubidots, avoid to use special
 * characters or blank spaces
 * @return true uppon succes
 */

void Ubidots::setDeviceName(char* deviceName) {
    _dsName = deviceName;
}


/**
 * This function is to set your device label, the device
 * label is the unique device identifier in Ubidots.
 * if you don't call this method the name by default will be the device ID
 * @arg deviceLabel is the device label, avoid to use special
 * characters or blank spaces
 * @return true uppon succes
 */

void Ubidots::setDeviceLabel(char* deviceLabel) {
    _pId = deviceLabel;
}


/**
 * Assamble all package to send in TCP or UDP method
 * @arg timestamp_global [optional] is the timestamp for all the variables added
 using add() method, if a timestamp_val was declared on the add() method, Ubidots
 will take as timestamp for the val the timestamp_val instead of the timestamp_global
 * @return true upon success, false upon error.
 */

bool Ubidots::sendAll() {
    return sendAll(NULL);
}


bool Ubidots::sendAll(unsigned long timestamp_global) {
    int i;
    char* allData = (char *) malloc(sizeof(char) * 700);
    if ( timestamp_global != NULL) {
        if (_dsName == "Particle") {
            sprintf(allData, "%s/%s|POST|%s|%s@%lu%s=>", USER_AGENT, VERSION, _token, _pId, timestamp_global, "000");
        } else {
            sprintf(allData, "%s/%s|POST|%s|%s:%s@%lu%s=>", USER_AGENT, VERSION, _token, _pId, _dsName, timestamp_global, "000");
        }
    } else {
        if (_dsName == "Particle") {
            sprintf(allData, "%s/%s|POST|%s|%s=>", USER_AGENT, VERSION, _token, _pId);
        } else {
            sprintf(allData, "%s/%s|POST|%s|%s:%s=>", USER_AGENT, VERSION, _token, _pId, _dsName);
        }
    }
    for (i = 0; i < _currentValue; ) {
        sprintf(allData, "%s%s:%f", allData, (val + i)->idName, (val + i)->idValue);
        if ((val + i)->timestamp_val != NULL) {
            sprintf(allData, "%s@%lu%s", allData, (val + i)->timestamp_val, "000");
        }
        if ((val + i)->contextOne != NULL) {
            sprintf(allData, "%s$%s", allData, (val + i)->contextOne);
        }
        i++;
        if (i < _currentValue) {
            sprintf(allData, "%s,", allData);
        }
    }
    sprintf(allData, "%s|end", allData);

    if (_debug) {
        Serial.println(allData);
    }

    if (_method == TYPE_TCP) {
        return sendAllTCP(allData);
    }

    if (_method == TYPE_UDP) {
        return sendAllUDP(allData);
    }
}


/**
 * Send all package via UDP method
 * @arg buffer the message to send
 * @reutrn true upon success, false upon error.
 */

bool Ubidots::sendAllUDP(char* buffer) {
    int size;
    IPAddress ipAddress;

    // Obtains the remote server's IP
    HAL_IPAddress ip;
    network_interface_t t;
    ipAddress = (inet_gethostbyname(_server, strlen(_server), &ip, t, NULL)<0) ?
               IPAddress(uint32_t(0)) : IPAddress(ip);

    // Routine to send data through UDP
    _clientUDP.begin(PORT);
    if (ipAddress) {
        if (! (_clientUDP.beginPacket(ipAddress, PORT)
                && _clientUDP.write(buffer)
                && _clientUDP.endPacket())) {
            if (_debug) {
                Serial.println("ERROR sending values with UDP");
            }
            _currentValue = 0;
            _clientUDP.stop();
            free(buffer);
            return false;
        }
    } else {
        if (_debug) {
            Serial.println("ERROR, could not solve IP Address of the remote server, please check your DNS setup");
        }
        _currentValue = 0;
        _clientUDP.stop();
        free(buffer);
        return false;
    }

    _currentValue = 0;
    _clientUDP.stop();
    free(buffer);
    return true;
}


/**
 * Send all package via TCP method
 * @arg buffer the message to send
 * @reutrn true upon success, false upon error.
 */

bool Ubidots::sendAllTCP(char* buffer) {
    int i = 0;
    while (!_client.connected() && i < 6) {
        i++;
        if (_debug) {
            Serial.println("not connected, trying to connect again");
        }
        _client.connect(_server, PORT);
        if (i == 5) {
            if (_debug) {
                Serial.println("Max attempts to connect reached, data could not be sent");
            }
            return false;
        }
    }
    if (_client.connected()) {        // Connect to the server
        if (_debug) {
            Serial.println("Sending data");
        }
        _client.println(buffer);
        _client.flush();
        _client.stop();
        free(buffer);
        _currentValue = 0;
        return true;
    }
    _currentValue = 0;
    return false; // If any of the above conditions is reached, returns false to indicate an unexpected issue
}

/***************************************************************************
AUXILIAR FUNCTIONS
***************************************************************************/

/**
 * Turns on or off debug messages
 * @debug is a bool flag to activate or deactivate messages
 */

void Ubidots::setDebug(bool debug) {
    _debug = debug;
}

/*
 * © Francesco Potortì 2013 - GPLv3 - Revision: 1.13
 *
 * Send an NTP packet and wait for the response, return the Unix time
 *
 * To lower the memory footprint, no buffers are allocated for sending
 * and receiving the NTP packets.  Four bytes of memory are allocated
 * for transmision, the rest is random garbage collected from the data
 * memory segment, and the received packet is read one byte at a time.
 * The Unix time is returned, that is, seconds from 1970-01-01T00:00.
 */

unsigned long Ubidots::ntpUnixTime () {

    static int udpInited = _clientTMP.begin(123); // open socket on arbitrary port

    // Only the first four bytes of an outgoing NTP packet need to be set
    // appropriately, the rest can be whatever.
    const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

    // Fail if WiFiUdp.begin() could not init a socket
    if (! udpInited)
        return 0;

    // Clear received data from possible stray received packets
    _clientTMP.flush();

    _client.connect(TIME_SERVER, 123);
    IPAddress ipAddress = _client.remoteIP();
    _client.stop();


    // Send an NTP request
    if (! (_clientTMP.beginPacket(ipAddress, 123) // 123 is the NTP port
            && _clientTMP.write((byte *)&ntpFirstFourBytes, 48) == 48
            && _clientTMP.endPacket()))
        return 0;               // sending request failed

    // Wait for response; check every pollIntv ms up to maxPoll times
    const int pollIntv = 150;     // poll every this many ms
    const byte maxPoll = 15;      // poll up to this many times
    int pktLen;               // received packet length
    for (byte i = 0; i < maxPoll; i++) {
        if ((pktLen = _clientTMP.parsePacket()) == 48)
            break;
        delay(pollIntv);
    }
    if (pktLen != 48)
        return 0;               // no correct packet received

    // Read and discard the first useless bytes
    // Set useless to 32 for speed; set to 40 for accuracy.
    const byte useless = 40;
    for (byte i = 0; i < useless; ++i)
        _clientTMP.read();

    // Read the integer part of sending time
    unsigned long time = _clientTMP.read();  // NTP time
    for (byte i = 1; i < 4; i++)
        time = time << 8 | _clientTMP.read();

    // Round to the nearest second if we want accuracy
    // The fractionary part is the next byte divided by 256: if it is
    // greater than 500ms we round to the next second; we also account
    // for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
    // additionally, we account for how much we delayed reading the packet
    // since its arrival, which we assume on average to be pollIntv/2.
    time += (_clientTMP.read() > 115 - pollIntv / 8);
    // Discard the rest of the packet
    _clientTMP.flush();
    return time - 2208988800ul;       // convert NTP time to Unix time
}


/***************************************************************************
DEPRECATED FUNCTIONS
***************************************************************************/

/**
 * WARNING: This function is deprecated, use setDeviceName() instead
 * This function is to set you data source name
 * @arg dsName is the name of your data source name
 * @return true uppon succes
 */

bool Ubidots::setDatasourceName(char* dsName) {
    Serial.print("Warning, this function is deprecated,");
    Serial.print("please try to use the setDeviceName() method instead");
    _dsName = dsName;
    return true;
}

/**
 * WARNING: This function is deprecated, use setDeviceLabel() instead
 * This function is to set you data source Tag
 * @arg dsTag is the name of your data source tag
 * @return true uppon succes
 */

bool Ubidots::setDatasourceTag(char* dsTag) {
    Serial.print("Warning, this function is deprecated,");
    Serial.print("please try to use the setDeviceLabel() method instead");
    _pId = dsTag;
    return true;
}
