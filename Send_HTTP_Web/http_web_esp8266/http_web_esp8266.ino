#include <ESP8266WiFi.h> // include library DHT22 Wifi
#include <ESP8266HTTPClient.h>

// konfigurasi Wifi
const char* ssid = "Sejak Tahun 1975";
const char* password = "merdekabelajar";

// alamat ip server
const char* server = "monjamer.online";
 
void setup() {
  Serial.begin(9600); 

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

  // Pengiriman data sensor
  WiFiClient wClient ;
  // Cek koneksi ke web server
  const int httpPort = 80;
  if(!wClient.connect(server, httpPort)) 
  {
    Serial.println("Gagal Konek Ke Web Server");
    return;
  }

  // Proses pengiriman data
  String url;
  HTTPClient http; // protokol http
  url = "http://" + String(server) + "/simpan/20/32/huhu/haha";
  // url = "https://hydroponic.fun/pages/urlData/30/30/Hallo/Hallo/Hallo";

  // Eksekusi url
  http.begin(wClient, url);
  http.GET();
  http.end();
  Serial.println("Berhasil mengirim data");

  delay(2000); //Collecting period should be : >1.7 second
}
