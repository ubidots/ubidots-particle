#ifndef _UbiParticle_H_
#define _UbiParticle_H_

#include "UbiProtocol.h"

class UbiParticle : public UbiProtocol {
  public:
    UbiParticle(const char* host, const int port, const char* user_agent, const char* token);
    bool sendData(const char* device_label, const char* device_name, char* payload, Ubi_flags* flags);
    void setDebug(bool debug);
  private:
    const char *_host;
    const char *_user_agent;
    const char *_token;
    bool _debug = false;
};

#endif