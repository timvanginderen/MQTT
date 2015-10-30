#include "MQTT.h"

void callback(char* topic, byte* payload, unsigned int length);

/**
 * if want to use IP address,
 * byte server[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * MQTT client("www.sample.com", 1883, callback);
 **/
byte server[] = { 192,168,1,20 };
MQTT client(server, 1883, callback);

byte bytebuffer[30];
int analogPin = A0;


// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);

    if (message.equals("RED"))
        RGB.color(255, 0, 0);
    else if (message.equals("GREEN"))
        RGB.color(0, 255, 0);
    else if (message.equals("BLUE"))
        RGB.color(0, 0, 255);
    else
        RGB.color(255, 255, 255);
    delay(1000);
}


void setup() {
    RGB.control(true);

    // connect to the server
    client.connect("sparkclient");

    // publish/subscribe
    if (client.isConnected()) {
        client.subscribe("amoeder");
    }
}

void loop() {
    if (client.isConnected())
        client.loop();

    publishSensorValue();

    delay(1000);


}

void publishSensorValue() {
    int sensorValue = analogRead(analogPin);

    String message = String(sensorValue);
    message.getBytes(bytebuffer, 20);

    client.publish("amoeder", bytebuffer, 20);
}
