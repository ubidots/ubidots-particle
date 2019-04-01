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

Developed and maintained by Jose Garcia for Ubidots Inc
@jotathebest at github: https://github.com/jotathebest
*/

#include "UbiUdp.h"
#include "Particle.h"
#include "UbiConstants.h"

/**************************************************************************
 * Overloaded constructors
 ***************************************************************************/

UbiUDP::UbiUDP(const char *host, const int port, const char *user_agent,
               const char *token) {
  _host = host;
  _user_agent = user_agent;
  _token = token;
  _port = port;
}

/**************************************************************************
 * Destructor
 ***************************************************************************/

UbiUDP::~UbiUDP() {
  delete[] _host;
  delete[] _user_agent;
  delete[] _token;
  _client_udp_ubi.flush();
  _client_udp_ubi.stop();
}

bool UbiUDP::sendData(const char *device_label, const char *device_name,
                      char *payload, UbiFlags *flags) {
  /* Obtains the remote host's IP */
  IPAddress serverIpAddress = getServerIp();

  if (!serverIpAddress) {
    if (_debug) {
      Serial.println(
          "ERROR, could not solve IP Address of the remote host, please check "
          "your DNS setup");
    }
    _client_udp_ubi.stop();
    return false;
  }

  /* Sends data to Ubidots */
  _client_udp_ubi.setBuffer(MAX_BUFFER_SIZE +
                            1);  // Sets the max buffer size to send data
  _client_udp_ubi.begin(UBIDOTS_TCP_PORT);
  if (!(_client_udp_ubi.beginPacket(serverIpAddress, UBIDOTS_TCP_PORT) &&
        _client_udp_ubi.write(payload) && _client_udp_ubi.endPacket())) {
    if (_debug) {
      Serial.println("ERROR sending values with UDP");
    }
    _client_udp_ubi.stop();
    return false;
  }

  _client_udp_ubi.stop();
  return true;
}

float UbiUDP::get(const char *device_label, const char *variable_label) {
  return ERROR_VALUE;
}

/**
 * Makes available debug traces
 */

void UbiUDP::setDebug(bool debug) { _debug = debug; }

/**
 * Obtains the remote host's IP
 */

IPAddress UbiUDP::getServerIp() {
  IPAddress serverIpAddress;
#if (PLATFORM_ID == 12 || PLATFORM_ID == 6)  // Argon(12), Photon(6)
  serverIpAddress = WiFi.resolve(_host);
  return serverIpAddress;
#elif (PLATFORM_ID == 13 || PLATFORM_ID == 10)  // Boron(13), Electron(10)
  serverIpAddress = Cellular.resolve(_host);
  return serverIpAddress;
#else
  HAL_IPAddress ip;
  network_interface_t t;
  serverIpAddress =
      (inet_gethostbyname(_host, strlen(_host), &ip, t, NULL) == 0)
          ? IPAddress(uint32_t(0))
          : IPAddress(ip);
  return serverIpAddress;
#endif

  return serverIpAddress;
}
