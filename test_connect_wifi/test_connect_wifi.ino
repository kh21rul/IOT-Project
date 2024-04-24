#include<KRwifi.h>
char* ssid = "Wifi Gratis";
char* pass = "12345678";
 
void setup() {
  Serial.begin(9600);
  setWifi(ssid, pass);
}
 
void loop() {
 
}