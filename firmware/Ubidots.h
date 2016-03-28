#ifndef __Ubidots_H_
#define __Ubidots_H_
#define DEBUG_UBIDOTS

#include "application.h"
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"
#include <stdarg.h>

#define USER_AGENT  "Ubidots/v1 Particle/1.0"
#define BASE_URL  "things.ubidots.com"
#define ATTEMPS  3
#define BUFFER_HTTP_SIZE 1500


typedef struct Value {
    char *name;
    float value;
    char *id;
    Value *next;
} Value;

typedef struct UbidotsCollection {
    char * idDatasourceDefault;
    Value *first;
} UbidotsCollection;


class Ubidots {
 private:
    TCPClient _client;
    UbidotsCollection * cache;
    char* pId;
    char* _token;
    char* dsName;

    Value * checkInitValue(UbidotsCollection *collection, char* name, double value, char * id);
    char* assemble(char* method, char* endpoint);
    char* assembleWithData(char* method, char* endpoint, char* data);
    char* parserId(char* status, char* body);    
    char* parserValue(char* status, char* body);
    
 public:
    Ubidots(char* token);
    Ubidots(char* token, char* datasourceName);
    char* getOrCreateDatasource(char* ds_name);
    char* getOrCreateVariable(char* dsId, char* variableName);
    bool send(char* chain, char* status, char* body, unsigned int size);
    bool sendWithReconnect(char* chain, char* status, char* body, unsigned int size);        
    int ubidotsCollectionSave(UbidotsCollection *collection);        
    void addValueWithName(UbidotsCollection *collection, char * name, double value);
    void addValue(UbidotsCollection *collection, char * variableId, double value);    
    bool sendUbidots(int number, ... );
    float getUbidotsValue(char* id);
    
};
#endif
