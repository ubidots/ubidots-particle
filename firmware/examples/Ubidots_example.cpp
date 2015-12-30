
#include "Ubidots.h"

ubidots Ubidots("CCN8FrVulRYGulPTkbaiR9Myx8qN2o");


void setup() {
    Serial.begin(115200);
}
void loop() {
    //char*  pch = Ubidots.get_or_create_datasource();
    //char* pch2 = Ubidots.get_or_create_variable(pch, "temperatura");
    /*UbidotsCollection *coll = Ubidots.ubidots_collection_init(3);
    Ubidots.ubidots_collection_add(coll, "56829c367625424b83d95d86", 23);
    Ubidots.ubidots_collection_add(coll, "56829c2f7625424c4a4752af", 5);
    Ubidots.ubidots_collection_add(coll, "5681ab677625423c3c26f90e", 32);
    Ubidots.ubidots_collection_save(coll);
    Ubidots.ubidots_collection_cleanup(coll);*/
    
    Ubidots.send_ubidots( 3, "Temperatura",2.3,"Servomotor", 3.2, "Humedad", 4.2 );
    

}
