#ifndef _Ubidots_H_
#define _Ubidots_H_

#include "application.h"
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"

#define SERVER "translate.ubidots.com"
#define PORT 9010

typedef struct Value {
  char  *idName;
  char  *contextOne;
  char  *contextTwo;
  float idValue;
} Value;

class Ubidots {
 public:
      Ubidots(char* token);
      bool sendAll();
      bool sendAll(char* dsName);
      float getValue(char* id);
      void add(char *variable_id, double value);
      void add(char *variable_id, double value, char *ctext1);
      void add(char *variable_id, double value, char *ctext1, char *ctext2);
      float getValueWithDatasource(char* dsName, char* idName);

 private:
      TCPClient _client;
      char* _token;
      uint8_t maxValues;
      uint8_t currentValue;
      Value * val;
};

#endif  // _Ubidots_H_
