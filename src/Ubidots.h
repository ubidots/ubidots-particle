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

Original maker Mateo Velez - Metavix for Ubidots Inc
Modified and mainteined by Jose Garcia for Ubidots Inc
*/

#ifndef _Ubidots_H_
#define _Ubidots_H_
#endif

#include "application.h"
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"
#include "inet_hal.h"
#include "Particle.h"

namespace {
  const char * UBIDOTS_SERVER = "industrial.api.ubidots.com";
  const char * USER_AGENT = "UbidotsParticle";
  const char * VERSION = "2.2";
  const int UBIDOTS_HTTP_PORT = 80;
  const int UBIDOTS_TCP_PORT = 9012;
  const int MAX_VALUES = 10;
  const float ERROR_VALUE = -3.4028235E+8;
  const int MAX_BUFFER_SIZE = 700;
}

typedef struct Value {
  char  *variable_label;
  char  *dot_context;
  float dot_value;
  unsigned long dot_timestamp_seconds;
  unsigned int dot_timestamp_millis;
} Value;

class Ubidots {
  public:
  explicit Ubidots(char* token, const char * host=UBIDOTS_SERVER);
  void add(char *variable_id, float value);
  void add(char *variable_id, float value, char *context);
  void add(char *variable_id, float value, char *context, unsigned long dot_timestamp_seconds);
  void add(char *variable_id, float value, char *context, unsigned long dot_timestamp_seconds, unsigned int dot_timestamp_millis);
  float getValue(char* id);
  float getValueWithDatasource(char* device, char* variable);
  float getValueHTTP(char* id);
  char* getVarContext(char* id);
  bool isDirty();
  bool sendValuesTcp();
  bool sendValuesTcp(unsigned long timestamp_global);
  bool sendValuesUdp();
  bool sendValuesHttp();
  void cleanValuesBuffer();
  bool sendValuesWebhook(char* webhook_name);
  bool sendValuesWebhook(char* webhook_name, PublishFlags flags);
  bool sendValuesUdp(unsigned long timestamp_global);
  void setDeviceName(char* device_name);
  void setDeviceLabel(char* device_label);
  void setDebug(bool debug);
  void setTimeout(int timeout);

 private:
  TCPClient _client_tcp_ubi;
  UDP _client_udp_ubi;
  Value * val;
  uint8_t _current_value;
  char* _device_name;
  bool _debug = false;
  bool _dirty = false;
  char* _device_label;
  const char * _host;
  char* _token;
  int _timeout = 5000;
  void buildTcpPayload(char* payload, unsigned long timestamp_global);
  void buildHttpPayload(char* payload);
  void reconnect();  // Default method to reconnect to both UDP and TCP server
  void reconnect(const char * host, int port);
  bool waitServerAnswer();
  bool parseTcpAnswer(char* response);
  bool sendUdpPacket();
  IPAddress getServerIp();
};
