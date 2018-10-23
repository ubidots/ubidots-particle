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

Modified and maintained by Jose Garcia for Ubidots Inc

*/

#include "Ubidots.h"
#include "inet_hal.h"

/***************************************************************************
CONSTRUCTOR
***************************************************************************/

/**
 * @token [Mandatory] Ubidots Account token
 * @host [Mandatory] Ubidots Endpoint to send data, default industrial.api.ubidots.com
 */

Ubidots::Ubidots(char* token, const char * host) {
  _token = token;
  _host = host;
  _method = TYPE_UDP;
  _device_name = "particle";
  _currentValue = 0;  // Current value index from Value type structure
  val = (Value *)malloc(MAX_VALUES * sizeof(Value));
  String str = System.deviceID();
  _device_label = new char[str.length() + 1];
  strcpy(_device_label, str.c_str());
}


/***************************************************************************
FUNCTIONS TO RETRIEVE DATA
***************************************************************************/


/**
 * This function is to get value from the Ubidots API
 * @variable_id [Mandatory] variable id where you will get the data from
 * @return value obtained from the Ubidots API, if any error occurs
  the function returns ERROR_VALUE
 */

float Ubidots::getValue(char* variable_id) {
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
        Serial.println("Could not connect to host");
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
 * @arg device_label [Mandatory] is the label of the device
 * @arg variable_label [Mandatory] is the label of the variable
 * @return value obtained from the Ubidots API, if any error occurs
  the function returns ERROR_VALUE
 */

float Ubidots::getValueWithDatasource(char* device_label, char* variable) {
  char* data = (char *) malloc(sizeof(char) * 700);
  char* response = (char *) malloc(sizeof(char) * 40);
  sprintf(data, "%s/%s|LV|%s|%s:%s", USER_AGENT, VERSION, _token, device_label, variable);
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
        Serial.println("Could not connect to host");
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
        Serial.println("Could not connect to host");
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
    Serial.println("there was an error parsing the host answer");
  }

  return ERROR_VALUE;
}


/**
 * This function obtains the context from a variable in Ubidots
 * @arg variable_id is the Variable ID
 * @return Context as char array
 */

