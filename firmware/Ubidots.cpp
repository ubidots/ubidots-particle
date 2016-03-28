#include "Ubidots.h"
static const uint16_t TIMEOUT = 10000;
/**
 * Constructor.
 */

Ubidots::Ubidots(char* token){
    Ubidots(token, NULL);
}
 
Ubidots::Ubidots(char* token, char* datasourceName){
    _token = token;
    cache = (UbidotsCollection *) malloc(sizeof(UbidotsCollection));
    cache->first = NULL;
    cache->idDatasourceDefault = NULL;
    dsName = datasourceName;    
    String str = Particle.deviceID();
    pId = new char [str.length()+1];
    strcpy (pId, str.c_str());
}

/** 
 * This function is to initialize the cache 
 * @arg collection, is the cache struct
 * @arg name is the name of the variable that you will save
 * @arg value is the value of the variable that you will save
 * @arg id is the id of the variable that you will add 
 */
Value * Ubidots::checkInitValue(UbidotsCollection *collection, char* name, double value, char * id){
    Value * newValue = (Value *)malloc(sizeof(Value));
    newValue->name = name;
    newValue->value = value;
    newValue->next = NULL;
    if (id == NULL){
        if(collection->idDatasourceDefault==NULL){
        }
            collection->idDatasourceDefault = getOrCreateDatasource(dsName);
        newValue->id = getOrCreateVariable(collection->idDatasourceDefault, name);
        Serial.println(newValue->id);
        
    }else{
        newValue->id = id;
    }
#ifdef DEBUG_UBIDOTS
    Serial.print("Variable id: ");
    Serial.println(newValue->id);
#endif
    return newValue;
}
/** 
 * This function is add value with name to the cache (for normal users) 
 * @arg collection, is the cache struct
 * @arg name is the name of the variable that you will save
 * @arg value is the value of the variable that you will save
 */
void Ubidots::addValueWithName(UbidotsCollection *collection, char * name, double value){
    if(cache->first == NULL){
        cache->first = checkInitValue(collection, name, value, NULL); 
    }else{
        Value * nod = cache->first;
        int cmp = strcmp(name, nod->name);
        while(nod->next || cmp == 0) {
            if (cmp == 0){
                nod->value = value;
                return;
            }
            nod = nod->next;
            cmp = strcmp(name, nod->name);
        }
        nod->next = checkInitValue(collection, name, value, NULL);
    }
}
/** 
 * This function is add value to the cache (for experts users with ID) 
 * @arg collection, is the cache struct
 * @arg variableId is the id of the variable that you will add
 * @arg value is the value of the variable that you will save
 */
void Ubidots::addValue(UbidotsCollection *collection, char * variableId, double value){
    if(cache->first == NULL){
        cache->first = checkInitValue(collection, NULL, value, variableId); 
    }else{
        Value *nod = cache->first;
        while(nod->next || strcmp(variableId, nod->id) == 0) {
            if (variableId == nod->id){
                nod->value = value;
                return;
                
            }
            nod = nod->next;
        }
        nod->next = checkInitValue(collection, NULL, value, variableId);
    }
}
/** 
 * This function is to send infinite variables to the API (photon only support 10 variables)
 * @arg number is the quantity of variables that you will send
 * @return true upon success
 */
bool Ubidots::sendUbidots( int number, ... ){
    va_list vl;
    int i;
    va_start( vl, number );
    for( i = 0; i< number; ++i ){
        addValueWithName(cache, va_arg( vl, char* ), va_arg( vl, double ));
    }
    ubidotsCollectionSave(cache);
    return true;
}
/** 
 * This function is to get value of a variable id from Ubidots API
 * @arg id is the quantity of variables that you will send
 * @return value float value from the API
 */
