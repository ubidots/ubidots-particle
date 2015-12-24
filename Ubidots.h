#ifndef __Ubidots_H_
#define __Ubidots_H_

#include "application.h"
#include "spark_wiring_string.h"
#include "spark_wiring_tcpclient.h"
#include "spark_wiring_usbserial.h"

#define USER_AGENT  "Ubidots/v1 Particle/1.0"
#define BASE_URL  "things.ubidots.com"
#define ATTEMPS  6



class Ubidots {
    public:
        Ubidots(char* token);
        void assemble(char* chain, char* method, char* endpoint);
        void assemble_with_data(char* chain, char* method, char* endpoint, char* data);
        bool get_or_create_datasource();
        bool Send(char* chain, char* status, char* body);
        bool  check_get_datasource(char* status, char* body, char* datasource);
        bool Send_with_reconect(char* chain, char* status, char* body);
        char* _token;

    private:
        

};
#endif
