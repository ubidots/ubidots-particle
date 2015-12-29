#include "Ubidots.h"
static const uint16_t TIMEOUT = 2000;
/**
 * Instantiate a collection.
 * @arg n  Number of values in this collection.
 * @return A pointer to a collection.
 */
UbidotsCollection* ubidots::ubidots_collection_init(int n) {
  UbidotsCollection* coll = (UbidotsCollection *) malloc(sizeof(UbidotsCollection));

  coll->n = n;
  coll->i = 0;
  coll->variable_ids = (char **) malloc(sizeof(char*) * n);
  coll->values = (float *) malloc(sizeof(float) * n);
  coll->_ids = (char *) malloc(sizeof(char*) * n);

  return coll;
}
/**
 * Add a value to a collection.
 * @arg coll         Pointer to the collection made by ubidots_collection_init().
 * @arg variable_id  The ID of the variable this value is associated with.
 * @arg value        The value.
 */
void ubidots::ubidots_collection_add(UbidotsCollection *coll, char* name, double value) {
  int i = coll->i;

  int len = sizeof(char) * strlen(name);
  coll->variable_ids[i] = (char* ) malloc(len + 1);
  strcpy(coll->variable_ids[i], name);

  coll->values[i] = value;

  coll->i++;
}
/**
 * Save a collection.
 * @arg coll Collection to save.
 * @reutrn Zero upon success, non-zero upon error.
 */
int ubidots::ubidots_collection_save(UbidotsCollection *coll) {
    int i, n = coll->n;
    char chain[700];
    char endpoint[100];
    char data[1000];
    char status[3];
    char body[200];
    memset(chain, 0, sizeof(chain));
    memset(endpoint, 0, sizeof(endpoint));
    memset(data, 0, sizeof(data));
    sprintf(endpoint, "collections/values");
    sprintf(data, "[");
  for (i = 0; i < n; i++) {
      sprintf(data, "%s{\"variable\": \"%s\", \"value\":\"%f\"}", data, coll->variable_ids[i], coll->values[i]);
      if((i+2)>n){
          sprintf(data, "%s]",data);
      }else{
          sprintf(data, "%s, ",data);
      }
  }
  Serial.println(data);
  assemble_with_data("POST", chain, endpoint, data);
  if(!send_with_reconect(chain, status, body)){
        Serial.print("Connection error");
        return 1;
    }
  return 0;
}
void ubidots::ubidots_collection_cleanup(UbidotsCollection *coll) {
  int i, n = coll->n;

  for (i = 0; i < n; i++) {
    free(coll->variable_ids[i]);
  }
  free(coll->variable_ids);
  free(coll->values);
  free(coll);
}
/**
* Constructor.
*/

ubidots::ubidots(char* token)
{
    _token = token;
}
/* This function is to assemble the data to send to Ubidots
   
   @arg chain This array is to save all data to send to the API 
   @arg method   This array contains GET or POST method
   @arg endpoint  This array contains the endpoint to send to the API
   
*/

void ubidots::assemble(char* chain, char* method, char* endpoint)
{
    sprintf(chain, "%s /api/v1.6/%s HTTP/1.1\nHost: %s\nUser-Agent: %s \nX-Auth-Token: %s", method, endpoint, BASE_URL, USER_AGENT, _token);
    #ifdef DEBUG_UBIDOTS
    Serial.println(chain);
    #endif
}
/* This function is to assemble the data with length and value of variable
   to send to Ubidots
   
   @arg chain This array is to save all data to send to the API 
   @arg method   This array contains GET or POST method
   @arg endpoint  This array contains the endpoint to send to the API
   @arg data  This array contains the value to POST to the Ubidots API
   
*/
void ubidots::assemble_with_data(char* method, char* chain, char* endpoint, char* data)
{
    assemble(chain, method, endpoint);
    sprintf(chain,"%s\nContent-Type: application/json\nContent-Length:  %d\n\n%s\n", chain, strlen(data), data);
    #ifdef DEBUG_UBIDOTS
    Serial.println(chain);
    #endif
}
/* This function is to get or post datasource
   
   @return false when the connection fails
*/

