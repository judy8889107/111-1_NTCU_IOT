
//Send counter every 30s
#include <LWiFi.h>


char ssid[] = "FAST_9C8D52";      //  your network SSID (name)
char pass[] = "048889107";  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;         
int counter = 1;

#define HOST    "api.thingspeak.com" // ThingSpeak IP Address: 184.106.153.149
#define PORT    80 //http

String GET = "GET /update?key=L2IEPI40VQX52HZV"; //Linkit 7697_counter_homework_lab11-01 api key

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(117,185,24,248);
char server[] = "api.thingspeak.com"; 

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
    }
  
    Serial.println("Connected to wifi");
    printWifiStatus();

    Serial.println("\nStarting connection to server...");
    
}

void loop() {

// if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
        Serial.println("connected to server (GET)");

        String getStr = GET + "&field1=" + String(counter++) +
                              "&field2=" + String(counter++) +
                              " HTTP/1.1\r\n";
        client.print( getStr );
        client.print( "Host: api.thingspeak.com\n" );
        client.print( "Connection: close\r\n\r\n" );
       
        delay(1000);

       // if there are incoming bytes available
       // from the server, read them and print them:
          while (client.available()) {
              char c = client.read();
              Serial.write(c);
          }
    }
    delay(30000);
}


void printWifiStatus() {
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
