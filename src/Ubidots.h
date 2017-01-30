/*
Copyright (c) 2013-2016 Ubidots.

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
Modified by Jose Garcia for Ubidots Inc
*/

#ifndef _Ubidots_H_
#define _Ubidots_H_

#include "application.h"
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"

#define SERVER "translate.ubidots.com"
#define TIME_SERVER "pool.ntp.org"
#define USER_AGENT "Particle"
#define VERSION "2.1.9"
#define PORT 9012
#define MAX_VALUES 10
#define TYPE_SMS 0
#define TYPE_TCP 1
#define TYPE_UDP 2
//#define DEBUG_UBIDOTS
#define TIMEOUT 10000
#define SERVERHTTP "things.ubidots.com"
#define PORTHTTP 80


typedef struct Value {
    char  *idName;
    char  *contextOne;
    float idValue;
    unsigned long timestamp_val;
} Value;

class Ubidots {
 public:
    explicit Ubidots(char* token, char* server = SERVER);
    bool setDatasourceName(char* dsName);
    bool setDatasourceTag(char* dsTag);
    void setMethod(uint8_t method);  // Default UDP
    bool sendAll();
    bool sendAll(unsigned long timestamp_global);
    float getValue(char* id);
    void add(char *variable_id, double value);
    void add(char *variable_id, double value, char *ctext);
    void add(char *variable_id, double value, char *ctext, unsigned long timestamp);
    float getValueWithDatasource(char* dsName, char* idName);
    unsigned long ntpUnixTime();

 private:
    TCPClient _client;
    UDP _clientUDP;
    UDP _clientTMP;
    uint8_t _method;
    char* _server;
    char* _token;
    char* _pId;
    char* _dsName;
    uint8_t maxValues;
    uint8_t currentValue;
    Value * val;
    bool sendAllUDP(char* buffer);
    bool sendAllTCP(char* buffer);
    float lastValue;
    //bool sendAllSMS(char* buffer);  // Work in progress
};

#endif  // _Ubidots_H_