float Ubidots::getUbidotsValue(char* id){
    char* endpoint= (char *) malloc(sizeof(char) * 100);
    char* status = (char *) malloc(sizeof(char) * 3);
    char* body = (char *) malloc(sizeof(char) * 500);
    char* chain;
    char* value; 
    sprintf(endpoint, "variables/%s/values", id);
    chain = assemble("GET", endpoint);
    free(endpoint);
    if(!sendWithReconnect(chain, status, body, 500)){
#ifdef DEBUG_UBIDOTS
        Serial.print("sendWithReconnect failure 3 times,\n\r maybe you have problem with your connection");
#endif
        free(chain);
        free(status);
        free(body);
        return NULL;
    }
    free(chain);
    free(status);
    free(body);
    value = parserValue(status, body);
    free(value);
    float n = atof(chain);
    return n;
}
/**
 * This function is to assemble the collection data
 * @arg collection is the Collection to save.
 * @reutrn Zero upon success, non-zero upon error.
 */
int Ubidots::ubidotsCollectionSave(UbidotsCollection *collection){
    char* endpoint= (char *) malloc(sizeof(char) * 100);
    char* data = (char *) malloc(sizeof(char) * 800);
    char* status = (char *) malloc(sizeof(char) * 3);
    char* body = (char *) malloc(sizeof(char) * 200);
    char *chain;
    sprintf(data, "[");
    Value *nod = cache->first;
    int count = 0;
    while(nod) {
        if(nod->value){
            sprintf(data, "%s%s{\"variable\": \"%s\", \"value\":\"%f\"} ", data, (count>0) ? "," : " ", nod->id , nod->value);
            count++;
        }
        nod->value = NULL;
        nod = nod->next;
    }
    sprintf(data, "%s]", data);
    nod->value = NULL;
    chain = assembleWithData("POST", "collections/values/?force=true", data);
#ifdef DEBUG_UBIDOTS
    Serial.print("Your data of body that you will send to ubidots is:\n");
    Serial.println(data);
#endif
    if(!sendWithReconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("sendWithReconnect fail 6 times, probably\n you have connection problem with your internet");
#endif
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return 1;
    }
    free(chain);
    free(endpoint);
    free(data);
    free(status);
    free(body);
    return 0;
}

/** 
 * This function is to assemble the data to send to Ubidots
 * @arg chain This array is to save all data to send to the API 
 * @arg method   This array contains GET or POST method
 * @arg endpoint  This array contains the endpoint to send to the API
 */
char* Ubidots::assemble(char* method, char* endpoint){
    char* chain = (char *) malloc(sizeof(char) * 300);
    sprintf(chain, "%s /api/v1.6/%s HTTP/1.1\nHost: %s\nUser-Agent: %s \nX-Auth-Token: %s\nConnection: close", method, endpoint, BASE_URL, USER_AGENT, _token);
#ifdef DEBUG_UBIDOTS
    Serial.println("Assemble chain: ");
    Serial.println(chain);
#endif
return chain;
}
/** 
 * This function is to assemble the data with length and value of variable
 * to send to Ubidots
 * @arg chain This array is to save all data to send to the API 
 * @arg method   This array contains GET or POST method
 * @arg endpoint  This array contains the endpoint to send to the API
 * @arg data  This array contains the value to POST to the Ubidots API
 */
char* Ubidots::assembleWithData(char* method, char* endpoint, char* data){
    char* chain = (char *) malloc(sizeof(char) * 1000);
    char* chain_dos = assemble(method, endpoint);
    sprintf(chain,"%s\nContent-Type: application/json\nContent-Length:  %d\n\n%s\n",chain_dos , strlen(data), data);
#ifdef DEBUG_UBIDOTS
    Serial.println("assembleWithData chain: ");
    Serial.println(chain);
#endif
    free(chain_dos);
    return chain;
}
/**
 * This function is to get or post datasource
 * @return datasource ID upon succes or NULL in bad connection
 */
