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

Made by Mateo Velez - Metavix for Ubidots Inc

*/

#include "Ubidots.h"
/**
 * Constructor.
 * Default method is TCO
 * Default dsNmae is Particle
 */
Ubidots::Ubidots(char* token, char* server) {
    _token = token;
    _server = server;
    _method = TYPE_UDP;
    _dsName = "Particle";
    lastValue = NULL;
    currentValue = 0;
    val = (Value *)malloc(MAX_VALUES*sizeof(Value));
    String str = Particle.deviceID();
    _pId = new char[str.length() + 1];
    strcpy(_pId, str.c_str());
}
/** 
 * This function is to set UDP or TCP method
 * @arg method is the method that you want to use
 * @return true uppon succes
 */
void Ubidots::setMethod(uint8_t method) {
    if (method >= 0 && method <= 2) {
        _method = method;
    }
}

/** 
 * This function is to set you data source name
 * @arg dsName is the name of your data source name
 * @return true uppon succes
 */

bool Ubidots::setDatasourceName(char* dsName) {
    _dsName = dsName;
    return true;
}

/** 
 * This function is to set you data source Tag
 * @arg dsTag is the name of your data source tag
 * @return true uppon succes
 */

bool Ubidots::setDatasourceTag(char* dsTag) {
    _pId = dsTag;
    return true;
}

/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */

float Ubidots::getValue(char* id) {
    char buffer[500];
    uint8_t bodyPosinit = 0;
    uint8_t bodyPosend = 0;
    uint8_t i = 0;
    float num;
    while (!_client.connected() && i < 6) {
        i++;
        _client.connect(SERVERHTTP, PORTHTTP);
    }
    if (_client.connected()) {  // Connect to the server
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected");
#endif
        _client.print("GET /api/v1.6/variables/");
        _client.print(id);
        _client.print("/values?token=");
        _client.print(_token);
        _client.println(" HTTP/1.1\nHost: things.ubidots.com\nContent-Type: application/json\nConnection: close");
        _client.println();
        _client.println();
        _client.flush();
    }
    unsigned int bufferPosition = 0;
    unsigned long lastRead = millis();
    unsigned long firstRead = millis();
    bool error = false;
    bool timeout = false;
    int bytes = _client.available();
    do {
#ifdef DEBUG_UBIDOTS
        if (bytes) {
            Serial.print("Receiving HTTP transaction of ");
            Serial.print(bytes);
            Serial.println(" bytes.");
        }
#endif
        while (_client.available()) {
            char c = _client.read();
#ifdef DEBUG_UBIDOTS
            Serial.print(c);
#endif
            lastRead = millis();

            if (c == -1) {
                error = true;

#ifdef DEBUG_UBIDOTS
                Serial.println("Error: No data available.");
#endif

                break;
            }

            // Check that received character fits in buffer before storing.
            if (bufferPosition < sizeof(buffer)-1) {
                buffer[bufferPosition] = c;
            } else if ((bufferPosition == sizeof(buffer)-1)) {
                buffer[bufferPosition] = '\0';  // Null-terminate buffer
                _client.stop();
                error = true;

                #ifdef DEBUG_UBIDOTS
                Serial.println("Error: Response body larger than buffer.");
                #endif
            }
            bufferPosition++;
        }
        buffer[bufferPosition] = '\0';  // Null-terminate buffer
        if (bytes) {
            Serial.print("End of TCP transaction.");
            _client.stop();
        }

        // Check that there hasn't been more than 5s since last read.
        timeout = millis() - lastRead > TIMEOUT;

        // Unless there has been an error or timeout wait 200ms to allow server
        // to respond or close connection.
        if (!error && !timeout) {
            delay(200);
        }
    } while (_client.connected() && !timeout && !error);

#ifdef DEBUG_UBIDOTS
    Serial.print("End of TCP Response (");
    Serial.print(millis() - firstRead);
    Serial.println("ms).");
#endif
    _client.stop();

    String raw_response(buffer);
    bodyPosinit = 4 + raw_response.indexOf("\r\n\r\n");
    raw_response = raw_response.substring(bodyPosinit);
    bodyPosinit = 9 + raw_response.indexOf("\"value\": ");
    bodyPosend = 13 + raw_response.indexOf(", \"timestamp\"");
    raw_response = raw_response.substring(bodyPosinit, bodyPosend);
    num = raw_response.toFloat();
    if (bodyPosend < 50) {  // 50 is the min value of content of body
        return lastValue;
    } else {
        lastValue = num;
        return num;
    }
}

