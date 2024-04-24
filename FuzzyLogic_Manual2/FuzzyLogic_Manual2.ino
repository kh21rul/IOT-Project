// variabel fuzzy input
float lowpH[2]  = {1, 7};
float optimalpH[2] = {7, 7.7};
float goodpH[2] = {7.7, 8.5};
float highpH[2] = {8.5, 10};

float lowDO[2] = {0, 4};
float optimalDO[2] = {4, 7};
float goodDO[2] = {7, 10};

float lowTemp[2] = {1, 21};
float optimalTemp[2] = {21, 29};
float goodTemp[2] = {29, 32};
float highTemp[2] = {32, 50};

float goodTurbidity[2] = {25, 200};
float optimalTurbidity[2] = {200, 400};
float highTurbidity[2] = {400, 600};

// variabel fuzzy output
float poorWater[2]  = {0, 50};
float avarageWater[2] = {50, 80};
float goodWater[2] = {70, 100};

// Variabel untuk menyimpan nilai sensor
float pH, DO, Temp, Turbidity;

// Variabel untuk menyimpan nilai fuzzy output
float poorWaterValue, averageWaterValue, goodWaterValue;

void setup() {
  Serial.begin(115200);
}

void loop() {
  // Ambil nilai random untuk sensor
  // pH = random(1, 10);
  // DO = random(0, 10);
  // Temp = random(1, 50);
  // Turbidity = random(25, 600);

  pH = 8;
  DO = 8;
  Temp = 30;
  Turbidity = 180;

  // Proses Fuzzyfikasi
  float pHmembership[4] = {fuzzyInput(pH, lowpH), fuzzyInput(pH, optimalpH), fuzzyInput(pH, goodpH), fuzzyInput(pH, highpH)};
  float DOmembership[3] = {fuzzyInput(DO, lowDO), fuzzyInput(DO, optimalDO), fuzzyInput(DO, goodDO)};
  float Tempmembership[4] = {fuzzyInput(Temp, lowTemp), fuzzyInput(Temp, optimalTemp), fuzzyInput(Temp, goodTemp), fuzzyInput(Temp, highTemp)};
  float Turbiditymembership[3] = {fuzzyInput(Turbidity, goodTurbidity), fuzzyInput(Turbidity, optimalTurbidity), fuzzyInput(Turbidity, highTurbidity)};

  // Rule Base dan Inferensi Fuzzy
  poorWaterValue = max(pHmembership[0],max(pHmembership[3],max(DOmembership[0],max(Tempmembership[0],max(Tempmembership[3],Turbiditymembership[2])))));
  averageWaterValue = min(min(pHmembership[1],DOmembership[1]), min(Tempmembership[1],Turbiditymembership[1]));
  goodWaterValue = min(min(pHmembership[2],DOmembership[2]), min(Tempmembership[2],Turbiditymembership[0]));

  // Defuzzifikasi
  float waterQuality = defuzzification(poorWaterValue, averageWaterValue, goodWaterValue);
  
  // Cetak hasil
  Serial.print("pH Air : ");
  Serial.println(pH);
  Serial.print("DO : ");
  Serial.println(DO);
  Serial.print("Temperature : ");
  Serial.println(Temp);
  Serial.print("Turbidity : ");
  Serial.println(Turbidity);
  Serial.print("Fuzzy Output : ");
  Serial.println(waterQuality);
  Serial.print("\nKualitas Air : ");
  if (waterQuality <= 50) Serial.println("Buruk");
  else if (waterQuality <= 80) Serial.println("Cukup");
  else Serial.println("Baik");
  
  delay(1000);
}

float fuzzyInput(float value, float* fuzzySet) {
  if (value <= fuzzySet[0]) return 0;
  else if (value <= fuzzySet[1]) return (value - fuzzySet[0]) / (fuzzySet[1] - fuzzySet[0]);
  else return 0;
}

float defuzzification(float poor, float average, float good) {
  float top = poor*poorWater[1] + average*avarageWater[1] + good*goodWater[1];
  float bottom = poor + average + good;
  return top / bottom;
}