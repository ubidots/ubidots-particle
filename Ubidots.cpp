#include "Ubidots.h"

/**
* Constructor.
*/
Ubidots::Ubidots(char* token)
{
	_token = token;
}
void Ubidots::assemble(char* chain, char* method, char* endpoint)
{
    delay(5000);
	//String chain = method + "/api/v1.6/"+endpoint+" HTTP/1.1\nHost: "+base_url+"\nUser-Agent: "+user_agent+"\nX-Auth-Token: "+_token+"\n";
	sprintf(chain, "%s /api/v1.6/%s HTTP/1.1\nHost: %s\nUser-Agent: %s \nX-Auth-Token: %s", method, endpoint, BASE_URL, USER_AGENT, _token);
	//strstr(chain, "User-Agent");
}
void Ubidots::assemble_with_data(char* method, char* chain, char* endpoint, char* data)
{
    
	assemble(chain, method, endpoint);
	sprintf(chain,"%s\nContent-Type: application/json\nContent-Length:  %d\n\n%s\n", chain, strlen(data), data);
	//chain += "\nContent-Type: application/json\nContent-Length: "+len+"\n\n"+data;
}
void Ubidots::get_or_create_datasource(){
    char chain[700];
    char endpoint[100];
    char data[100];
    const char* ID = Particle.deviceID();
    sprintf(endpoint, "datasources/?tag=%s", ID);
    sprintf(data, "{\"name\": \"Particle\",\"tags\":[\"%s\"]}", ID);
    String state;
    String body;
	assemble((char *) chain, (char *)"GET",(char *) endpoint); // send core id and check if it is living
	Send(chain, state, body);
	memset(chain, 0, sizeof(chain));
	assemble_with_data("POST", chain, endpoint, data);
	Send(chain, state, body);
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

boolean Ubidots::Send(char* chain, String state, String body)
{
    TCPClient client;
	int len_result = 2048;
	char result[len_result];
	int i = 0;
	int status = 0;
	String info;
	info = String(chain);
	Serial.println(info);
	delay(2000);

	if (client.connect("things.ubidots.com", 80))        // Connect to the server
	{
		client.print(info+"\n\n");                         // Print the data to the client
		int timeout = 1000;
		while (client.available() || (timeout-- > 0))    // Wait until the API return the data
		{
			char c = client.read();                      // Save the data into a char
			if (i < 512)
				result[i++] = c;                         // Organice the data into a char array
			delay(1);
		}
		i = 0;
		while(i<(len_result-1))                          // Select the important data from the char array, body and state
		{
			if(result[i] == '\n'&&result[i+1] == '\n')   // This code is to find 2 consecutives "\n" 
			{
				break;
			}
			i++;
		} 
		String b = String(result);
		Serial.print(b);
		status = b.indexOf("HTTP/"); 
		state = b.substring(status,3);
		
		body = b.substring(i);
    	client.stop();	                                     // Close the connection to server.
	    return 1;
    }
    else
    {
    	return 0;
    }
}