char* Ubidots::getOrCreateDatasource(char* ds_name){
    char* endpoint= (char *) malloc(sizeof(char) * 100);
    char* data = (char *) malloc(sizeof(char) * 100);
    char* status = (char *) malloc(sizeof(char) * 3);
    char* body = (char *) malloc(sizeof(char) * 200);
    char *datasource;
    char *chain;
    if(ds_name!= NULL){
        sprintf(endpoint, "datasources/?tag=%s", ds_name);
    }else{
        sprintf(endpoint, "datasources/?tag=%s", pId);
    }
    
    chain = assemble("GET", endpoint);
    if(!sendWithReconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("sendWithReconnect fail 3 times, probably\n\r you have connection problem with your internet");
#endif
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return NULL;
    }
    datasource = parserId(status, body);
    if(datasource==NULL && strstr(body,"\"count\": 0")!=NULL){
        sprintf(data, "{\"name\": \"Particle\",\"tags\":[\"%s\"]}", pId);
        chain = assembleWithData("POST", endpoint, data);
        if(!sendWithReconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("sendWithReconnect fail 3 times, probably\n\r you have connection problem with your internet");
#endif  
        free(datasource);
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return NULL;
        }
        datasource = parserId(status, body);
    }
    free(chain);
    free(endpoint);
    free(data);
    free(status);
    free(body);
    return datasource;
}
/**
 * This function is to know the existence of variable
 * in the API
 * @arg ID This array contains the datasource ID
 * @arg variableName  This contains the variable name 
 * @return variable ID or NULL in bad connection
 */
char* Ubidots::getOrCreateVariable(char* dsId, char* variableName){
    char* endpoint= (char *) malloc(sizeof(char) * 100);
    char* data = (char *) malloc(sizeof(char) * 100);
    char* status = (char *) malloc(sizeof(char) * 3);
    char* body = (char *) malloc(sizeof(char) * 200);
    char *variable;
    char *chain;
    sprintf(endpoint, "datasources/%s/variables/?tag=%s", dsId, variableName);
    chain = assemble((char *)"GET",(char *) endpoint); // send core id and check if it is living
    if(!sendWithReconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("sendWithReconnect fail 3 times, probably\n\r you have connection problem with your internet");
#endif
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return NULL;
    }
    variable = parserId(status, body);
    if(variable==NULL && strstr(body,"\"count\": 0")!=NULL){
        sprintf(data, "{\"name\": \"%s\",\"tags\":[\"%s\"]}", variableName, variableName);
        chain = assembleWithData("POST", endpoint, data);
        if(!sendWithReconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("sendWithReconnect fail 3 times, probably\n\r you have connection problem with your internet");
#endif  
        free(variable);
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return NULL;
        }
        variable = parserId(status, body);
    }
    free(chain);
    free(endpoint);
    free(data);
    free(status);
    free(body);
    return variable;   
}
/**
 * This function is to parser IDs of datasource or variables
 * @arg Status This array contains the connection status of
 * the API 
 * @arg body   This array contains the body of the API
 * @return the ID upon succes or NULL when it's fail
 */
char* Ubidots::parserId(char* status, char* body){
    String rawResponse(body);
    int bodyPos = rawResponse.indexOf("\"id\": ");
    if((strstr(status, "200")!=NULL || strstr(status, "201")!=NULL) && strstr(body, "\"id\": ")!=NULL && strstr(body,"\"count\": 0")==NULL){
        char* ID = (char *) malloc(sizeof(char) * 24);
        rawResponse.substring(bodyPos+7).toCharArray(ID, 25);
#ifdef DEBUG_UBIDOTS
        Serial.print("Your id to return in paser_id is: ");
        Serial.println(ID);
#endif
        return ID;
    }else{
        return NULL;
    }
}
/**
 * This function is to parser value of  variables
 * @arg Status This array contains the connection status of
 * the API 
 * @arg body   This array contains the body of the API
 * @return the ID upon succes or NULL when it's fail
 */
