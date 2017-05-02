// This example is to get the last value of variable from the Ubidots API

/****************************************
 * Include Libraries
 ****************************************/

#include <Ubidots.h>

/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "...."  // Put here your Ubidots TOKEN
#define VAR_ID "58d9153e762542576b721820"  // Put here your data source name
#define VAR_LABEL "...." // Put here your variable api label
#define DEVICE_LABEL "...." // Put here your device api label
const ERROR_VALUE = -3.4028235E+10 // Error constant value returned by get functions if any issue is captured

/****************************************
 * Auxiliar Functions
 ****************************************/

//Put here your auxiliar functions


/****************************************
 * Main Functions
 ****************************************/


Ubidots ubidots(TOKEN);

void setup() {
    Serial.begin(115200);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    /*
    * Obtains values using TCP according to structure specified at
    * http://help.ubidots.com/developers/send-data-to-ubidots-over-tcp-or-udp
    */
    float value1 = ubidots.getValue(VAR_ID);
    float value2 = ubidots.getValueWithDatasource(DEVICE_LABEL, VAR_LABEL);

    /*
    * Obtains values using HTTP according to structure specified at
    * https://ubidots.com/docs/api/index.html#get-values
    */
    float value3 = ubidots.getValueHTTP(VAR_ID);

    // Evaluates the results obtained
    if(value1!=ERROR_VALUE){
      Serial.print("value 1:");
      Serial.println(value1);
    }
    if(value2!=ERROR_VALUE){
      Serial.print("value 2:");
      Serial.println(value2);
    }
    if(value3!=ERROR_VALUE){
      Serial.print("value 3:");
      Serial.println(value3);
    }
    delay(5000);
}
