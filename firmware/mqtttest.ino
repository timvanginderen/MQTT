#include "MQTT.h"

void callback(char* topic, byte* payload, unsigned int length);

/**
 * if want to use IP address,
 * byte server[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * MQTT client("www.sample.com", 1883, callback);
 **/
byte server[] = { 192,168,1,121 };
MQTT client(server, 1883, callback);

byte bytebuffer[30];
int analogPin = A0;


unsigned long timer; // the timer
unsigned long PUBLISH_INTERVAL = 60 * 1000; // the repeat interval
unsigned long RE_CONNECT_INTERVAL = 30 * 1000; // the repeat interval

// recieve message and trigger publish or led color change
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
    else if (message.equals("A0"))
        publishSensorValue();
    else
        RGB.color(255, 255, 255);

    delay(1000);
}

void setup() {
    RGB.control(true);
    timer = millis();

    connectAndSuscribe();
}

void loop() {
    if (client.isConnected()) {
        client.loop();

        if ((millis()-timer) > PUBLISH_INTERVAL) {
            // reset timer by moving it along to the next interval
            timer += PUBLISH_INTERVAL;

            publishSensorValue();
        }

    } else {
      // Try to re-connect on failure
      delay(RE_CONNECT_INTERVAL);
      connectAndSuscribe();
    }
}

void connectAndSuscribe() {
  // connect to the server
  client.connect("photon");

  // subscribe to receive mqtt message to trigger events
  if (client.isConnected()) {
      client.subscribe("/photon/input");
  }
}

void publishSensorValue() {

    // Get sensor value
    int sensorValue = analogRead(analogPin);

    // Convert to char*
    String message = String(sensorValue);
    char* char_type = new char[message.length()];
    strcpy(char_type, message.c_str());

    // Publish to MQTT topic
    client.publish("/photon/humidity", char_type);

}
