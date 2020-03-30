#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "strongpapazola"
#define WIFI_PASS "aaaaaaaa"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "hoked"
#define MQTT_PASS "0e1a1c9fef5c4c268fb5a007fb0c7c93"

const int PIN1 = D1;
const int PIN2 = D2;
const int PIN3 = D3;
const int PIN4 = D4;

//Set up MQTT and WiFi clients
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

//Set up the feed you're subscribing to
Adafruit_MQTT_Subscribe switch1 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/switch1");
Adafruit_MQTT_Subscribe switch2 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/switch2");
Adafruit_MQTT_Subscribe switch3 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/switch3");
Adafruit_MQTT_Subscribe switch4 = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/switch4");
Adafruit_MQTT_Subscribe all = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/all");


void setup()
{
  Serial.begin(9600);

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("OK!");

  //Subscribe to the switch1 feed
  mqtt.subscribe(&switch1);
  mqtt.subscribe(&switch2);
  mqtt.subscribe(&switch3);
  mqtt.subscribe(&switch4);
  mqtt.subscribe(&all);

  pinMode(PIN1, OUTPUT);
  digitalWrite(PIN1, LOW);
  pinMode(PIN2, OUTPUT);
  digitalWrite(PIN2, LOW);
  pinMode(PIN3, OUTPUT);
  digitalWrite(PIN3, LOW);
  pinMode(PIN4, OUTPUT);
  digitalWrite(PIN4, LOW);
//  pinMode(LED_BUILTIN, OUTPUT);
//  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  MQTT_connect();
  
  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &switch1)
    {
      //Print the new value to the serial monitor
      Serial.print("switch1: ");
      Serial.println((char*) switch1.lastread);
      
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) switch1.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(PIN1, LOW);
      }
      else
      {
        digitalWrite(PIN1, HIGH);
      }
    }
    //If we're in here, a subscription updated...
    if (subscription == &switch2)
    {
      //Print the new value to the serial monitor
      Serial.print("switch2: ");
      Serial.println((char*) switch2.lastread);
      
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) switch2.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(PIN2, LOW);
      }
      else
      {
        digitalWrite(PIN2, HIGH);
      }
    }
    //If we're in here, a subscription updated...
    if (subscription == &switch3)
    {
      //Print the new value to the serial monitor
      Serial.print("switch3: ");
      Serial.println((char*) switch3.lastread);
      
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) switch3.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(PIN3, LOW);
      }
      else
      {
        digitalWrite(PIN3, HIGH);
      }
    }
    //If we're in here, a subscription updated...
    if (subscription == &switch4)
    {
      //Print the new value to the serial monitor
      Serial.print("switch4: ");
      Serial.println((char*) switch4.lastread);
      
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) switch4.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(PIN4, LOW);
      }
      else
      {
        digitalWrite(PIN4, HIGH);
      }
    }
    //If we're in here, a subscription updated...
    if (subscription == &all)
    {
      //Print the new value to the serial monitor
      Serial.print("all: ");
      Serial.println((char*) all.lastread);
      
      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) all.lastread, "ON"))
      {
        //Active low logic
        digitalWrite(PIN1, LOW);
        digitalWrite(PIN2, LOW);
        digitalWrite(PIN3, LOW);
        digitalWrite(PIN4, LOW);
      }
      else
      {
        digitalWrite(PIN1, HIGH);
        digitalWrite(PIN2, HIGH);
        digitalWrite(PIN3, HIGH);
        digitalWrite(PIN4, HIGH);
      }
    }
  }

  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping())
  {
    mqtt.disconnect();
  }
}


/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

void MQTT_connect() 
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
