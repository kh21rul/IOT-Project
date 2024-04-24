const int relayWaterPump = 13; // Pin digital untuk mengontrol modul relay Water Pump
const int relayAerator = 12; // Pin digital untuk mengontrol modul relay Aerator

void setup() {
  pinMode(relayWaterPump, OUTPUT); // Mengatur pin sebagai output
  pinMode(relayAerator, OUTPUT); // Mengatur pin sebagai output
  Serial.begin(115200); // Memulai komunikasi serial dengan baud rate 9600
}

void loop() {
  digitalWrite(relayWaterPump, HIGH); // Mengaktifkan relay
  digitalWrite(relayAerator, LOW); // Mengaktifkan relay
  Serial.println("Pompa Air Hidup, Aerator Mati");
  delay(5000); 
  
  digitalWrite(relayWaterPump, LOW); // Matikan relay
  digitalWrite(relayAerator, HIGH); // Matikan relay
  Serial.println("Pompa Air Mati, Aerator Hidup");
  delay(3000); 
}