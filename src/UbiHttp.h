#ifndef _UbiHttp_H_
#define _UbiHttp_H_

#include "Particle.h"
#include "UbiProtocol.h"

class UbiHttp : public UbiProtocol {
  public:
    UbiHttp(const char* host, const int port, const char* user_agent, const char* token);
    bool sendData(const char* device_label, const char* device_name, char* payload);
    void setDebug(bool debug);
  private:
    const char *_host;
    const char *_user_agent;
    const char *_token;
    int _port;
    bool _debug = false;
    bool waitServerAnswer();
    void reconnect(const char * host, int port);
    TCPClient _client_tcp_ubi;
    int _timeout = 5000;
};

#endif