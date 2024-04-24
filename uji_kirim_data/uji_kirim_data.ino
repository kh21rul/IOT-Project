#include "WiFiEsp.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#endif

char ssid[] = "Wifi Gratis";            // your network SSID (name)
char pass[] = "08222913";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "monitoring.kh21rul.site";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

// Initialize the Ethernet client object
WiFiEspClient client;

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
}

void loop()
{
  float temperature = random(25, 40);
  float ntuValue = random(25, 600);
  float Po = random(2, 10);
  float DO_mgL = random(2, 12);

  boolean relayState = false;

  // Cek kondisi untuk menghidupkan relay berdasarkan kualitas air
  if ((temperature < 29 || temperature > 30) ||
      (ntuValue > 400) ||
      (Po < 7.7 || Po > 8.5) ||
      (DO_mgL < 4)) {
    relayState = true;
  }

  // Variabel untuk menyimpan kualitas air dan status sistem kendali
  String kualitasAirStr;
  String sistemKendaliStr;

  // Set nilai kualitas air berdasarkan kondisi
  if (relayState) {
    kualitasAirStr = "Buruk";
    sistemKendaliStr = "Hidup";
  } else {
    kualitasAirStr = "Baik";
    sistemKendaliStr = "Mati";
  }

  String data = String(temperature, 2) + "/" +
              String(ntuValue, 2) + "/" +
              String(Po, 2) + "/" +
              String(DO_mgL, 2) + "/" +
              kualitasAirStr + "/" +
              sistemKendaliStr;

  Serial.print("Suhu Air:\t\t");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Kekeruhan Air:\t\t");
  Serial.print(ntuValue);
  Serial.println(" NTU");

  Serial.print("Kadar pH Air:\t\t");
  Serial.println(Po, 2);

  Serial.print("Kadar Oksigen Air:\t");
  Serial.print(DO_mgL, 2); // Print DO with 3 decimal places
  Serial.println(" mg/L");

  Serial.print("Kualitas Air:\t\t");
  Serial.println(kualitasAirStr);

  Serial.print("Sistem Kendali:\t\t");
  Serial.println(sistemKendaliStr);
  Serial.println("========================");
  Serial.println();

  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest(data);
  }
  
  delay(1000);
}

// this method makes a HTTP connection to the server
void httpRequest(String data)
{
  Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    
    // send the HTTP GET request with the provided data
    client.print(F("GET /simpan/"));
    client.print(data);
    client.println(F(" HTTP/1.1"));
    client.println(F("Host: monitoring.kh21rul.site"));
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
