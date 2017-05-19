// This #include statement was automatically added by the Particle IDE.
#include <Ubidots.h>

#define TOKEN "BBFF-nSMdAoRzUNZ8H8jqh8j69t7FRKRdMEi5nA9lIF3XKSnjJde3qJQppYD" // Put here your Ubidots TOKEN

#define SERVER {50, 23, 124, 66}

Ubidots ubidots(TOKEN);

void setup() {

    Serial.begin(115200);
    Serial1.begin(115200);

    ubidots.setDebug(true); //Uncomment this line for printing debug messages
    ubidots.setMethod(TYPE_UDP);

}

void loop() {

    if (Serial1.available()) {
        uint8_t i = 0;
        char data[4];
        float dataFloat;
        while (Serial1.available() > 0 ) { // Reads the value from the serial port
            data[i++] = (char)Serial1.read();
        }

        dataFloat = atof(data); // casts to float type

        if ((dataFloat >= 2000.0) && (dataFloat <= 3000.0)) {
            float value = dataFloat - 2000.0;
            char dataParticle[100];

            sprintf(dataParticle, "{ \"BATTERY_TEST\": %s }", data);
            Particle.publish("BATTERY", dataParticle);

            ubidots.add("battery-test", value);
            if(ubidots.sendAll()){
                 // Do something if values were sent properly
                Serial.println("Values sent by the device");
            }
        }
    }
}
