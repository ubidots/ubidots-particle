/*
Copyright (c) 2013-2018 Ubidots.

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

Ubidots::Ubidots(char* token, const char * server) {
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
  char* response = (char *) malloc(sizeof(char) * 40);
  char* data = (char *) malloc(sizeof(char) * 700);
  sprintf(data, "%s/%s|GET|%s|%s", USER_AGENT, VERSION, _token, id);
  sprintf(data, "%s|end", data);

  if (_debug) {
    Serial.println("Attemping to connect");
  }

  int timeout = 0;
  while (!_client.connected()) {
    _client.connect(UBIDOTS_SERVER, UBIDOTS_HTTP_PORT);
    timeout++;
    if (timeout > _timeout - 1) {
      if (_debug) {
        Serial.println("Could not connect to server");
      }
      free(response);
      free(data);
      return ERROR_VALUE;
    }
    delay(1);
  }

  if (_debug){
    Serial.println("attemping to retrieve data with request:");
    Serial.println(data);
  }


  if (_client.connect(UBIDOTS_SERVER, UBIDOTS_HTTP_PORT)) {
    if (_debug){
      Serial.println(F("Getting your variable: "));
    }
    _client.print(data);
  }

  timeout = 0;
  free(data);

  while(!_client.available() && timeout < _timeout) {
    timeout++;
    if (timeout >= _timeout - 1){
      free(response);
      Serial.println("timeout");
      return ERROR_VALUE;
    }
    delay(1);
  }

  int i = 0;
  for (int i = 0; i <= 40; i++){
    response[i] = '\0';
  }

   while (_client.available()) {
    response[i++] = (char)_client.read();
  }

  // Parses the answer, Expected "OK|{value}"
  char * pch = strchr(response, '|');
  if (pch != NULL){
    float num;
    pch[0] = '0';
    num = atof(pch);
    free(response);
    _client.stop();
    return num;
  }

  free(response);
  return ERROR_VALUE;
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
  char* data = (char *) malloc(sizeof(char) * 700);
  char* response = (char *) malloc(sizeof(char) * 40);
  sprintf(data, "%s/%s|LV|%s|%s:%s", USER_AGENT, VERSION, _token, device, variable);
  sprintf(data, "%s|end", data);

  int timeout = 0;

  if (_debug) {
    Serial.println("Attemping to connect");
  }

  while (!_client.connected()) {
    _client.connect(UBIDOTS_SERVER, UBIDOTS_HTTP_PORT);
    timeout++;
    if (timeout > _timeout - 1) {
      if (_debug) {
        Serial.println("Could not connect to server");
      }
      free(response);
      free(data);
      return ERROR_VALUE;
    }
    delay(1);
  }

  if (_debug){
    Serial.println("attemping to retrieve data with request:");
    Serial.println(data);
  }

  if (_client.connect(UBIDOTS_SERVER, UBIDOTS_HTTP_PORT)) {
    if (_debug){
      Serial.println(F("Getting your variable: "));
    }
    _client.print(data);
  }

  timeout = 0;
  free(data);

  while(!_client.available() && timeout < _timeout) {
    timeout++;
    if (timeout >= _timeout - 1){
      Serial.println("Server connection timeout");
      free(response);
      return ERROR_VALUE;
    }
    delay(1);
  }

  int i = 0;
  for (int i = 0; i <= 40; i++){
    response[i] = '\0';
  }

   while (_client.available()) {
    response[i++] = (char)_client.read();
  }

  // Parses the answer, Expected "OK|{value}"
  char * pch = strchr(response, '|');
  if (pch != NULL){
    float num;
    pch[0] = '0';
    num = atof(pch);
    free(response);
    _client.stop();
    return num;
  }

  free(response);
  return ERROR_VALUE;
}


/**
 * This function obtains the variable from a variable in Ubidots using HTTP requests format
 * @arg id is the Variable ID
 * @return value as float, if any error is raised retuns ERROR_VALUE
 */

