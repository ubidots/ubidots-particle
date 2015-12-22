#include "Ubidots.h"
#include <stdlib.h>

/**
* Constructor.
*/
Ubidots::Ubidtos(String token)
{
	_token = token;
}
void Ubidots::assemble(char* chain, char* method, char* endpoint)
{
	//String chain = method + "/api/v1.6/"+endpoint+" HTTP/1.1\nHost: "+base_url+"\nUser-Agent: "+user_agent+"\nX-Auth-Token: "+_token+"\n";

	sprintf(chain, "%s /api/v1.6/%s HTTP/1.1\nHost: %S\nUser-Agent: %s \nX-Auth-Token: %s", method, BASE_URL, endpoint, USER_AGENT, _token);


}
void Ubidots::assemble_with_data(char* chain, char* method, char* endpoint, char* data)
{
	
	assemble(chain, method, endpoint);
	sprintf(chain,"%s\nContent-Type: application/json\nContent-Length:  %d\n\n%s\n", chain, strlen(data), data)
	//chain += "\nContent-Type: application/json\nContent-Length: "+len+"\n\n"+data;

}


void Ubidots::start(){
	info = assemble();
	int data = Send;
	
}
void Ubidots::create_datasource(){
    char chain[500];
    char state[];
    char body[];
	assemble(chain,"GET","datasources/?tag="+ID); // send core id and check if it is living
	int data = Send();
	if (dat==1)
	{
		assemble_with_data("POST","datasources/?tag="+ID,"{\"name\": \"Particle\",\"tags\":[\""ID"\"]}");
		int data = Send();
	}
}

void Ubidots::save_value(String varibleName, String varialbeValue) {
	info = assemble();
	int data = Send;

}
void Ubidots::create_variable(){
	info = assemble();
	int data = Send;
}
void Ubidots::save_value(String varibleName, String varialbeValue, String context) {
	info = assemble();
	int data = Send;
}
//---------------------------------------------------------------------------
/* This function is to connect to the Ubidots API, and this one 
   saves the data that the API send you 
   
   @arg state This array is to save the information to
              confirm the connection status, example 200, 201 or wathever.
   @arg body  This array is to save the body that you get.
   @return true upon success, false upon error
*/

boolean Ubidots::Send(char* state, char* body)
{
	TCPClient client;
	int len_result = 2048;
	char result[len_result];
	int i = 0;
	String
	int status = 0;


	if (client.connect("things.ubidots.com", 80))        // Connect to the server
	{
		client.print(info+"\n");                         // Print the data to the client
		int timeout = 1000;
		while (client.available() || (timeout-- > 0))    // Wait until the API return the data
		{
			char c = client.read();                      // Save the data into a char
			Serial.print(c);	
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
		status = result.find("HTTP/")+9;                 // This code is to save the body and the state in two char arrays
		sprintf(state, "%s", result.substring(status,3));
		sprintf(body, "%s",result.substring(i));
	
	client.stop();	                                     // Close the connection to server.
	return 1;
    }
    else
    {
    	return 0;
    }
}