/** 
 * This function is to get value from the Ubidots API with the data source tag
 * and variable tag
 * @arg dsTag is the Tag of Data Source
 * @arg idName is the Tag of the variable
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValueWithDatasource(char* dsTag, char* idName) {
  float num;
  int i = 0;
  char buffer[50];
  uint8_t bodyPosinit = 0;
  while (!_client.connected() && i < 6) {
        i++;
        _client.connect(SERVER, PORT);
    }
    if (_client.connected()) {  // Connect to the server
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected");
#endif
        _client.print(USER_AGENT);
        _client.print(F("/"));
        _client.print(VERSION);
        _client.print(F("|LV|"));
        _client.print(_token);
        _client.print(F("|"));
        _client.print(dsTag);
        _client.print(F(":"));
        _client.print(idName);
        _client.print(F("|end"));
        
    }
    _client.flush();
    
    unsigned int bufferPosition = 0;
    unsigned long lastRead = millis();
    unsigned long firstRead = millis();
    bool error = false;
    bool timeout = false;
    int bytes = 0;
    bytes = _client.available();
    int count = 0;
    while(!bytes && count < 10000) {
        bytes = _client.available();
        delay(1);
        count++;
    }
    do {
#ifdef DEBUG_UBIDOTS
        if (bytes) {
            Serial.print("Receiving TCP transaction of ");
            Serial.print(bytes);
            Serial.println(" bytes.");
        }
#endif

        while (_client.available()) {
            char c = _client.read();
#ifdef DEBUG_UBIDOTS
            Serial.print(c);
#endif
            lastRead = millis();

            if (c == -1) {
                error = true;

#ifdef DEBUG_UBIDOTS
                Serial.println("Error: No data available.");
#endif

                break;
            }
            buffer[bufferPosition] = c;
            bufferPosition++;
        }
        buffer[bufferPosition] = '\0';  // Null-terminate buffer
        if (bytes) {
            Serial.print("End of TCP transaction.");
            _client.stop();
            break;
        }

        // Check that there hasn't been more than 5s since last read.
        timeout = millis() - lastRead > TIMEOUT;

        // Unless there has been an error or timeout wait 200ms to allow server
        // to respond or close connection.
        if (!error && !timeout) {
            delay(200);
        }
    } while (bytes > 0 && !timeout && !error);

#ifdef DEBUG_UBIDOTS
    Serial.print("End of TCP Response (");
    Serial.print(millis() - firstRead);
    Serial.println("ms).");
#endif
    _client.stop();
    String raw_response(buffer);
    bodyPosinit = 3 + raw_response.indexOf("OK|");
    raw_response = raw_response.substring(bodyPosinit);
    num = raw_response.toFloat();
    if (bodyPosinit != 3) {  // 3 is the number of "OK|"
        return lastValue;
    } else {
        lastValue = num;
        return num;
    }
}

/**
 * Add a value of variable to save
 * @arg variable_id variable id or name to save in a struct
 * @arg value variable value to save in a struct
 * @arg ctext1 is the context that you will save, default
 * is NULL
 */

void Ubidots::add(char *variable_id, double value) {
  return add(variable_id, value, NULL, NULL);
}

void Ubidots::add(char *variable_id, double value, char *ctext) {
  return add(variable_id, value, ctext, NULL);
}

void Ubidots::add(char *variable_id, double value, char *ctext, char *timestamp) {
  (val+currentValue)->idName = variable_id;
  (val+currentValue)->idValue = value;
  (val+currentValue)->contextOne = ctext;
  (val+currentValue)->timestamp = timestamp;
  currentValue++;
  if (currentValue > MAX_VALUES) {
        Serial.println(F("You are sending more than the maximum of consecutive variables"));
        currentValue = MAX_VALUES;
  }
}

/**
 * Assamble all package to send in TCP or UDP method
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll() {
    int i;
    char* allData = (char *) malloc(sizeof(char) * 700);
    if (_dsName == "Particle") {
        sprintf(allData, "%s|POST|%s|%s=>", USER_AGENT, _token, _pId);
    } else {
        sprintf(allData, "%s|POST|%s|%s:%s=>", USER_AGENT, _token, _pId, _dsName);
    }
    for (i = 0; i < currentValue; ) {
        sprintf(allData, "%s%s:%f", allData, (val + i)->idName, (val + i)->idValue);
        if ((val + i)->timestamp != NULL) {
            sprintf(allData, "%s@%s", allData, (val + i)->timestamp);
        }
        if ((val + i)->contextOne != NULL) {
            sprintf(allData, "%s$%s", allData, (val + i)->contextOne);
        }
        i++;
        if (i < currentValue) {
            sprintf(allData, "%s,", allData);
        }
    }
    sprintf(allData, "%s|end", allData);
#ifdef DEBUG_UBIDOTS
    Serial.println(allData);
#endif
    if (_method == TYPE_TCP) {
        return sendAllTCP(allData);
    }
    if (_method == TYPE_UDP) {
        return sendAllUDP(allData);
    }
    /*if (_method == TYPE_SMS) {
        return sendAllSMS(allData);
    }*/
}

/**
 * Send all package via UDP method
 * @arg buffer the message to send
 * @reutrn true upon success, false upon error.
 */

bool Ubidots::sendAllUDP(char* buffer) {
    int size;
    _clientUDP.begin(8888);
    if (_clientUDP.sendPacket(buffer, strlen(buffer), REMOTE_IP, PORT) <= 0) {
        Serial.println("ERROR");
    }
    currentValue = 0;
    _clientUDP.stop();
    free(buffer);
    return true;
}
/*
// Work in progress
bool Ubidots::sendAllSMS(char* buffer) {
    return true;
}
*/

/**
 * Send all package via TCP method
 * @arg buffer the message to send
 * @reutrn true upon success, false upon error.
 */

bool Ubidots::sendAllTCP(char* buffer) {
    int i = 0;
    while (!_client.connected() && i < 6) {
        i++;
        _client.connect(_server, PORT);
    }
    if (_client.connected()) {        // Connect to the server
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected");
#endif
        _client.println(buffer);
        _client.flush();
    }
    currentValue = 0;
    _client.stop();
    free(buffer);
    return true;
}
