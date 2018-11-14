#ifndef _UbiTcp_H_
#define _UbiTcp_H_

#include "UbiProtocol.h"

class UbiTcp : public UbiProtocol {
  public:
    UbiTcp(const char* host, const int port, const char* user_agent, const char* token);
    bool sendData(const char* device_label, const char* device_name, char* payload, Ubi_flags* flags);
    float get(const char* device_label, const char* variable_label);
    void setDebug(bool debug);
  private:
    const char *_host;
    const char *_user_agent;
    const char *_token;
    int _port;
    bool _debug = false;
    TCPClient _client_tcp_ubi;
    int _timeout = 5000;
    bool waitServerAnswer();
    float parseTcpAnswer(const char* request_type, char* response);
    void reconnect(const char * host, const int port);
};

#endif