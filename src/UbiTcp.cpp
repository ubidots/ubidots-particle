#include "Particle.h"
#include "UbiTcp.h"
#include "UbiConstants.h"

UbiTcp::UbiTcp(const char* host, const int port, const char* user_agent, const char* token){
  _host = host;
  _user_agent = user_agent;
  _token = token;
  _port = port;
}

bool UbiTcp::sendData(const char* device_label, const char* device_name, char* payload, Ubi_flags* flags){
  /* Makes sure that the client is connected to Ubidots */
  _client_tcp_ubi.connect(_host, UBIDOTS_TCP_PORT);
  reconnect(_host, UBIDOTS_TCP_PORT);

  /* Sends data to Ubidots */
  if (_client_tcp_ubi.connected()) {
    _client_tcp_ubi.print(payload);
  } else {
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

  float value = parseTcpAnswer("POST", response);
  free(response);
  if (value != ERROR_VALUE){
    return true;
  }

  return false;

}

float UbiTcp::get(const char* device_label, const char* variable_label) {
  /* Connecting the client */
  _client_tcp_ubi.connect(_host, UBIDOTS_TCP_PORT);
  reconnect(_host, UBIDOTS_TCP_PORT);

  if (_client_tcp_ubi.connected()) {
    /* Builds the request POST - Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
    _client_tcp_ubi.print(_user_agent);
    _client_tcp_ubi.print("|LV|");
    _client_tcp_ubi.print(_token);
    _client_tcp_ubi.print("|");
    _client_tcp_ubi.print(device_label);
    _client_tcp_ubi.print(":");
    _client_tcp_ubi.print(variable_label);
    _client_tcp_ubi.print("|end");

    if (_debug){
      Serial.println("----");
      Serial.println("Payload for request:");
      Serial.print(_user_agent);
      Serial.print("|LV|");
      Serial.print(_token);
      Serial.print("|");
      Serial.print(device_label);
      Serial.print(":");
      Serial.print(variable_label);
      Serial.print("|end");
      Serial.println("\n----");
    }

    /* Waits for the host's answer */
    if (!waitServerAnswer()) {
      return ERROR_VALUE;
    }

    /* Reads the response from the server */
    char* response = (char *) malloc(sizeof(char) * MAX_BUFFER_SIZE);
    float value = parseTcpAnswer("LV", response);
    free(response);
    return value;
  }

  if (_debug) {
    Serial.println("ERROR could not connect to the server");
  }

  return ERROR_VALUE;
}

/**
 * Reconnects to the server
 * @return true once the host answer buffer length is greater than zero,
 *         false if timeout is reached.
 */

void UbiTcp::reconnect(const char * host, const int port) {
  uint8_t attempts = 0;
  while (!_client_tcp_ubi.connected() && attempts < 5) {
    if (_debug) {
      Serial.print("Trying to connect to ");
      Serial.print(host);
      Serial.print(" , attempt number: ");
      Serial.println(attempts);
    }
    _client_tcp_ubi.connect(host, port);
    attempts += 1;
    delay(1000);
  }
}

/**
 * Function to wait for the host answer up to the already set _timeout.
 * @return true once the host answer buffer length is greater than zero,
 *         false if timeout is reached.
 */

bool UbiTcp::waitServerAnswer() {
  int timeout = 0;
  while(!_client_tcp_ubi.available() && timeout < _timeout) {
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

float UbiTcp::parseTcpAnswer(const char* request_type, char* response) {
  int j = 0;

  if (_debug){
    Serial.println("----------");
    Serial.println("Server's response:");
  }

  while (_client_tcp_ubi.available()) {
    char c = _client_tcp_ubi.read();
    if (_debug) {
      Serial.write(c);
    }
    response[j] = c;
    j++;
    if (j >= MAX_BUFFER_SIZE - 1) {
      break;
    }
  }

  if (_debug){
    Serial.println("\n----------");
  }

  response[j] = '\0';
  float result = ERROR_VALUE;

  // POST
  if (request_type == "POST") {
    char *pch = strstr(response, "OK");
    if (pch != NULL) {
      result = 1;
    }
    return result;
  }

  // LV
  char *pch = strchr(response, '|');
  if (pch != NULL) {
    result = atof(pch + 1);
  }

  return result;
}

/**
 * Makes available debug traces
 */

void UbiTcp::setDebug(bool debug) {
  _debug = debug;
}