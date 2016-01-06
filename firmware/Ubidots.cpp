#include "Ubidots.h"
static const uint16_t TIMEOUT = 10000;
/**
 * Constructor.
 */
 
ubidots::ubidots(char* token){
    _token = token;
    cache = (UbidotsCollection *) malloc(sizeof(UbidotsCollection));
    cache->first = NULL;
    cache->id_datasource_default = NULL;    
    String str = Particle.deviceID();
    p_id = new char [str.length()+1];
    strcpy (p_id, str.c_str());
    //particle_id = Particle.deviceID();
}

/** 
 * This function is to initialize the cache 
 * @arg collection, is the cache struct
 * @arg name is the name of the variable that you will save
 * @arg value is the value of the variable that you will save
 * @arg id is the id of the variable that you will add 
 */
Value * ubidots::check_init_value(UbidotsCollection *collection, char* name, double value, char * id){
    Value * new_value = (Value *)malloc(sizeof(Value));
    new_value->name = name;
    new_value->value = value;
    new_value->next = NULL;
    if (id == NULL){
        if(collection->id_datasource_default==NULL){
            collection->id_datasource_default = get_or_create_datasource();
            //uno
        }
        new_value->id = get_or_create_variable(collection->id_datasource_default, name);
        //dos
        Serial.println(new_value->id);
        
    }else{
        new_value->id = id;
    }
#ifdef DEBUG_UBIDOTS
    Serial.print("Variable id: ");
    Serial.println(new_value->id);
#endif
    return new_value;
}
/** 
 * This function is add value with name to the cache (for normal users) 
 * @arg collection, is the cache struct
 * @arg name is the name of the variable that you will save
 * @arg value is the value of the variable that you will save
 */
void ubidots::add_value_with_name(UbidotsCollection *collection, char * name, double value){
    if(cache->first == NULL){
        cache->first = check_init_value(collection, name, value, NULL); 
    }else{
        Value * nod = cache->first;
        int cmp = strcmp(name, nod->name);
        while(nod->next || cmp == 0) {
            if (cmp == 0){
                nod->value = value;
                return;
            }
            Serial.println(cmp);
            Serial.println(nod->name);
            Serial.println(name);
            nod = nod->next;
            cmp = strcmp(name, nod->name);
        }
        nod->next = check_init_value(collection, name, value, NULL);
    }
}
/** 
 * This function is add value to the cache (for experts users with ID) 
 * @arg collection, is the cache struct
 * @arg variable_id is the id of the variable that you will add
 * @arg value is the value of the variable that you will save
 */
void ubidots::add_value(UbidotsCollection *collection, char * variable_id, double value){
    if(cache->first == NULL){
        cache->first = check_init_value(collection, NULL, value, variable_id); 
    }else{
        Value *nod = cache->first;
        while(nod->next || strcmp(variable_id, nod->id) == 0) {
            if (variable_id == nod->id){
                nod->value = value;
                return;
            }
            nod = nod->next;
        }
        nod->next = check_init_value(collection, NULL, value, variable_id);
    }
}
/** 
 * This function is to send infinite variables to the API (photon only support 10 variables)
 * @arg number is the quantity of variables that you will send
 * @return true upon success
 */
bool ubidots::send_ubidots( int number, ... ){
    va_list vl;
    int i;
    va_start( vl, number );
    for( i = 0; i< number; ++i ){
        add_value_with_name(cache, va_arg( vl, char* ), va_arg( vl, double ));
    }
    //return true;
    ubidots_collection_save(cache);
    return true;
}
/**
 * This function is to assemble the collection data
 * @arg collection is the Collection to save.
 * @reutrn Zero upon success, non-zero upon error.
 */
