/*
 WiFiEsp example: WebClientRepeating

 This sketch connects to a web server and makes an HTTP request
 using an Arduino ESP8266 module.
 It repeats the HTTP call each 10 seconds.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include "WiFiEsp.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // TX, RX
#endif

char ssid[] = "Wifi Gratis";            // your network SSID (name)
char pass[] = "12345678";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "192.168.251.174";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 5000L; // delay between updates, in milliseconds

// Initialize the Ethernet client object
WiFiEspClient client;

const int pinData = 4; // Ubah sesuai dengan pin yang Anda gunakan
OneWire oneWire(pinData);
DallasTemperature sensors(&oneWire);

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  
  printWifiStatus();

  sensors.begin();   // Mulai komunikasi dengan sensor
}

void loop()
{
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}

// this method makes a HTTP connection to the server
void httpRequest()
{
  float ntuValue = 25.0;
  float pHValue = 7.5;
  float DOValue = 4.5;
  String kualitasAir = "Buruk";
  String sistemKendali = "Hidup";

  sensors.requestTemperatures(); // Minta sensor untuk membaca suhu
  float temperature = sensors.getTempCByIndex(0); // Baca suhu dalam Celsius
  Serial.print("Suhu pada air sekarang : ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Kekeruhan pada air sekarang : ");
  Serial.print(ntuValue);
  Serial.println(" NTU");
  Serial.print("Kadar pH pada air sekarang : ");
  Serial.println(pHValue);
  Serial.print("Kadar Oksigen pada air sekarang : ");
  Serial.print(DOValue);
  Serial.println(" NTU");
  Serial.print("Kualitas air : ");
  Serial.println(kualitasAir);
  Serial.print("Sistem Kendali : ");
  Serial.println(sistemKendali);
  Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    
    // send the HTTP PUT request
    client.print(F("GET /water-monitoring/public/simpan/"));
    client.print(String(temperature));
    client.print(F("/"));
    client.print(String(ntuValue));
    client.print(F("/"));
    client.print(String(pHValue));
    client.print(F("/"));
    client.print(String(DOValue));
    client.print(F("/"));
    client.print(kualitasAir);
    client.print(F("/"));
    client.print(sistemKendali);
    client.println(F(" HTTP/1.1"));
    client.println(F("Host: 192.168.251.174"));
    client.println(F("Connection: close"));
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
