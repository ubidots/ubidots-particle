#include "Particle.h"
#include "UbiTcp.h"
#include "UbiConstants.h"

UbiTcp::UbiTcp(const char* host, const int port, const char* user_agent, const char* token){
  _host = host;
  _user_agent = user_agent;
  _token = token;
  _port = port;
}

bool UbiTcp::sendData(const char* device_label, const char* device_name, char* payload){
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
  bool result = parseTcpAnswer(response);
  free(response);
  return result;

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

bool UbiTcp::parseTcpAnswer(char* response) {
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
 * Makes available debug traces
 */

void UbiTcp::setDebug(bool debug) {
  _debug = debug;
}