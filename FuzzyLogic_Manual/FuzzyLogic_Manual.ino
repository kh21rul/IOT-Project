float randompH, randomDO, randomTemp, randomTurbidity;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  // randompH = random(10) + 1;  // Generate random pH between 1 to 10
  // randomDO = random(10);  // Generate random DO between 0 to 10
  // randomTemp = random(50) + 1;  // Generate random Temperature between 1 to 50
  // randomTurbidity = random(600) + 25;  // Generate random Turbidity between 25 to 600

  randompH = 7;
  randomDO = 5;
  randomTemp = 30;
  randomTurbidity = 100;

  // Fuzzification
  float lowpH, optimalpH, goodpH, highpH;
  float lowDO, optimalDO, goodDO;
  float lowTemp, optimalTemp, goodTemp, highTemp;
  float goodTurbidity, optimalTurbidity, highTurbidity;
  
  // Fuzzification for pH
  // Use min-max scaling to scale the input to values between 0 and 1
  lowpH = max(0, min(1, (7 - randompH) / 6));
  optimalpH = max(0, min(1, (randompH - 7) / 0.7));
  goodpH = max(0, min(1, (8.5 - randompH) / 0.8));
  highpH = max(0, min(1, (randompH - 8.5) / 1.5));

  // Fuzzification for DO
  // Similar process for DO
  lowDO = max(0, min(1, (4 - randomDO) / 4));
  optimalDO = max(0, min(1, (randomDO - 4) / 3));
  goodDO = max(0, min(1, (randomDO - 7) / 3));

  // Fuzzification for Temperature
  // Similar process for Temperature
  lowTemp = max(0, min(1, (21 - randomTemp) / 20));
  optimalTemp = max(0, min(1, (randomTemp - 21) / 8));
  goodTemp = max(0, min(1, (32 - randomTemp) / 3));
  highTemp = max(0, min(1, (randomTemp - 32) / 18));

  // Fuzzification for Turbidity
  // Similar process for Turbidity
  goodTurbidity = max(0, min(1, (200 - randomTurbidity) / 175));
  optimalTurbidity = max(0, min(1, (randomTurbidity - 200) / 200));
  highTurbidity = max(0, min(1, (randomTurbidity - 400) / 200));

  // Rule Base
  float poorWater, averageWater, goodWater;
  
  // Inferencing
  // Using max-min inference method
  poorWater = max(max(max(max(max(lowpH, highpH), lowDO), lowTemp), highTemp), highTurbidity);
  averageWater = max(max(max(optimalpH, optimalDO), optimalTemp), optimalTurbidity);
  goodWater = max(max(goodpH, goodDO), max(goodTemp, goodTurbidity));

  // Defuzzification
  // Using centroid method for defuzzification
  float waterQuality = (poorWater * 50 + averageWater * 80 + goodWater * 100) / (poorWater + averageWater + goodWater);

  // Print the results
  Serial.print("pH Air : "); Serial.println(randompH);
  Serial.print("DO : "); Serial.println(randomDO);
  Serial.print("Temperature : "); Serial.println(randomTemp);
  Serial.print("Turbidity : "); Serial.println(randomTurbidity);
  Serial.println();
  Serial.print("Water Kuality : "); Serial.println(waterQuality);
  Serial.print("Kualitas Air : ");
  if (waterQuality <= 50) {
    Serial.println("Buruk");
  } else if (waterQuality <= 80) {
    Serial.println("Cukup");
  } else {
    Serial.println("Baik");
  }
  Serial.println();
  
  delay(2000); // Wait for 2 seconds before the next iteration
}