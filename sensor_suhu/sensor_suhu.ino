#include <OneWire.h>
#include <DallasTemperature.h>

const int pinData = 4; // Ubah sesuai dengan pin yang Anda gunakan
OneWire oneWire(pinData);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200); // Mulai serial monitor
  sensors.begin();   // Mulai komunikasi dengan sensor
}

void loop() {
  sensors.requestTemperatures(); // Minta sensor untuk membaca suhu
  float temperature = sensors.getTempCByIndex(0); // Baca suhu dalam Celsius
  Serial.print("Suhu pada air sekarang : ");
  Serial.print(temperature);
  Serial.println(" °C");
  delay(1000); // Tunda 1 detik sebelum membaca suhu berikutnya
}