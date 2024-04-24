#include <Fuzzy.h>
#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#endif

char ssid[] = "Wifi Gratis";            // your network SSID (name)
char pass[] = "21092001";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "monitoring.kh21rul.site";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

// Initialize the Ethernet client object
WiFiEspClient client;

// Fuzzy
Fuzzy *fuzzy = new Fuzzy();

FuzzySet *lowpH = new FuzzySet(1, 1, 7, 7);
FuzzySet *goodpH = new FuzzySet(7, 8, 8, 8.5);
FuzzySet *optimalpH = new FuzzySet(8, 8, 8.5, 8.5);
FuzzySet *highpH = new FuzzySet(8.5, 10, 10, 10);

// FuzzyInput for DO (Dissolved Oxygen)
FuzzySet *lowDO = new FuzzySet(0, 0, 0, 4);
FuzzySet *optimalDO = new FuzzySet(2, 4.5, 4.5, 7);
FuzzySet *highDO = new FuzzySet(5, 10, 10, 10);

// FuzzyInput for temperature
FuzzySet *lowTemp = new FuzzySet(1, 1, 28, 28);
FuzzySet *optimalTemp = new FuzzySet(28, 29.5, 29.5, 30);
FuzzySet *goodTemp = new FuzzySet(29.5, 31, 31, 31);
FuzzySet *highTemp = new FuzzySet(31, 50, 50, 50);

// FuzzyInput for turbidity
FuzzySet *goodTurbidity = new FuzzySet(25, 25, 200, 200);
FuzzySet *optimalTurbidity = new FuzzySet(200, 300, 300, 400);
FuzzySet *highTurbidity = new FuzzySet(400, 600, 600, 600);

// FuzzyOutput for water quality 
FuzzySet *poorQuality = new FuzzySet(0, 0, 0, 70);
FuzzySet *averageQuality = new FuzzySet(50, 70, 70, 90);
FuzzySet *goodQuality = new FuzzySet(80, 100, 100, 100);

void setup()
{
  Serial.begin(115200);
  // randomSeed(analogRead(0));
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    // Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    // Serial.print("Attempting to connect to WPA SSID: ");
    // Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  
  // printWifiStatus();

  // FuzzyInput
  FuzzyInput *pHInput = new FuzzyInput(1);
  pHInput->addFuzzySet(lowpH);
  pHInput->addFuzzySet(goodpH);
  pHInput->addFuzzySet(optimalpH);
  pHInput->addFuzzySet(highpH);
  fuzzy->addFuzzyInput(pHInput);

  // FuzzyInput
  FuzzyInput *DOInput = new FuzzyInput(2);
  DOInput->addFuzzySet(lowDO);
  DOInput->addFuzzySet(optimalDO);
  DOInput->addFuzzySet(highDO);
  fuzzy->addFuzzyInput(DOInput);

  // FuzzyInput
  FuzzyInput *tempInput = new FuzzyInput(3);
  tempInput->addFuzzySet(lowTemp);
  tempInput->addFuzzySet(optimalTemp);
  tempInput->addFuzzySet(goodTemp);
  tempInput->addFuzzySet(highTemp);
  fuzzy->addFuzzyInput(tempInput);

  // FuzzyInput
  FuzzyInput *turbidityInput = new FuzzyInput(4);
  turbidityInput->addFuzzySet(goodTurbidity);
  turbidityInput->addFuzzySet(optimalTurbidity);
  turbidityInput->addFuzzySet(highTurbidity);
  fuzzy->addFuzzyInput(turbidityInput);

  // FuzzyOutput
  FuzzyOutput *waterQuality = new FuzzyOutput(1);
  waterQuality->addFuzzySet(poorQuality);
  waterQuality->addFuzzySet(averageQuality);
  waterQuality->addFuzzySet(goodQuality);
  fuzzy->addFuzzyOutput(waterQuality);

// Rules poor water
FuzzyRuleAntecedent *rule1_antecedent = new FuzzyRuleAntecedent();
rule1_antecedent->joinWithOR(lowpH, highpH); // pH < 7.7 OR pH > 8.5

FuzzyRuleAntecedent *rule2_antecedent = new FuzzyRuleAntecedent();
rule2_antecedent->joinSingle(lowDO); // DO < 4

FuzzyRuleAntecedent *rule3_antecedent = new FuzzyRuleAntecedent();
rule3_antecedent->joinWithOR(lowTemp, highTemp); // Temperature < 29 OR Temperature > 31

FuzzyRuleAntecedent *rule4_antecedent = new FuzzyRuleAntecedent();
rule4_antecedent->joinSingle(highTurbidity); // Turbidity > 400

FuzzyRuleConsequent *rule_consequent_poor = new FuzzyRuleConsequent();
rule_consequent_poor->addOutput(poorQuality); // Set output to poor quality


// Rules avarage water
FuzzyRuleAntecedent *rule5_antecedent = new FuzzyRuleAntecedent();
rule5_antecedent->joinSingle(optimalpH); // pH is optimal

FuzzyRuleAntecedent *rule6_antecedent = new FuzzyRuleAntecedent();
rule6_antecedent->joinSingle(optimalDO); // DO is optimal or high

FuzzyRuleAntecedent *rule7_antecedent = new FuzzyRuleAntecedent();
rule7_antecedent->joinSingle(optimalTemp); // Temperature is optimal

FuzzyRuleAntecedent *rule8_antecedent = new FuzzyRuleAntecedent();
rule8_antecedent->joinSingle(optimalTurbidity); // Turbidity is low or optimal

FuzzyRuleConsequent *rule_consequent_avarage = new FuzzyRuleConsequent();
rule_consequent_avarage->addOutput(averageQuality); // Set output to avarage quality


// Rules good water
FuzzyRuleAntecedent *rule9_antecedent = new FuzzyRuleAntecedent();
rule9_antecedent->joinSingle(goodpH); // pH is good

FuzzyRuleAntecedent *rule10_antecedent = new FuzzyRuleAntecedent();
rule10_antecedent->joinSingle(highDO); // DO is high

FuzzyRuleAntecedent *rule11_antecedent = new FuzzyRuleAntecedent();
rule11_antecedent->joinSingle(goodTemp); // Temperature good

FuzzyRuleAntecedent *rule12_antecedent = new FuzzyRuleAntecedent();
rule12_antecedent->joinSingle(goodTurbidity); // Turbidity is good

FuzzyRuleConsequent *rule_consequent_good = new FuzzyRuleConsequent();
rule_consequent_good->addOutput(goodQuality); // Set output to good quality


FuzzyRule *rule1 = new FuzzyRule(1, rule1_antecedent, rule_consequent_poor);
FuzzyRule *rule2 = new FuzzyRule(2, rule2_antecedent, rule_consequent_poor);
FuzzyRule *rule3 = new FuzzyRule(3, rule3_antecedent, rule_consequent_poor);
FuzzyRule *rule4 = new FuzzyRule(4, rule4_antecedent, rule_consequent_poor);
FuzzyRule *rule5 = new FuzzyRule(5, rule5_antecedent, rule_consequent_avarage);
FuzzyRule *rule6 = new FuzzyRule(6, rule6_antecedent, rule_consequent_avarage);
FuzzyRule *rule7 = new FuzzyRule(7, rule7_antecedent, rule_consequent_avarage);
FuzzyRule *rule8 = new FuzzyRule(8, rule8_antecedent, rule_consequent_avarage);
FuzzyRule *rule9 = new FuzzyRule(9, rule9_antecedent, rule_consequent_good);
FuzzyRule *rule10 = new FuzzyRule(10, rule10_antecedent, rule_consequent_good);
FuzzyRule *rule11 = new FuzzyRule(11, rule11_antecedent, rule_consequent_good);
FuzzyRule *rule12 = new FuzzyRule(12, rule12_antecedent, rule_consequent_good);

fuzzy->addFuzzyRule(rule1);
fuzzy->addFuzzyRule(rule2);
fuzzy->addFuzzyRule(rule3);
fuzzy->addFuzzyRule(rule4);
fuzzy->addFuzzyRule(rule5);
fuzzy->addFuzzyRule(rule6);
fuzzy->addFuzzyRule(rule7);
fuzzy->addFuzzyRule(rule8);
fuzzy->addFuzzyRule(rule9);
fuzzy->addFuzzyRule(rule10);
fuzzy->addFuzzyRule(rule11);
fuzzy->addFuzzyRule(rule12);
}

