/*
ISC License

Copyright (c) 2016, Mateo Velez Rodriguez - Metavix

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

*/

#ifndef _Ubidots_H_
#define _Ubidots_H_

#include "application.h"
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"


#define SERVER "translate.ubidots.com"
#define USER_AGENT "Particle/1.0"
#define PORT 9010
#define MAX_VALUES 10

typedef struct Value {
  char  *idName;
  char  *contextOne;
  float idValue;
} Value;

class Ubidots {
 public:
      Ubidots(char* token);
      bool setDatasourceName(char* dsName);
      bool setDatasourceTag(char* dsTag);
      bool sendAll();
      float getValue(char* id);
      void add(char *variable_id, double value);
      void add(char *variable_id, double value, char *ctext1);
      float getValueWithDatasource(char* dsName, char* idName);
 private:
      TCPClient _client;
      char* _token;
      char* _pId;
      char* _dsName;
      uint8_t maxValues;
      uint8_t currentValue;
      Value * val;
};

#endif  // _Ubidots_H_