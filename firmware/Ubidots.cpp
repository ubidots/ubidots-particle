#include "Ubidots.h"
/**
 * Constructor.
 */
Ubidots::Ubidots(char* token){
    _token = token;
    maxValues = 10;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
}
/**
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValueWithDatasource(char* dsName, char* idName) {
  float num;
  int i = 0;
  char* allData = (char *) malloc(sizeof(char) * 300);
  String response;
  uint8_t bodyPosinit;
  sprintf(allData, "Particle|LV|%s|%s:%s|end", _token, dsName, idName);
  while (!_client.connected() && i < 6) {
        i++;
        _client.connect(SERVER, PORT);
    }
    if (_client.connected()) {        // Connect to the server
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected");
#endif
        Serial.println(allData);
        _client.println(allData);
        _client.flush();
    }
    i = 50000;
    while (!_client.available()|| i == 0) {
        i--;
    }
    while (_client.available()) {
        char c = _client.read();
        response += c;
    }
    bodyPosinit = 3 + response.indexOf("OK|");
    response = response.substring(bodyPosinit);
    num = response.toFloat();
    currentValue = 0;
    _client.stop();
    free(allData);
    _client.stop();
    return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value) {
  add(variable_id, value, NULL, NULL);
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value, char *ctext1) {
  add(variable_id, value, ctext1, NULL);
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value, char *ctext1, char *ctext2) {
  (val+currentValue)->idName = variable_id;
  (val+currentValue)->idValue = value;
  (val+currentValue)->contextOne = ctext1;
  (val+currentValue)->contextTwo = ctext2;
  currentValue++;
  if (currentValue > maxValues) {
        Serial.println(F("You are sending more than 10"));
        Serial.println(F("consecutives variables,"));
        Serial.println(F("you just could send 5 variables"));
        currentValue = maxValues;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll() {
    return sendAll(NULL);
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(char* dsName) {
    int i;
    char* allData = (char *) malloc(sizeof(char) * 700);
    if (dsName == NULL) {
        sprintf(allData, "Particle|POST|%s|Particle,", _token);
    } else {
        sprintf(allData, "Particle|POST|%s|%s,", _token, dsName);
    }
    for ( i = 0; i < currentValue; ) {
        sprintf(allData, "%s%s:%f", allData, (val + i)->idName, (val + i)->idValue);
        if ((val + i)->contextOne != NULL) {
            sprintf(allData, "%s#%s", allData, (val + i)->contextOne);
        }
        if ((val + i)->contextTwo != NULL) {
            sprintf(allData, "%s#%s", allData, (val + i)->contextTwo);
        }
        i++;
        if (i < currentValue) {
            sprintf(allData, "%s,", allData);
        }
    }
    sprintf(allData, "%s|end", allData);
    Serial.println(allData);
    while (!_client.connected() && i < 6) {
        i++;
        _client.connect(SERVER, PORT);
    }
    if (_client.connected()) {        // Connect to the server
#ifdef DEBUG_UBIDOTS
        Serial.println("Client connected");
#endif
        Serial.println(allData);
        _client.println(allData);
        _client.flush();
    }
    i = 50000;
    while (!_client.available() || i == 0) {
        i--;
    }
    while (_client.available()) {
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    _client.stop();
    free(allData);
    return true;
}
