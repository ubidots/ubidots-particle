#include "Particle.h"
#include "UbiHttp.h"

UbiHttp::UbiHttp(const char* host, const int port, const char* user_agent, const char* token){
  _host = host;
  _user_agent = user_agent;
  _token = token;
  _port = port;
}

bool UbiHttp::sendData(const char* device_label, const char* device_name, char* payload){
  reconnect(_host, _port);
  bool result = false;

  if (_client_tcp_ubi.connected()) {  // Connect to the host
    /* Builds the request POST - Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
    _client_tcp_ubi.print(F("POST /api/v1.6/devices/"));
    _client_tcp_ubi.print(device_label);
    _client_tcp_ubi.print(F(" HTTP/1.1\r\n"));
    _client_tcp_ubi.print(F("Host: "));
    _client_tcp_ubi.print(_host);
    _client_tcp_ubi.print(F("\r\n"));
    _client_tcp_ubi.print(F("User-Agent: "));
    _client_tcp_ubi.print(_user_agent);
    _client_tcp_ubi.print(F("\r\n"));
    _client_tcp_ubi.print(F("X-Auth-Token: "));
    _client_tcp_ubi.print(_token);
    _client_tcp_ubi.print(F("\r\n"));
    _client_tcp_ubi.print(F("Connection: close\r\n"));
    _client_tcp_ubi.print(F("Content-Type: application/json\r\n"));
    int content_length = strlen(payload);
    _client_tcp_ubi.print(F("Content-Length: "));
    _client_tcp_ubi.print(content_length);
    _client_tcp_ubi.print(F("\r\n\r\n"));
    _client_tcp_ubi.print(payload);
    _client_tcp_ubi.print(F("\r\n"));

    if (_debug) {
      Serial.println(F("Making request to Ubidots:\n"));
      Serial.print("POST /api/v1.6/devices/");
      Serial.print(device_label);
      Serial.print(" HTTP/1.1\r\n");
      Serial.print("Host: ");
      Serial.print(_host);
      Serial.print("\r\n");
      Serial.print("User-Agent: ");
      Serial.print(_user_agent);
      Serial.print("\r\n");
      Serial.print("X-Auth-Token: ");
      Serial.print(_token);
      Serial.print("\r\n");
      Serial.print("Connection: close\r\n");
      Serial.print("Content-Type: application/json\r\n");
      Serial.print("Content-Length: ");
      Serial.print(content_length);
      Serial.print("\r\n\r\n");
      Serial.print(payload);
      Serial.print("\r\n");
      _client_tcp_ubi.flush();

      Serial.println("waiting for server answer ...");
      waitServerAnswer();
      /* Reads the response from the server */
      Serial.println("\nUbidots' Server response:\n");
      while (_client_tcp_ubi.available()) {
        char c = _client_tcp_ubi.read();
        Serial.print(c);
      }
      result = true;
    }

  } else {
    if (_debug) {
      Serial.println("Could not send data to ubidots using HTTP");
    }
  }

  _client_tcp_ubi.stop();
  return result;

}

/**
 * Reconnects to the server
 * @return true once the host answer buffer length is greater than zero,
 *         false if timeout is reached.
 */

void UbiHttp::reconnect(const char * host, const int port) {
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

bool UbiHttp::waitServerAnswer() {
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
 * Makes available debug traces
 */

void UbiHttp::setDebug(bool debug) {
  _debug = debug;
}