#include "Particle.h"
#include "UbiParticle.h"

UbiParticle::UbiParticle(const char* host, const int port, const char* user_agent, const char* token){
  _host = host;
  _user_agent = user_agent;
  _token = token;
}

bool UbiParticle::sendData(const char* device_label, const char* device_name, char* payload){
  return true;
}

void UbiParticle::setDebug(bool debug){
  _debug = debug;
}
