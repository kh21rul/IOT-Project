#include <Fuzzy.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DO_PIN 4 // Ubah sesuai dengan pin yang Anda gunakan untuk sensor suhu DS18B20

OneWire oneWire(DO_PIN);
DallasTemperature sensors(&oneWire);

// Instantiating a Fuzzy object
Fuzzy *fuzzy = new Fuzzy();

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  FuzzyInput *temperature = new FuzzyInput(1);
  FuzzySet *cold = new FuzzySet(25, 28, 28, 30);
  temperature->addFuzzySet(cold);
  FuzzySet *normal = new FuzzySet(28, 30, 31, 32);
  temperature->addFuzzySet(normal);
  FuzzySet *hot = new FuzzySet(31, 32, 35, 35);
  temperature->addFuzzySet(hot);
  fuzzy->addFuzzyInput(temperature);

  FuzzyOutput *quality = new FuzzyOutput(1);
  FuzzySet *moderate = new FuzzySet(25, 40, 40, 50);
  quality->addFuzzySet(moderate);
  FuzzySet *good = new FuzzySet(40, 50, 50, 60);
  quality->addFuzzySet(good);
  FuzzySet *bad = new FuzzySet(50, 60, 80, 80);
  quality->addFuzzySet(bad);
  fuzzy->addFuzzyOutput(quality);

  FuzzyRuleAntecedent *ifNormal = new FuzzyRuleAntecedent();
  ifNormal->joinSingle(normal);
  FuzzyRuleConsequent *thenGood = new FuzzyRuleConsequent();
  thenGood->addOutput(good);
  FuzzyRule *ruleNormalGood = new FuzzyRule(1, ifNormal, thenGood);
  fuzzy->addFuzzyRule(ruleNormalGood);

  FuzzyRuleAntecedent *ifCold = new FuzzyRuleAntecedent();
  ifCold->joinSingle(cold);
  FuzzyRuleConsequent *thenModerate = new FuzzyRuleConsequent();
  thenModerate->addOutput(moderate);
  FuzzyRule *ruleColdModerate = new FuzzyRule(2, ifCold, thenModerate);
  fuzzy->addFuzzyRule(ruleColdModerate);

  FuzzyRuleAntecedent *ifHot = new FuzzyRuleAntecedent();
  ifHot->joinSingle(hot);
  FuzzyRuleConsequent *thenBad = new FuzzyRuleConsequent();
  thenBad->addOutput(bad);
  FuzzyRule *ruleHotBad = new FuzzyRule(3, ifHot, thenBad);
  fuzzy->addFuzzyRule(ruleHotBad);
}

void loop()
{
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  Serial.println("\n\n\nEntrance: ");
  Serial.print("\t\t\tTemperature: ");
  Serial.println(temperature);

  fuzzy->setInput(1, temperature);
  fuzzy->fuzzify();
  float qualityValue = fuzzy->defuzzify(1);

  Serial.println("Result: ");
  Serial.print("\t\t\tWater Quality Value: ");
  Serial.println(qualityValue);

  String waterQuality;
  if (qualityValue >= 50)
  {
    waterQuality = "Bad";
  }
  else if (qualityValue >= 40)
  {
    waterQuality = "Moderate";
  }
  else
  {
    waterQuality = "Good";
  }

  Serial.print("\t\t\tWater Quality: ");
  Serial.println(waterQuality);

  delay(5000);
}