char* Ubidots::getVarContext(char* variable_id) {
  int timeout = 0;
  uint8_t max_retries = 0;
  char* data = (char *) malloc(sizeof(char) * 300);
  char* response = (char *) malloc(sizeof(char) * 700);

  sprintf(data, "GET /api/v1.6/variables/%s", variable_id);
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
        Serial.println("Could not connect to host");
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
 * @arg variable_labeln [Mandatory] variable label where the dot will be stored
 * @arg value [Mandatory] Dot value
 * @arg context [optional] Dot context to store. Default NULL
 * @arg dot_timestamp [optional] Dot timestamp, usefull for datalogger. Default NULL
 */

void Ubidots::add(char *variable_label, float value) {
  return add(variable_label, value, NULL, NULL);
}


void Ubidots::add(char *variable_label, float value, char *context) {
  return add(variable_label, value, context, NULL);
}


void Ubidots::add(char *variable_label, float value, char *context, long unsigned dot_timestamp) {
  _dirty = true;
  (val + _currentValue)->variable_label = variable_label;
  (val + _currentValue)->dot_value = value;
  (val + _currentValue)->dot_context = context;
  (val + _currentValue)->dot_timestamp = dot_timestamp;
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
 * @arg device_name is the name to display in Ubidots, avoid to use special
 * characters or blank spaces
 * @return true uppon succes
 */

void Ubidots::setDeviceName(char* device_name) {
  _device_name = device_name;
}


/**
 * This function is to set your device label, the device
 * label is the unique device identifier in Ubidots.
 * if you don't call this method the name by default will be the device ID
 * @arg device_label is the device label, avoid to use special
 * characters or blank spaces
 * @return true uppon succes
 */

void Ubidots::setDeviceLabel(char* device_label) {
  _device_label = device_label;
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
      Serial.println("Could not connect to the host");
    }
    return false;
  }

  /* Waits for the host's answer */
  if (!waitServerAnswer()) {
    return false;
  }

  /* Parses the host answer, returns true if it is 'Ok' */
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

  /* Obtains the remote host's IP */
  IPAddress serverIpAddress = getServerIp();

  if (! serverIpAddress){
    if (_debug) {
      Serial.println("ERROR, could not solve IP Address of the remote host, please check your DNS setup");
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
 * @arg timestamp_global [optional] Timestamp for all the dots
 * @return true upon success, false upon error.
 */

bool Ubidots::sendAll() {
  return sendAll(NULL);
}


bool Ubidots::sendAll(unsigned long timestamp_global) {
  int i;
  char* allData = (char *) malloc(sizeof(char) * 700);
  if ( timestamp_global != NULL) {
    if (_device_name == "Particle") {
      sprintf(allData, "%s/%s|POST|%s|%s@%lu%s=>", USER_AGENT, VERSION, _token, _device_label, timestamp_global, "000");
    } else {
      sprintf(allData, "%s/%s|POST|%s|%s:%s@%lu%s=>", USER_AGENT, VERSION, _token, _device_label, _device_name, timestamp_global, "000");
    }
  } else {
    if (_device_name == "Particle") {
      sprintf(allData, "%s/%s|POST|%s|%s=>", USER_AGENT, VERSION, _token, _device_label);
    } else {
      sprintf(allData, "%s/%s|POST|%s|%s:%s=>", USER_AGENT, VERSION, _token, _device_label, _device_name);
    }
  }

  for (i = 0; i < _currentValue; ) {
    sprintf(allData, "%s%s:%f", allData, (val + i)->variable_label, (val + i)->dot_value);
    if ((val + i)->dot_timestamp != NULL) {
      sprintf(allData, "%s@%lu%s", allData, (val + i)->dot_timestamp, "000");
    }
    if ((val + i)->dot_context != NULL) {
      sprintf(allData, "%s$%s", allData, (val + i)->dot_context);
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
    return sendAllTcp(allData);
  }

  if (_method == TYPE_UDP) {
    return sendAllUdp(allData);
  }
}


/**
 * Send all package via UDP method
 * @arg buffer [Mandatory] message to send
 * @return true upon success, false upon error.
 */

bool Ubidots::sendAllUdp(char* buffer) {
  int size;
  IPAddress ipAddress;

  // Obtains the remote host's IP
  HAL_IPAddress ip;
  network_interface_t t;
  ipAddress = (inet_gethostbyname(_host, strlen(_host), &ip, t, NULL)<0) ?
       IPAddress(uint32_t(0)) : IPAddress(ip);

  if (! ipAddress){
    if (_debug) {
      Serial.println("ERROR, could not solve IP Address of the remote host, please check your DNS setup");
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
 * @arg buffer [Mandatory] the message to send
 * @return true upon success, false upon error.
 */

bool Ubidots::sendAllTcp(char* buffer) {
  int i = 0;
  while (!_client.connected() && i < 6) {
    i++;
    if (_debug) {
      Serial.println("not connected, trying to connect again");
    }
    _client.connect(_host, UBIDOTS_TCP_PORT);
    if (i == 5) {
      if (_debug) {
        Serial.println("Max attempts to connect reached, data could not be sent");
      }
      free(buffer);
      _dirty = false;
      return false;
    }
  }

  if (_client.connected()) {    // Connect to the host
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
 * @debug [Mandatory] bool flag to activate or deactivate debug messages
 */

void Ubidots::setDebug(bool debug) {
  _debug = debug;
}


/**
 * Lets to the user to know if the buffer array has data or not
 */

bool Ubidots::isDirty() {
  return _dirty;
}

/**
 * Lets the user to set the max time to wait for the host answer
 * @timeout [Mandatory] max time to wait in seconds. Default 5000
 */

void Ubidots::setTimeout(int timeout){
  // Changes the max timeout value
  _timeout = timeout;
}

/**
 * Builds the TCP payload to send and saves it to the input char pointer.
 * @payload [Mandatory] char payload pointer to store the built structure.
 * @timestamp_global [Optional] If set, it will be used for any dot without timestamp.
 */

void Ubidots::buildTcpPayload(char* payload, unsigned long timestamp_global) {
  sprintf(payload, "");
  sprintf(payload, "%s/%s|POST|%s|", USER_AGENT, VERSION, _token);
  sprintf(payload, "%s%s:%s", payload, _device_label, _device_name);

  if (timestamp_global != NULL) {
    sprintf(payload, "%s@%lu", payload, timestamp_global);
  }

  sprintf(payload, "%s=>", payload);
  for (uint8_t i = 0; i < _currentValue;) {
    sprintf(payload, "%s%s:%f", payload, (val + i)->variable_label, (val+i)->dot_value);

    if ((val + i)->dot_timestamp != NULL) {
      sprintf(payload, "%s@%lu%s", payload, (val + i)->dot_timestamp, "000");
    }
    if ((val + i)->dot_context != NULL) {
      sprintf(payload, "%s$%s", payload, (val + i)->dot_context);
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

/**
 * Implements a reconnect routine
 */

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

/**
 * Function to waits for the host answer up to the already set _timeout.
 * @return true once the host answer buffer length is greater than zero,
 *         false if timeout is reached.
 */

bool Ubidots::waitServerAnswer() {
  int timeout = 0;
  while(!_client.available() && timeout < _timeout) {
    timeout++;
    delay(1);
    if (timeout > _timeout - 1) {
      if (_debug) {
        Serial.println("timeout, could not read any response from the host");
      }
      return false;
    }
  }
  return true;
}

/**
 * Parse the TCP host answer and saves it to the input char pointer.
 * @payload [Mandatory] char payload pointer to store the host answer.
 * @return true if there is an 'Ok' in the answer, false if not.
 */

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

/**
 * Obtains the remote host's IP
 */

IPAddress Ubidots::getServerIp() {
  IPAddress serverIpAddress;
  HAL_IPAddress ip;
  network_interface_t t;
  serverIpAddress = (inet_gethostbyname(_host, strlen(_host), &ip, t, NULL)<0) ?
       IPAddress(uint32_t(0)) : IPAddress(ip);

  return serverIpAddress;
}