char* Ubidots::parserValue(char* status, char* body){
    String rawResponse(body);
    int bodyPosini = 9 + rawResponse.indexOf("\"value\": ");
    int bodyPosend = rawResponse.indexOf(", \"timestamp\"");
    if(strstr(status, "200")!=NULL || strstr(status, "201")!=NULL){
        char* value = (char *) malloc(sizeof(char) * 24);
        rawResponse.substring(bodyPosini,bodyPosend).toCharArray(value, 10);
#ifdef DEBUG_UBIDOTS
        Serial.print("Your id to return in paser_id is: ");
        Serial.println(value);
#endif
        return value;
    }else{
        return NULL;
    }
}
/**
 * This function is verify the connection with the Ubidots server
 * @arg chain  This array contain the all data to send to Ubidots
 * @arg status This array is to save the status value of the connection
 * confirm the connection status, example 200, 201 or wathever.
 * @arg body  This array is to save the body that you get.
 * @return true upon success, false upon i>ATTEMPS
 */
bool Ubidots::sendWithReconnect(char* chain, char* status, char* body, unsigned int size){
    int i = 0;
    while(!send(chain, status, body, size)){
        if(i > ATTEMPS){
            System.reset();
            return false;
        }
        i++;
    }
    return true;
}
/* This function is to connect to the Ubidots API, and this one 
 * saves the data that the API send you 
 * @arg status This array is to save the information to
 * confirm the connection status, example 200, 201 or wathever.
 * @arg body  This array is to save the body that you get.
 * @return true upon success, false upon error
 */
bool Ubidots::send(char* chain, char* status, char* body, unsigned int size){
    char* result = (char *) malloc(sizeof(char) * BUFFER_HTTP_SIZE);
    unsigned int bufferPosition = 0;
    unsigned long lastRead = millis();
    bool error = false;
    bool timeout = false;
    int i = 0;
    while(!_client.connected() && i<6){
        i++;
        _client.stop();
        _client.connect("things.ubidots.com", 80);
    }
    if (_client.connected()){        // Connect to the server    
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected");
#endif
        _client.write(chain);
        _client.print("\n\n");
        _client.flush();
        do {
            while (_client.available()){
                char c = _client.read();
                lastRead = millis();
                if (c == -1){
                    error = true;
                    Serial.println("HttpClient>\tError: No data available.");
                    break;
                }  
                // Check that received character fits in buffer before storing.
                if (bufferPosition < BUFFER_HTTP_SIZE-1) {
                    result[bufferPosition] = c;
                } else if ((bufferPosition == BUFFER_HTTP_SIZE-1)) {
                    result[bufferPosition] = '\0'; // Null-terminate buffer
                    error = true;
                }
                bufferPosition++;
            }
            result[bufferPosition] = '\0'; // Null-terminate buffer
            // Check that there hasn't been more than 5s since last read.
            timeout = millis() - lastRead > TIMEOUT;
            // Unless there has been an error or timeout wait 200ms to allow server
            // to respond or close connection.
            if (!error && !timeout){
                delay(200);
            }   
        }
        while (_client.connected() && !timeout && !error);
        delay(200);
        if(result[0]=='\0'){
#ifdef DEBUG_UBIDOTS
            Serial.println("Error when particle recive the data\narray of content of c is NULL");
#endif
            free(result);
            return false;
        }
        _client.flush();
        String rawResponse(result);
        rawResponse.substring(9,12).toCharArray(status, 4);
        int bodyPos = rawResponse.indexOf("\r\n\r\n");
        rawResponse.substring(bodyPos+4).toCharArray(body, size);
#ifdef DEBUG_UBIDOTS
        Serial.println("--------------------body-----------------");
        Serial.println(body);
        Serial.println("--------------------body END---------------------------");
        Serial.println("-------------------status code-----------------");
        Serial.println(status);
        Serial.println("-----------------status code END----------------------------");
#endif
        free(result);
        delay(10);
        return true;
    }
}
