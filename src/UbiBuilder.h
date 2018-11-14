#ifndef _UbiBuilder_H_
#define _UbiBuilder_H_

#include "Particle.h"
#include "UbiProtocol.h"
#include "UbiTypes.h"
#include "UbiConstants.h"
#include <map>

UbiProtocol * builderTcp();
UbiProtocol * builderHttp();
UbiProtocol * builderUdp();
UbiProtocol * builderParticle();

typedef std::function<UbiProtocol * ()> FunctionType;
typedef std::map<IotProtocol, FunctionType> mapProtocol;

namespace {
  const char* _host;
  const char* _token;
}

class UbiBuilder {
  public:
    explicit UbiBuilder(const char* host, const char* token, IotProtocol iotProtocol);
    UbiProtocol * builder();
  private:
    IotProtocol _iot_protocol;
    mapProtocol command_list;
};

#endif