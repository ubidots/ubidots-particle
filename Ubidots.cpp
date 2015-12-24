#include "Ubidots.h"
static const uint16_t TIMEOUT = 2000;
/**
* Constructor.
*/
Ubidots::Ubidots(char* token)
{
    _token = token;
}
void Ubidots::assemble(char* chain, char* method, char* endpoint)
{
    sprintf(chain, "%s /api/v1.6/%s HTTP/1.1\nHost: %s\nUser-Agent: %s \nX-Auth-Token: %s", method, endpoint, BASE_URL, USER_AGENT, _token);
    //strstr(chain, "User-Agent");
}
void Ubidots::assemble_with_data(char* method, char* chain, char* endpoint, char* data)
{
    assemble(chain, method, endpoint);
    sprintf(chain,"%s\nContent-Type: application/json\nContent-Length:  %d\n\n%s\n", chain, strlen(data), data);
}
bool Ubidots::get_or_create_datasource(){
    char chain[700];
    char endpoint[100];
    char data[100];
    char status[3];
    char body[200];
    char  datasource[24];
    const char* ID = Particle.deviceID();
    sprintf(endpoint, "datasources/?tag=%s", ID);
    sprintf(data, "{\"name\": \"Particle\",\"tags\":[\"%s\"]}", ID);
    assemble((char *) chain, (char *)"GET",(char *) endpoint); // send core id and check if it is living
    if(!Send_with_reconect(chain, status, body)){
        Serial.print("Connection error");
        return false;
    }
    memset(chain, 0, sizeof(chain));
    if(check_get_datasource(status, body, datasource))
    {
        assemble_with_data("POST", chain, endpoint, data);
        //Send(chain, status, body);
        //memset(chain, 0, sizeof(chain));
    }
    
    
}
bool Ubidots::check_get_datasource(char* status, char* body, char* datasource){
    int first_number = 0;
    String raw_response(body);
    int bodyPos = raw_response.indexOf("\"id\": ");
    if(strstr(status, "200")!=NULL && strstr(body, "\"id\": ")!=NULL && strstr(body,"\"count\": 0\"")==NULL){
        raw_response.substring(bodyPos+7).toCharArray(datasource, 24);
        Serial.println(datasource);
        return true;
    }else{
        return false;
    }
}

bool Ubidots::Send_with_reconect(char* chain, char* status, char* body)
{
    int i = 0;
    while(!Send(chain, status, body)){
        if(i > ATTEMPS)
        {
            return false;
        }i++;
    }return true;
    
}
// int check_post_datasource(char* state, char* body)
// {
//  int first_number = 0;
    
//  if(strcmp(state, "201"))
//  {
//      first_number = body.find("\"id\": ")+8;
//      //24
//  }
//  else
//  {
//      return 1;
//  }
// }

/* This function is to connect to the Ubidots API, and this one 
   saves the data that the API send you 
   
   @arg state This array is to save the information to
              confirm the connection status, example 200, 201 or wathever.
   @arg body  This array is to save the body that you get.
   @return true upon success, false upon error
*/

boolean Ubidots::Send(char* chain, char* status, char* body)
{
    TCPClient client;
    int len_result = 2048;
    char result[len_result];
    unsigned int bufferPosition = 0;
    unsigned long lastRead = millis();
    unsigned long firstRead = millis();
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
                //Serial.print("CC: ");
                //Serial.println(c);
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
        if(result[0]=='\0')
        {
            return false;
        }
        String raw_response(result);
        // Not super elegant way of finding the status code, but it works.
        raw_response.substring(9,12).toCharArray(status, 4);

        //Serial.print("HttpClient>\tStatus Code: ");
        //Serial.println(statusCode);

        int bodyPos = raw_response.indexOf("\r\n\r\n");
    
        // Return the entire message body from bodyPos+4 till end.
    
        raw_response.substring(bodyPos+4).toCharArray(body, 200);
        Serial.println(body);
        Serial.println(status);
        //Serial.println(raw_response);
    
    }

}
