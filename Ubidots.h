#ifndef __Ubidots_H_
#define __Ubidots_H_
#define DEBUG_UBIDOTS

#include "application.h"
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"

#define USER_AGENT  "Ubidots/v1 Particle/1.0"
#define BASE_URL  "things.ubidots.com"
#define ATTEMPS  6



class ubidots {
    public:
        ubidots(char* token);
        void ubidots_init(char * variableName);
        void assemble(char* chain, char* method, char* endpoint);
        void assemble_with_data(char* chain, char* method, char* endpoint, char* data);
        char* get_or_create_datasource();
        char* get_or_create_variable(char* ID, char* variableName);
        bool send(char* chain, char* status, char* body);
        bool check_get_datasource(char* status, char* body, char* datasource);
        bool check_get_variable(char* status, char* body, char* datasource);
        bool send_with_reconect(char* chain, char* status, char* body);
        char* _token;

    private:
        

};
#endif