void loop()
{
  // float pHValue = random(2, 10);
  // float DOValue = random(2, 12);
  // float turbidityValue = random(25, 600);
  // float tempValue = random(25, 40);

  float pHValue = 7.5;
  float DOValue = 8;
  float turbidityValue = 300;
  float tempValue = 30.5;

  fuzzy->setInput(1, pHValue);
  fuzzy->setInput(2, DOValue);
  fuzzy->setInput(3, tempValue);
  fuzzy->setInput(4, turbidityValue);

  fuzzy->fuzzify();

  // Defuzzify and Get Results
  float waterQualityResult = fuzzy->defuzzify(1);

  // Determine the quality of water based on the result
  String waterQualityString;
  String sistemKendali;

  if (waterQualityResult < 50) {
    waterQualityString = "Buruk";
    sistemKendali = "Hidup";
  } else if (waterQualityResult < 80) {
    waterQualityString = "Cukup";
    sistemKendali = "Mati";
  } else {
    waterQualityString = "Baik";
    sistemKendali = "Mati";
  }

   String data = String(tempValue, 2) + "/" +
              String(turbidityValue, 2) + "/" +
              String(pHValue, 2) + "/" +
              String(DOValue, 2) + "/" +
              waterQualityString + "/" +
              sistemKendali;

  // Serial.print("pH Value: ");
  // Serial.println(pHValue, 1);
  // Serial.print("DO Value: ");
  // Serial.println(DOValue, 1);
  // Serial.print("Turbidity Value: ");
  // Serial.println(turbidityValue, 1);
  // Serial.print("Temperature Value: ");
  // Serial.println(tempValue, 1);
  // Serial.print("Kualitas Air: ");
  // Serial.println(waterQualityString);
  // Serial.print("Sitem Kendali: ");
  // Serial.println(sistemKendali);
  // Serial.println();

  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest(data);
  }

  // wait 10 seconds
  delay(10000);
}

// this method makes a HTTP connection to the server
void httpRequest(String data)
{
  Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    Serial.println("Connecting...");
    
    // send the HTTP GET request with the provided data
    client.print(F("GET /simpan/"));
    client.print(data);
    client.println(F(" HTTP/1.1"));
    client.println(F("Host: monitoring.kh21rul.site"));
    client.println(F("Connection: close"));
    client.println();

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}
