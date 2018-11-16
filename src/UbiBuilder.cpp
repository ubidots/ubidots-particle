#include "UbiProtocol.h"
#include "UbiTcp.h"
#include "UbiHttp.h"
#include "UbiUdp.h"
#include "UbiParticle.h"
#include "UbiBuilder.h"
#include "UbiTypes.h"
#include "UbiConstants.h"

UbiBuilder::UbiBuilder(const char* host, const char* token, IotProtocol iot_protocol) {
  _iot_protocol = iot_protocol;
  command_list[UBI_TCP]=&builderTcp;
  command_list[UBI_HTTP]=&builderHttp;
  command_list[UBI_UDP]=&builderUdp;
  command_list[UBI_PARTICLE]=&builderParticle;
  _host = host;
  _token = token;
}

UbiProtocol * UbiBuilder::builder() {
  mapProtocol::iterator i=command_list.find(_iot_protocol);
  UbiProtocol * ubiBuilder = (i->second)();
  return ubiBuilder;
}

UbiProtocol * builderTcp() {
  UbiProtocol * tcpInstance = new UbiTcp(_host, UBIDOTS_TCP_PORT, USER_AGENT, _token);
  return tcpInstance;
}

UbiProtocol * builderHttp() {
  UbiProtocol * httpInstance = new UbiHttp(_host, UBIDOTS_HTTP_PORT, USER_AGENT, _token);
  return httpInstance;
}

UbiProtocol * builderUdp() {
  UbiProtocol * udpInstance = new UbiUdp(_host, UBIDOTS_TCP_PORT, USER_AGENT, _token);
  return udpInstance;
}

UbiProtocol * builderParticle() {
  UbiProtocol * particleInstance = new UbiParticle(_host, NULL, NULL, _token);
  return particleInstance;
}