float Ubidots::getValueHTTP(char* id) {
  char* response = (char *) malloc(sizeof(char) * 700);
  char* data = (char *) malloc(sizeof(char) * 300);

  sprintf(data, "GET /api/v1.6/variables/%s", id);
  sprintf(data, "%s/values?page_size=1 HTTP/1.1\r\n", data);
  sprintf(data, "%sHost: things.ubidots.com\r\nUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
  sprintf(data, "%sX-Auth-Token: %s\r\nConnection: close\r\n\r\n", data, _token);

  _client.connect(UBIDOTS_SERVER, UBIDOTS_HTTP_PORT); // Initial connection

  int timeout = 0;
  if (_debug) {
    Serial.println("Attemping to connect");
  }

  while (!_client.connected()) {
    _client.connect(UBIDOTS_SERVER, UBIDOTS_HTTP_PORT);
    timeout++;
    if (timeout > _timeout - 1) {
      if (_debug) {
        Serial.println("Could not connect to server");
      }
      free(response);
      free(data);
      return ERROR_VALUE;
    }
    delay(1);
  }

  if (_debug) {
    Serial.println(F("Getting your variable with request: "));
    Serial.println(F(data));
  }

  _client.print(data);
  free(data);
  timeout = 0;

  while(!_client.available() && timeout < _timeout) {
    timeout++;
    if (timeout >= _timeout - 1){
      Serial.println("Server connection timeout");
      free(response);
      return ERROR_VALUE;
    }
    delay(1);
  }

  int i = 0;
  for (int i = 0; i <= 40; i++){
    response[i] = '\0';
  }

  while (_client.available()) {
    response[i++] = (char)_client.read();
  }

  // Parses the answer, Expected "OK|{value}"
  char * pch = strstr(response, "value");
  if (pch != NULL){
    char * pch2 = strchr(pch, ':');
    pch = strrchr(pch2, ']');
    int index = (int)(pch - pch2 - 1);

    char num[15];
    for (int i = 0; i < 15; i++){
      num[i] = '\0';
    }
    memcpy(num, pch2, index);
    num[0] = '0';
    num[1] = '0';
    float result = atof(num);
    free(response);
    _client.stop();
    return result;
  }

  if (_debug){
    Serial.println("there was an error parsing the server answer");
  }

  return ERROR_VALUE;
}


/**
 * This function obtains the context from a variable in Ubidots
 * @arg id is the Variable ID
 * @return Context as char array
 */

char* Ubidots::getVarContext(char* id) {
  int timeout = 0;
  uint8_t max_retries = 0;
  char* data = (char *) malloc(sizeof(char) * 300);
  char* response = (char *) malloc(sizeof(char) * 700);

  sprintf(data, "GET /api/v1.6/variables/%s", id);
  sprintf(data, "%s/values?page_size=1 HTTP/1.1\r\n", data);
  sprintf(data, "%sHost: things.ubidots.com\r\nUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
  sprintf(data, "%sX-Auth-Token: %s\r\nConnection: close\r\n\r\n", data, _token);

  _client.connect(UBIDOTS_SERVER, UBIDOTS_HTTP_PORT); // Initial connection

  while (!_client.connected()) {
    if (_debug) {
      Serial.println("Attemping to connect");
    }
    _client.connect(UBIDOTS_SERVER, UBIDOTS_HTTP_PORT);
    max_retries++;
    if (max_retries > 5) {
      if (_debug) {
        Serial.println("Could not connect to server");
      }
      free(data);
      char* error_message = "error";
      return error_message;
    }
    delay(5000);
  }

  if (_debug) {
    Serial.println(F("Getting your variable with request: "));
    Serial.println(F(data));
  }

  _client.print(data);
  free(data);

  while (!_client.available() && timeout < _timeout) {
    timeout++;
    if (timeout >= _timeout - 1) {
      if (_debug) {
        Serial.println(F("Error, max timeout reached"));
      }
      _client.stop();
      free(response);
      free(data);
      return "error";
    }
  }

  // Populates with null values the response array
  for (int i = 0; i <= 700; i++){
    response[i] = '\0';
  }

  int i = 0;
  while (_client.available()) {
    if (i >= 699){
      break;
    }
    response[i++] = (char)_client.read();
  }

  if (_debug) {
    Serial.println(F("response:"));
    Serial.println(response);
  }

  // Parses the answer
  char * pch = strchr(response, '[');

  if (pch != NULL){
    char * pch2 = strchr(pch + 2, '{');
    pch = strchr(pch2, '}');
    int index = (int)(pch - pch2 + 1);

    // Populates with null values the context array
    char context[400];
    for (int i = 0; i < 400; i++){
      context[i] = '\0';
    }

    memcpy(context, pch2, index);
    free(response);
    _client.stop();
    return context;
  }

  free(response);
  return "error";
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
  _dirty = true;
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

bool Ubidots::sendValuesTCP() {
  return sendValuesTCP(NULL);
}

bool Ubidots::sendValuesTCP(unsigned long timestamp_global) {
  /* Builds the payload */
  char* payload = (char *) malloc(sizeof(char) * 700);
  buildTcpPayload(payload, timestamp_global);

  /* Makes sure that the client is connected to Ubidots */
  _client.connect(UBIDOTS_SERVER, UBIDOTS_TCP_PORT);
  reconnect();

  /* Sends data to Ubidots */
  if (_client.connected()) {
    _client.print(payload);
    free(payload);
  } else {
    free(payload);
    if (_debug) {
      Serial.println("Could not connect to the server");
    }
    return false;
  }

  /* Waits for the server's answer */
  if (!waitServerAnswer()) {
    return false;
  }

  /* Parses the server answer, returns true if it is 'Ok' */
  char* response = (char *) malloc(sizeof(char) * 100);
  bool result = parseTCPAnswer(response);
  free(response);
  return result;

}

bool Ubidots::sendValuesUDP() {
  return sendValuesUDP(NULL);
}

bool Ubidots::sendValuesUDP(unsigned long timestamp_global) {
  /* Builds the payload */
  char* payload = (char *) malloc(sizeof(char) * 700);
  buildTcpPayload(payload, timestamp_global);

  /* Obtains the remote server's IP */
  IPAddress serverIpAddress = getServerIp();

  if (! serverIpAddress){
    if (_debug) {
      Serial.println("ERROR, could not solve IP Address of the remote server, please check your DNS setup");
    }
    free(payload);
    _currentValue = 0;
    _clientUDP.stop();
    _dirty = false;
    return false;
  }

  /* Sends data to Ubidots */
  _clientUDP.begin(UBIDOTS_TCP_PORT);
  if (! (_clientUDP.beginPacket(serverIpAddress, UBIDOTS_TCP_PORT)
      && _clientUDP.write(payload)
      && _clientUDP.endPacket())) {
    if (_debug) {
      Serial.println("ERROR sending values with UDP");
    }
    _currentValue = 0;
    _clientUDP.stop();
    _dirty = false;
    free(payload);
    return false;
  }

  _currentValue = 0;
  _clientUDP.stop();
  _dirty = false;
  free(payload);
  return true;
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
 * @return true upon success, false upon error.
 */

bool Ubidots::sendAllUDP(char* buffer) {
  int size;
  IPAddress ipAddress;

  // Obtains the remote server's IP
  HAL_IPAddress ip;
  network_interface_t t;
  ipAddress = (inet_gethostbyname(_server, strlen(_server), &ip, t, NULL)<0) ?
       IPAddress(uint32_t(0)) : IPAddress(ip);

  if (! ipAddress){
    if (_debug) {
      Serial.println("ERROR, could not solve IP Address of the remote server, please check your DNS setup");
    }
    _currentValue = 0;
    _clientUDP.stop();
    _dirty = false;
    free(buffer);
    return false;
  }


  // Routine to send data through UDP
  _clientUDP.begin(UBIDOTS_TCP_PORT);
  if (! (_clientUDP.beginPacket(ipAddress, UBIDOTS_TCP_PORT)
      && _clientUDP.write(buffer)
      && _clientUDP.endPacket())) {
    if (_debug) {
      Serial.println("ERROR sending values with UDP");
    }
    _currentValue = 0;
    _clientUDP.stop();
    _dirty = false;
    free(buffer);
    return false;
  }

  _currentValue = 0;
  _clientUDP.stop();
  _dirty = false;
  free(buffer);
  return true;
}


/**
 * Send all package via TCP method
 * @arg buffer the message to send
 * @return true upon success, false upon error.
 */

bool Ubidots::sendAllTCP(char* buffer) {
  int i = 0;
  while (!_client.connected() && i < 6) {
    i++;
    if (_debug) {
      Serial.println("not connected, trying to connect again");
    }
    _client.connect(_server, UBIDOTS_TCP_PORT);
    if (i == 5) {
      if (_debug) {
        Serial.println("Max attempts to connect reached, data could not be sent");
      }
      free(buffer);
      _dirty = false;
      return false;
    }
  }

  if (_client.connected()) {    // Connect to the server
    if (_debug) {
      Serial.println("Sending data");
    }
    _client.println(buffer);
    _client.flush();
    _client.stop();
    free(buffer);
    _dirty = false;
    _currentValue = 0;
    return true;
  }

  free(buffer);
  _dirty = false;
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


/**
 * Lets to the user to know if the buffer array has data or not
 * @debug is a bool flag to activate or deactivate messages
 */

bool Ubidots::isDirty() {
  return _dirty;
}

void Ubidots::setTimeout(int timeout){
  // Changes the max timeout value
  _timeout = timeout;
}

void Ubidots::buildTcpPayload(char* payload, unsigned long timestamp_global) {
  sprintf(payload, "");
  sprintf(payload, "%s/%s|POST|%s|", USER_AGENT, VERSION, _token);
  sprintf(payload, "%s%s:%s", payload, _pId, _dsName);

  if (timestamp_global != NULL) {
    sprintf(payload, "%s@%lu", payload, timestamp_global);
  }

  sprintf(payload, "%s=>", payload);
  for (uint8_t i = 0; i < _currentValue;) {
    sprintf(payload, "%s%s:%f", payload, (val + i)->idName, (val+i)->idValue);

    if ((val + i)->timestamp_val != NULL) {
      sprintf(payload, "%s@%lu%s", payload, (val + i)->timestamp_val, "000");
    }
    if ((val + i)->contextOne != NULL) {
      sprintf(payload, "%s$%s", payload, (val + i)->contextOne);
    }

    i++;

    if (i < _currentValue) {
      sprintf(payload, "%s,", payload);
    } else {
      sprintf(payload, "%s|end", payload);
      _currentValue = 0;
    }
  }

  if (_debug) {
    Serial.println("----------");
    Serial.println("payload:");
    Serial.println(payload);
    Serial.println("----------");
    Serial.println("");
  }

}

void Ubidots::reconnect() {
  uint8_t attempts = 0;
  while (!_client.connected() && attempts < 5) {
    if (_debug) {
      Serial.print("Trying to connect to Ubidots");
      Serial.print(", attempt number: ");
      Serial.println(attempts);
    }
    _client.connect(UBIDOTS_SERVER, UBIDOTS_TCP_PORT);
    attempts += 1;
    delay(1000);
  }
}

bool Ubidots::waitServerAnswer() {
  int timeout = 0;
  while(!_client.available() && timeout < _timeout) {
    timeout++;
    delay(1);
    if (timeout > _timeout - 1) {
      if (_debug) {
        Serial.println("timeout, could not read any response from the server");
      }
      return false;
    }
  }
  return true;
}

bool Ubidots::parseTCPAnswer(char* response) {
  int j = 0;

  if (_debug){
    Serial.println("----------");
    Serial.println("Server's response:");
  }

  while (_client.available()) {
    char c = _client.read();
    if (_debug) {
      Serial.write(c);
    }
    response[j] = c;
    j++;
  }

  if (_debug){
    Serial.println("\n----------");
  }

  response[j] = '\0';

  char *pch = strstr(response, "OK");
  bool result = false;
  if (pch != NULL) {
    result = true;
  }

  return result;
}


IPAddress Ubidots::getServerIp() {
  /* Obtains the remote server's IP */
  IPAddress serverIpAddress;
  HAL_IPAddress ip;
  network_interface_t t;
  serverIpAddress = (inet_gethostbyname(_server, strlen(_server), &ip, t, NULL)<0) ?
       IPAddress(uint32_t(0)) : IPAddress(ip);

  return serverIpAddress;
}