int ubidots::ubidots_collection_save(UbidotsCollection *collection){
    char* endpoint= (char *) malloc(sizeof(char) * 100);
    char* data = (char *) malloc(sizeof(char) * 800);
    char* status = (char *) malloc(sizeof(char) * 3);
    char* body = (char *) malloc(sizeof(char) * 200);
    char *chain;
    sprintf(data, "[");
    Value *nod = cache->first;
    int count = 0;
    
    while(nod) {
        Serial.println(nod->id);
        if(nod->value){
            
            sprintf(data, "%s%s{\"variable\": \"%s\", \"value\":\"%f\"} ", data, (count>0) ? "," : " ", nod->id , nod->value);
            count++;
            
        }
        
        nod->value = NULL;
        nod = nod->next;
    }
    sprintf(data, "%s]", data);
    nod->value = NULL;
    chain = assemble_with_data("POST", "collections/values/?force=true", data);
#ifdef DEBUG_UBIDOTS
    Serial.print("Your data of body that you will send to ubidots is:\n");
    Serial.println(data);
#endif
    if(!send_with_reconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("send_with_reconnect fail 6 times, probably\n you have connection problem with your internet");
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
 * Cleanup a collection when after it is no longer being used.
 * @arg coll Pointer to the collection made by ubidots_collection_init().
 */
void ubidots::ubidots_collection_cleanup(UbidotsCollection *collection){
    /*for (i = 0; i < n; i++){
      free(coll->id[i]);
      }
      free(coll->id);
      free(coll->values);
      free(coll);*/
}
/** 
 * This function is to assemble the data to send to Ubidots
 * @arg chain This array is to save all data to send to the API 
 * @arg method   This array contains GET or POST method
 * @arg endpoint  This array contains the endpoint to send to the API
 */
char* ubidots::assemble(char* method, char* endpoint){
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
char* ubidots::assemble_with_data(char* method, char* endpoint, char* data){
    char* chain = (char *) malloc(sizeof(char) * 1000);
    char* chain_dos = assemble(method, endpoint);
    sprintf(chain,"%s\nContent-Type: application/json\nContent-Length:  %d\n\n%s\n",chain_dos , strlen(data), data);
#ifdef DEBUG_UBIDOTS
    Serial.println("Assemble_with_data chain: ");
    Serial.println(chain);
#endif
    free(chain_dos);
    return chain;
}
/**
 * This function is to get or post datasource
 * @return datasource ID upon succes or NULL in bad connection
 */
char* ubidots::get_or_create_datasource(){
    
    char* endpoint= (char *) malloc(sizeof(char) * 100);
    char* data = (char *) malloc(sizeof(char) * 100);
    char* status = (char *) malloc(sizeof(char) * 3);
    char* body = (char *) malloc(sizeof(char) * 200);
    char *datasource;
    char *chain;
    Serial.println(p_id);
    sprintf(endpoint, "datasources/?tag=%s", p_id);
    //tres
    
    chain = assemble("GET", endpoint);
    
    
    if(!send_with_reconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("send_with_reconnect fail 6 times, probably\n you have connection problem with your internet");
#endif
        free(datasource);
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return NULL;
    }
    datasource = parser_id(status, body);
  


    if(datasource==NULL && strstr(body,"\"count\": 0")!=NULL){
        sprintf(data, "{\"name\": \"Particle\",\"tags\":[\"%s\"]}", p_id);
        chain = assemble_with_data("POST", endpoint, data);
        datasource = parser_id(status, body);
    
        
        if(!send_with_reconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("send_with_reconnect fail 6 times, probably\n you have connection problem with your internet");
#endif  
        free(datasource);
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return NULL;
        }
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
char* ubidots::get_or_create_variable(char* ID, char* variableName){

    char* endpoint= (char *) malloc(sizeof(char) * 100);
    char* data = (char *) malloc(sizeof(char) * 100);
    char* status = (char *) malloc(sizeof(char) * 3);
    char* body = (char *) malloc(sizeof(char) * 200);
    char *variable;
    char *chain;
    sprintf(endpoint, "datasources/%s/variables/?tag=%s", ID, variableName);
    chain = assemble((char *)"GET",(char *) endpoint); // send core id and check if it is living
    if(!send_with_reconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("send_with_reconnect fail 6 times, probably\n you have connection problem with your internet");
#endif
        free(variable);
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return NULL;
    }
    variable = parser_id(status, body);
    if(variable==NULL && strstr(body,"\"count\": 0")!=NULL){
        sprintf(data, "{\"name\": \"%s\",\"tags\":[\"%s\"]}", variableName, variableName);
        chain = assemble_with_data("POST", endpoint, data);
        if(!send_with_reconnect(chain, status, body, 190)){
#ifdef DEBUG_UBIDOTS
        Serial.print("send_with_reconnect fail 6 times, probably\n you have connection problem with your internet");
#endif  
        free(variable);
        free(chain);
        free(endpoint);
        free(data);
        free(status);
        free(body);
        return NULL;
        }
        variable = parser_id(status, body);
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
char* ubidots::parser_id(char* status, char* body){
    String raw_response(body);
    int bodyPos = raw_response.indexOf("\"id\": ");
    
    if((strstr(status, "200")!=NULL || strstr(status, "201")!=NULL) && strstr(body, "\"id\": ")!=NULL && strstr(body,"\"count\": 0")==NULL){
        char* ID = (char *) malloc(sizeof(char) * 24);
        raw_response.substring(bodyPos+7).toCharArray(ID, 25);
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
 * This function is verify the connection with the Ubidots server
 * @arg chain  This array contain the all data to send to Ubidots
 * @arg status This array is to save the status value of the connection
 * confirm the connection status, example 200, 201 or wathever.
 * @arg body  This array is to save the body that you get.
 * @return true upon success, false upon i>ATTEMPS
 */
bool ubidots::send_with_reconnect(char* chain, char* status, char* body, unsigned int size){
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
bool ubidots::send(char* chain, char* status, char* body, unsigned int size){
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
        String raw_response(result);
        raw_response.substring(9,12).toCharArray(status, 4);
        int bodyPos = raw_response.indexOf("\r\n\r\n");
        raw_response.substring(bodyPos+4).toCharArray(body, size);
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
