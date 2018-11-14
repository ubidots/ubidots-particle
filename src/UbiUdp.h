#ifndef _UbiUdp_H_
#define _UbiUdp_H_

#include "UbiProtocol.h"
#include "Particle.h"

class UbiUdp : public UbiProtocol {
  public:
    UbiUdp(const char* host, const int port, const char* user_agent, const char* token);
    bool sendData(const char* device_label, const char* device_name, char* payload, Ubi_flags* flags);
    float get(const char* device_label, const char* variable_label);
    void setDebug(bool debug);
  private:
    const char *_host;
    const char *_user_agent;
    const char *_token;
    int _port;
    bool _debug = false;
    UDP _client_udp_ubi;
    int _timeout = 5000;
    bool waitServerAnswer();
    bool parseTcpAnswer(char* response);
    void reconnect(const char * host, const int port);
    IPAddress getServerIp();
};

#endif