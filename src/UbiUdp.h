#ifndef _UbiUdp_H_
#define _UbiUdp_H_

#include "UbiProtocol.h"

class UbiUdp : public UbiProtocol {
  public:
    UbiUdp(const char* host, const int port, const char* user_agent, const char* token);
    bool sendData(const char* device_label, const char* device_name, char* payload);
    void setDebug(bool debug);
  private:
    const char *_host;
    const char *_user_agent;
    const char *_token;
    int _port;
    bool _debug = false;
};

#endif