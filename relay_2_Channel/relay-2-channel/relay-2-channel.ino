// Mendefinisikan pin relay
const int relayPin1 = 16;
const int relayPin2 = 5;

void setup() {
  // Mengatur pin sebagai output
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  // Memastikan relay dimatikan pada awalnya
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
}

void loop() {
  // Menghidupkan relay 1 dan mematikan relay 2
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, LOW);
  delay(1000); // Waktu tunggu 1 detik

  // Menghidupkan relay 2 dan mematikan relay 1
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, HIGH);
  delay(1000); // Waktu tunggu 1 detik
}
