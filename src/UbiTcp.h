/*
Copyright (c) 2013-2019 Ubidots.
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

#ifndef _UbiTcp_H_
#define _UbiTcp_H_

#include <map>

#include "UbiConstants.h"
#include "UbiProtocol.h"

static TCPClient _client_tcp_ubi;

class UbiTCP : public UbiProtocol {
 public:
  UbiTCP(const char* host, const int port, const char* user_agent, const char* token);
  bool sendData(const char* device_label, const char* device_name, char* payload, UbiFlags* flags);
  float get(const char* device_label, const char* variable_label);
  tcpMap getMultipleValues(const char* deviceLabel, const char* variableLabels);
  void setDebug(bool debug);
  ~UbiTCP();

 private:
  const char* _host;
  const char* _user_agent;
  const char* _token;
  int _port;
  bool _debug = false;
  int _timeout = 5000;
  bool waitServerAnswer();
  void buildAndSendGetPacket(const char* device_label, const char* variable_label);
  float parseTCPAnswer(const char* request_type, char* response);
  tcpMap parseMultipleValues();
  void reconnect(const char* host, const int port);
};

#endif
