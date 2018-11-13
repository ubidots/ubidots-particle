#include "Particle.h"
#include "UbiUdp.h"

UbiUdp::UbiUdp(const char* host, const int port, const char* user_agent, const char* token){
  _host = host;
  _user_agent = user_agent;
  _token = token;
  _port = port;
}

bool UbiUdp::sendData(const char* device_label, const char* device_name, char* payload){
  return true;
}

/**
 * Makes available debug traces
 */

void UbiUdp::setDebug(bool debug) {
  _debug = debug;
}
