#include <DHT22.h> // include library DHT22
#include <ESP8266WiFi.h> // include library DHT22 Wifi
#include <ESP8266HTTPClient.h>
//define pin data dht22
#define pinDATA 13 // SDA, or almost any other I/O pin
// Mendefinisikan pin relay
const int relayKipas = 16;
const int relayAtomizer = 5;

DHT22 dht22(pinDATA); 

// konfigurasi Wifi
const char* ssid = "POCCO";
const char* password = "kipasangin1234";

// alamat ip server
const char* server = "monjatir.cloud";
 
void setup() {
  Serial.begin(9600); 
  pinMode(relayKipas, OUTPUT);
  pinMode(relayAtomizer, OUTPUT);

  // Koneksi Ke Wifi
  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, password);

  // uji koneksi wifi
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi gagal terkoneksi");
    delay(500);
  }

  // apabila berhasil konek
    Serial.println("Wifi berhasil terkoneksi");
}

void loop() {

  float t = dht22.getTemperature();
  float h = dht22.getHumidity();
  String kipas;
  String atomizer;

  Serial.print("Kelembapan: ");
  Serial.print(h, 1);
  Serial.print("\t");
  Serial.print("Suhu: ");
  Serial.println(t, 1);

  // Check if temperature is above 30 degrees Celsius
  if (t > 30.0) {
    digitalWrite(relayKipas, LOW); // Turn on the fan relay
    Serial.println("Kipas dinyalakan."); // Print message that fan is turned on
    kipas = "Hidup";
  } else {
    digitalWrite(relayKipas, HIGH); // Turn off the fan relay
    Serial.println("Kipas dimatikan."); // Print message that fan is turned off
    kipas = "Mati";
  }

  // Control relayAtomizer based on humidity
  if (h < 80.0) {
    digitalWrite(relayAtomizer, LOW); // Turn on the atomizer relay
    Serial.println("Atomizer dinyalakan."); // Print message that atomizer is turned on
    atomizer = "Hidup";
  } else {
    digitalWrite(relayAtomizer, HIGH); // Turn off the atomizer relay
    Serial.println("Atomizer dimatikan."); // Print message that atomizer is turned off
    atomizer = "Mati";
  }

  // Pengiriman data sensor
  WiFiClient wClient ;
  // Cek koneksi
  const int httpPort = 80;
  if(!wClient.connect(server, httpPort)) {
    Serial.println("Gagal Konek Ke Web Server");
    return;
  }

  // Proses pengiriman data
  String url;
  HTTPClient http; // protokol http
  url = "http://" + String(server) + "/simpan/" + String(t) + "/" + String(h) + "/" + kipas + "/" + atomizer;

  // Eksekusi url
  http.begin(wClient, url);
  http.GET();
  http.end();

  delay(5000); //Collecting period should be : >1.7 second
}
