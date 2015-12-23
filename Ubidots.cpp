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
void Ubidots::get_or_create_datasource(){
    char chain[700];
    char endpoint[100];
    char data[100];
    char status[3];
    char body[200];
    const char* ID = Particle.deviceID();
    sprintf(endpoint, "datasources/?tag=%s", ID);
    sprintf(data, "{\"name\": \"Particle\",\"tags\":[\"%s\"]}", ID);
	assemble((char *) chain, (char *)"GET",(char *) endpoint); // send core id and check if it is living
	Send(chain, status, body);
	memset(chain, 0, sizeof(chain));
	assemble_with_data("POST", chain, endpoint, data);
	Send(chain, status, body);
	memset(chain, 0, sizeof(chain));
	
}
// int check_get_datasource(char* state, char* body)
// {
// 	int first_number = 0;
// 	char request[];
// 	if(strcmp(state, "200")&&strcmp(body, "\"id\": "))
// 	{
// 		first_number = body.find("\"id\": ")+8;
// 		sprintf(request, "%s", body.substring(first_number,24));
// 	}
// 	else
// 	{
// 		return 1;
// 	}
// }
// int check_post_datasource(char* state, char* body)
// {
// 	int first_number = 0;
	
// 	if(strcmp(state, "201"))
// 	{
// 		first_number = body.find("\"id\": ")+8;
// 		//24
// 	}
// 	else
// 	{
// 		return 1;
// 	}
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

	String info = "";
	info = String(chain);
	//Serial.println(info);
	unsigned int bufferPosition = 0;
	unsigned long lastRead = millis();
    unsigned long firstRead = millis();
    bool error = false;
    bool timeout = false;
    memset(&result[0], 0, sizeof(result));
    
    if (client.connect("things.ubidots.com", 80))        // Connect to the server
    	{
    		client.print(info);
    		client.println();
    		client.println();
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
                } else if ((bufferPosition == sizeof(result)-1)) {
                    result[bufferPosition] = '\0'; // Null-terminate buffer
                    client.stop();
                    error = true;
                    //Serial.println("HttpClient>\tError: Response body larger than buffer.");
                
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
        String raw_response(result);
        // Not super elegant way of finding the status code, but it works.
        String statusCode = raw_response.substring(9,12);
        statusCode.toCharArray(status, 4);

        //Serial.print("HttpClient>\tStatus Code: ");
        //Serial.println(statusCode);

        int bodyPos = raw_response.indexOf("\r\n\r\n");
    
        // Return the entire message body from bodyPos+4 till end.
    
        String bodyCode = raw_response.substring(bodyPos+4);
        bodyCode.toCharArray(body, 200);
        Serial.println(body);
        Serial.println(status);
        //Serial.println(raw_response);
    
    }

}
