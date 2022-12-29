#include <LWiFi.h>
#include <PubSubClient.h>
#include "Ultrasonic.h"

#define WIFI_SSID "FAST_9C8D52"
#define WIFI_PASSWORD "048889107"

#define MQTT_SERVER_IP "mqtt3.thingspeak.com"  //192.168.5.191
#define MQTT_SERVER_PORT 1883
const char MQTT_TOPIC[] =  "channels/1760300/publish/fields/field1";
const char clientID[] = "AzoMMhkkKTM0DgAQMToKIxM";
const char userName[] ="AzoMMhkkKTM0DgAQMToKIxM";
const char passwd[] ="LXtQ86zF2opkLWI4r+/HqGH0";

int status = WL_IDLE_STATUS;
WiFiClient client;
PubSubClient upload(client);

Ultrasonic ultrasonic(2);// pin D2

void reconnect()
{
  // Loop until we're reconnected
  while (!upload.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (upload.connect(clientID, userName, passwd)) //mqtt若有限制access，則要這樣使用
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(upload.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();


  upload.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  delay(1500);
}

void loop()
{
  if (!upload.connected())
  {
    reconnect();
  }
  else
  {
    // send distance
    String payload = String(ultrasonic.MeasureInCentimeters());

    // publish to MQTT Broker
    if (upload.publish(MQTT_TOPIC, payload.c_str()))
    {
      Serial.println("Message => " + payload + " has been sent to " + MQTT_SERVER_IP + ".");
    }
    delay(100);
  }

  upload.loop();
  delay(2000);
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
