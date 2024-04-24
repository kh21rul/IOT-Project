#include <DHT22.h>
//define pin data dht22
#define pinDATA 13 // SDA, or almost any other I/O pin
// Mendefinisikan pin relay
const int relayKipas = 16;
const int relayAtomizer = 5;

DHT22 dht22(pinDATA); 
 
void setup() {
  Serial.begin(9600); 
  pinMode(relayKipas, OUTPUT);
  pinMode(relayAtomizer, OUTPUT);
}

void loop() {

  float t = dht22.getTemperature();
  float h = dht22.getHumidity();

  Serial.print("Kelembapan: ");
  Serial.print(h, 1);
  Serial.print("\t");
  Serial.print("Suhu: ");
  Serial.println(t, 1);

  // Check if temperature is above 30 degrees Celsius
  if (t > 30.0) {
    digitalWrite(relayKipas, LOW); // Turn on the fan relay
    Serial.println("Kipas dinyalakan."); // Print message that fan is turned on
  } else {
    digitalWrite(relayKipas, HIGH); // Turn off the fan relay
    Serial.println("Kipas dimatikan."); // Print message that fan is turned off
  }

  // Control relayAtomizer based on humidity
  if (h < 80.0) {
    digitalWrite(relayAtomizer, LOW); // Turn on the atomizer relay
    Serial.println("Atomizer dinyalakan."); // Print message that atomizer is turned on
  } else if (h > 90.0) {
    digitalWrite(relayAtomizer, HIGH); // Turn off the atomizer relay
    Serial.println("Atomizer dimatikan."); // Print message that atomizer is turned off
  }

  delay(2000); //Collecting period should be : >1.7 second
}
