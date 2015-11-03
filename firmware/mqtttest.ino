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
unsigned long INTERVAL = 1000; // the repeat interval

// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);

    if (message.equals("GET/A0")) {
      publishSensorValue();
    } else {

      if (message.equals("RED"))
          RGB.color(255, 0, 0);
      else if (message.equals("GREEN"))
          RGB.color(0, 255, 0);
      else if (message.equals("BLUE"))
          RGB.color(0, 0, 255);
      else
          RGB.color(255, 255, 255);
    }

    delay(1000);
}


void setup() {
    RGB.control(true);

    timer = millis(); // start timer

    connectAndSuscribe();
}

void loop() {
    if (client.isConnected()) {
        client.loop();

        if ((millis()-timer) > INTERVAL) {
            timer += INTERVAL; // reset timer by moving it along to the next interval
            publishSensorValue();
        }

    } else {
      delay(5000);
      connectAndSuscribe();
    }
}

void connectAndSuscribe() {
  // connect to the server
  client.connect("sparkclient");

  // publish/subscribe
  if (client.isConnected()) {
      client.subscribe("amoeder");
  }
}

void publishSensorValue() {
    int sensorValue = analogRead(analogPin);

    String message = String(sensorValue);
    message.getBytes(bytebuffer, 20);

    client.publish("amoeder", bytebuffer, 20);
}
