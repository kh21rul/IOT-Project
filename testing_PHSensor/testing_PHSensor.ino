const int ph_Pin = A1; // Pin analog untuk membaca sensor pH
float Po = 0;
float PH_step;
int nilai_analog_PH;
double TeganganPh;

float PH4 = 3.6;
float PH7 = 3.3;

void setup() {
  pinMode (ph_Pin, INPUT);
  Serial.begin(115600); // Mulai serial monitor
}

void loop() {
  int nilai_analog_PH = analogRead(ph_Pin);
  Serial.print("Nilai ADC Ph: ");
  Serial.print(nilai_analog_PH);
  TeganganPh = 5 / 1024.0 * nilai_analog_PH;
  Serial.print("\tTeganganPh: ");
  Serial.print(TeganganPh, 3);

  PH_step = (PH4 - PH7) / 3;
  Po = 7.00 + ((PH7 - TeganganPh) / PH_step);
  Serial.print("\tNilai PH Air: ");
  Serial.println(Po, 2);
  delay(1000); // Tunda 1 detik sebelum membaca data berikutnya
}
