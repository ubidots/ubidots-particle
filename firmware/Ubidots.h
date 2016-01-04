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
#define ATTEMPS  6


/*
  typedef struct UbidotsCollection {
  int n;
  int i;
  char  **variable_ids;
  float *values;
  char  *_ids;
  } UbidotsCollection;
*/

typedef struct Value {
    char *name;
    float value;
    char *id;
    Value *next;
} Value;


typedef struct UbidotsCollection {
    Value *first;
} UbidotsCollection;


//typedef tipoNodo *pNodo;
//typedef tipoNodo *Lista;

class ubidots {
 public:
    ubidots(char* token);
    char* get_or_create_datasource();
    char* get_or_create_variable(char* ID, char* variableName);
    bool send(char* chain, char* status, char* body);
    char* parser_id(char* status, char* body);
    bool send_with_reconect(char* chain, char* status, char* body);
    Value * init_value(char* name, double value, char * id);
        
    int ubidots_collection_save(UbidotsCollection *collection);
    void ubidots_collection_cleanup(UbidotsCollection *collection);
        
    void add_value_with_name(UbidotsCollection *collection, char * name, double value);
    void add_value(UbidotsCollection *collection, char * variable_id, double value);
        
        
    bool send_ubidots(int number, ... );
    char* _token;

 private:
    void assemble(char* chain, char* method, char* endpoint);
    void assemble_with_data(char* chain, char* method, char* endpoint, char* data);
    int number_cache;
    UbidotsCollection * cache;
    char* pch;
        

};
#endif
