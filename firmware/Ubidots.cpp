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
 */
Ubidots::Ubidots(char* token) {
    _token = token;
    _method = TYPE_TCP;
    _dsName = "Particle";
    currentValue = 0;
    val = (Value *)malloc(MAX_VALUES*sizeof(Value));
    String str = Particle.deviceID();
    _pId = new char[str.length() + 1];
    strcpy (_pId, str.c_str());
}
void Ubidots::setMethod(uint8_t method) {
    if (method >= 0 && method <= 2) {
        _method = method;
    }
}
bool Ubidots::setDatasourceName(char* dsName) {
    _dsName = dsName;
    return true;
}
bool Ubidots::setDatasourceTag(char* dsTag) {
    _pId = dsTag;
    return true;
}

/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValueWithDatasource(char* dsTag, char* idName) {
  float num;
  int i = 0;
  char* allData = (char *) malloc(sizeof(char) * 300);
  String response;
  uint8_t bodyPosinit;
  sprintf(allData, "Particle/1.0|LV|%s|%s:%s|end", _token, dsTag, idName);
  while (!_client.connected() && i < 6) {
        i++;
        _client.connect(SERVER, PORT);
    }
    if (_client.connected()) {  // Connect to the server
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected");
        Serial.println(allData);
#endif
        _client.println(allData);
        _client.flush();
    }
    i = 50000;
    while (!_client.available()|| i == 0) {
        i--;
    }
    while (_client.available()) {
        char c = _client.read();
        response += c;
    }
    bodyPosinit = 3 + response.indexOf("OK|");
    response = response.substring(bodyPosinit);
    num = response.toFloat();
    currentValue = 0;
    _client.stop();
    free(allData);
    _client.stop();
    return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value) {
  add(variable_id, value, NULL);
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value, char *ctext1) {
  (val+currentValue)->idName = variable_id;
  (val+currentValue)->idValue = value;
  (val+currentValue)->contextOne = ctext1;
  currentValue++;
  if (currentValue > MAX_VALUES) {
        Serial.println(F("You are sending more than the maximum of consecutive variables"));
        currentValue = MAX_VALUES;
  }
}
/**
 * Send all data of all variables that you saved by TCP method
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
    if (_method == TYPE_SMS) {
        return sendAllSMS(allData);
    }
}
bool Ubidots::sendAllUDP(char* buffer) {
    int size;
    _clientUDP.begin(8888);
    while (_clientUDP.sendPacket(buffer, strlen(buffer), REMOTE_IP, PORT) <= 0) {
        Serial.println("ERROR");
    }
    delay(500);
    size = _clientUDP.parsePacket();
    while (_clientUDP.available() > 0) {
#ifdef DEBUG_UBIDOTS
        Serial.write(c);
#endif
    }
    currentValue = 0;
    _clientUDP.stop();
    free(buffer);
    return true;
}
bool Ubidots::sendAllSMS(char* buffer) {
    return true;
}
 
bool Ubidots::sendAllTCP(char* buffer) {
    int i = 0;
    while (!_client.connected() && i < 6) {
        i++;
        _client.connect(SERVER, PORT);
    }
    if (_client.connected()) {        // Connect to the server
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected");
#endif
        _client.println(buffer);
        _client.flush();
    }
    delay(200);
    while (_client.available() > 0) {
        char c = _client.read();
#ifdef DEBUG_UBIDOTS
        Serial.write(c);
#endif
    }
    currentValue = 0;
    _client.stop();
    free(buffer);
    return true;
}
