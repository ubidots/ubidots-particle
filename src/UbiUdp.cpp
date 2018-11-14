#include "Particle.h"
#include "UbiUdp.h"
#include "UbiConstants.h"

UbiUdp::UbiUdp(const char* host, const int port, const char* user_agent, const char* token){
  _host = host;
  _user_agent = user_agent;
  _token = token;
  _port = port;
}

bool UbiUdp::sendData(const char* device_label, const char* device_name, char* payload, Ubi_flags* flags){
  /* Obtains the remote host's IP */
  IPAddress serverIpAddress = getServerIp();

  if (! serverIpAddress){
    if (_debug) {
      Serial.println("ERROR, could not solve IP Address of the remote host, please check your DNS setup");
    }
    _client_udp_ubi.stop();
    return false;
  }

  /* Sends data to Ubidots */
  _client_udp_ubi.begin(UBIDOTS_TCP_PORT);
  if (! (_client_udp_ubi.beginPacket(serverIpAddress, UBIDOTS_TCP_PORT)
      && _client_udp_ubi.write(payload)
      && _client_udp_ubi.endPacket())) {
    if (_debug) {
      Serial.println("ERROR sending values with UDP");
    }
    _client_udp_ubi.stop();
    return false;
  }

  _client_udp_ubi.stop();
  return true;
}

float UbiUdp::get(const char* device_label, const char* variable_label) {
  return ERROR_VALUE;
}

/**
 * Makes available debug traces
 */

void UbiUdp::setDebug(bool debug) {
  _debug = debug;
}

/**
 * Obtains the remote host's IP
 */

IPAddress UbiUdp::getServerIp() {
  IPAddress serverIpAddress;
  HAL_IPAddress ip;
  network_interface_t t;
  serverIpAddress = (inet_gethostbyname(_host, strlen(_host), &ip, t, NULL)<0) ?
       IPAddress(uint32_t(0)) : IPAddress(ip);

  return serverIpAddress;
}
