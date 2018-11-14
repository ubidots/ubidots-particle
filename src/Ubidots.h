#ifndef _Ubidots_H_
#define _Ubidots_H_


#include "Particle.h"
#include "UbiTypes.h"
#include "UbiConstants.h"
#include "UbiProtocol.h"

class Ubidots {
  public:
    explicit Ubidots(char* token, IotProtocol iot_protocol=UBI_PARTICLE);
    void add(char *variable_id, float value);
    void add(char *variable_id, float value, char *context);
    void add(char *variable_id, float value, char *context, unsigned long dot_timestamp_seconds);
    void add(char *variable_id, float value, char *context, unsigned long dot_timestamp_seconds, unsigned int dot_timestamp_millis);
    bool send();
    bool send(const char* device_label);
    bool send(const char* device_label, const char* device_name);
    bool send(const char* device_label, PublishFlags flags);
    bool send(const char* device_label, const char* device_name, Ubi_flags* flags);
    void setDebug(bool debug);
  private:
    UbiProtocol * _ubiProtocol;
    const char * _token;
    char* _default_device_label;
    Value * _dots;
    int8_t _current_value=0;
    bool _dirty=false;
    bool _debug;
    IotProtocol _iot_protocol;
    void buildHttpPayload(char* payload);
    void buildTcpPayload(char* payload, const char* device_label, const char* device_name);
};

#endif