char* ubidots::get_or_create_datasource(){
    char chain[700];
    char endpoint[100];
    char data[100];
    char status[3];
    char body[200];
    char * datasource = (char *) malloc(sizeof(char) * 24);
    char ID[24];
    String particleid = Particle.deviceID();
    particleid.toCharArray(ID, particleid.length());
    memset(chain, 0, sizeof(chain));
    memset(endpoint, 0, sizeof(endpoint));
    memset(data, 0, sizeof(data));
    sprintf(endpoint, "datasources/?tag=%s", ID);
    assemble((char *) chain, (char *)"GET",(char *) endpoint); // send core id and check if it is living
    if(!send_with_reconect(chain, status, body)){
        Serial.print("Connection error");
        return NULL;
    }
    memset(chain, 0, sizeof(chain));
    if(!check(status, body, datasource) && strstr(body,"\"count\": 0")!=NULL){
        memset(data, 0, sizeof(data));
        sprintf(data, "{\"name\": \"Particle\",\"tags\":[\"%s\"]}", ID);
        assemble_with_data("POST", chain, endpoint, data);
        send_with_reconect(chain, status, body);
    }
    return datasource;
}
char* ubidots::get_or_create_variable(char* ID, char* variableName){
    char chain[700];
    char endpoint[100];
    char data[100];
    char status[3];
    char body[200];
    char * datasource = (char *) malloc(sizeof(char) * 24);
    memset(chain, 0, sizeof(chain));
    memset(endpoint, 0, sizeof(endpoint));
    memset(data, 0, sizeof(data));
    sprintf(endpoint, "datasources/%s/variables/?tag=%s", ID, variableName);
    assemble((char *) chain, (char *)"GET",(char *) endpoint); // send core id and check if it is living
    if(!send_with_reconect(chain, status, body)){
        Serial.print("Connection error");
        return NULL;
    }
    if(!check(status, body, datasource) && strstr(body,"\"count\": 0")!=NULL){
        memset(data, 0, sizeof(data));
        sprintf(data, "{\"name\": \"%s\",\"tags\":[\"%s\"]}", variableName, variableName);
        assemble_with_data("POST", chain, endpoint, data);
        send_with_reconect(chain, status, body);
    }
    return datasource;
   
}
/* This function is to know if there is spark ID
   datasource in the API
   
   @arg Status This array contains the connection status of
               the API 
   @arg body   This array contains the body of the API
   @arg datasource  This array is to save the ID of the API response 
   @return true if there is created ID, false if there is not created ID
*/
bool ubidots::check(char* status, char* body, char* datasource){
    String raw_response(body);
    int bodyPos = raw_response.indexOf("\"id\": ");
    if(strstr(status, "200")!=NULL && strstr(body, "\"id\": ")!=NULL && strstr(body,"\"count\": 0")==NULL){
        raw_response.substring(bodyPos+7).toCharArray(datasource, 25);
        #ifdef DEBUG_UBIDOTS
        Serial.println(datasource);
        #endif
        return true;
    }else{
        return false;
    }
}
/* This function is verify the connection with the Ubidots server
   
   @arg chain  This array contain the all data to send to Ubidots
   @arg status This array is to save the status value of the connection
               confirm the connection status, example 200, 201 or wathever.
   @arg body  This array is to save the body that you get.
   @return true upon success, false upon i>ATTEMPS
*/
bool ubidots::send_with_reconect(char* chain, char* status, char* body)
{
    int i = 0;
    while(!send(chain, status, body)){
        if(i > ATTEMPS){
            return false;
        }
        i++;
    }
    return true;
}
/* This function is to connect to the Ubidots API, and this one 
   saves the data that the API send you 
   
   @arg status This array is to save the information to
               confirm the connection status, example 200, 201 or wathever.
   @arg body  This array is to save the body that you get.
   @return true upon success, false upon error
*/

boolean ubidots::send(char* chain, char* status, char* body)
{
    TCPClient client;
    int len_result = 2048;
    char result[len_result];
    unsigned int bufferPosition = 0;
    unsigned long lastRead = millis();
    bool error = false;
    bool timeout = false;
    memset(&result[0], 0, sizeof(result));
    if (client.connect("things.ubidots.com", 80)){        // Connect to the server
    
            client.print(chain);
            client.print("\n\n");
            client.flush();
        do {
            while (client.available()) {
                char c = client.read();
                lastRead = millis();
                if (c == -1) {
                    error = true;
                    Serial.println("HttpClient>\tError: No data available.");
                    break;
                }
    
                // Check that received character fits in buffer before storing.
                if (bufferPosition < sizeof(result)-1) {
                    result[bufferPosition] = c;
                } 
                bufferPosition++;
            }
            result[bufferPosition] = '\0'; // Null-terminate buffer
            // Check that there hasn't been more than 5s since last read.
            timeout = millis() - lastRead > TIMEOUT;

            // Unless there has been an error or timeout wait 200ms to allow server
            // to respond or close connection.
            if (!error && !timeout) {
                delay(200);
            }
        } while (client.connected() && !timeout && !error);
        client.stop();
        // If result doesnt have any thing, return false
        if(result[0]=='\0'){
            #ifdef DEBUG_UBIDOTS
            Serial.print("Error when particle recive the data");
            #endif
            return false;
        }
        String raw_response(result);
        // Not super elegant way of finding the status code, but it works.
        raw_response.substring(9,12).toCharArray(status, 4);
        int bodyPos = raw_response.indexOf("\r\n\r\n");
        raw_response.substring(bodyPos+4).toCharArray(body, sizeof(int[200]));
        #ifdef DEBUG_UBIDOTS
        Serial.println(body);
        Serial.println(status);
        #endif
        return true;
    }

}
