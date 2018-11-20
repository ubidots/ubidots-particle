/*
Copyright (c) 2013-2018 Ubidots.
Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:
The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Developed and maintained by Jose Garcia for IoT Services Inc
@jotathebest at github: https://github.com/jotathebest
*/

#ifndef _Ubidots_H_
#define _Ubidots_H_

#include "Particle.h"
#include "UbiTypes.h"
#include "UbiConstants.h"
#include "UbiProtocol.h"

class Ubidots {
  public:
    explicit Ubidots(char* token, IotProtocol iot_protocol);
    explicit Ubidots(char* token, UbiServer server=UBI_INDUSTRIAL, IotProtocol iot_protocol=UBI_TCP);
    void add(char *variable_label, float value);
    void add(char *variable_label, float value, char *context);
    void add(char *variable_label, float value, char *context, unsigned long dot_timestamp_seconds);
    void add(char *variable_label, float value, char *context, unsigned long dot_timestamp_seconds, unsigned int dot_timestamp_millis);
    void addContext(char *key_label, char *key_value);
    void getContext(char* context_result);
    bool send();
    bool send(const char* device_label);
    bool send(const char* device_label, const char* device_name);
    bool send(const char* device_label, PublishFlags flags);
    bool send(const char* device_label, const char* device_name, UbiFlags* flags);
    float get(const char* device_label, const char* variable_label);
    void setDebug(bool debug);
  private:
    UbiProtocol * _ubiProtocol;
    const char * _token;
    char* _default_device_label;
    Value * _dots;
    ContextUbi * _context;
    int8_t _current_value=0;
    int8_t _current_context=0;
    bool _dirty=false;
    bool _debug;
    IotProtocol _iot_protocol;
    void buildHttpPayload(char* payload);
    void buildTcpPayload(char* payload, const char* device_label, const char* device_name);
    void builder(char* token, UbiServer server, IotProtocol iot_protocol);
};

#endif