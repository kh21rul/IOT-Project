// Deklarasi keanggotaan himpunan suhu
float suhuDingin[] = {28, 33};     // <28, 28, 33
float suhuNormal[] = {28, 33, 38}; // 28, 33, 38
float suhuPanas[] = {33, 38};      // 33, 38, 38>

// Deklarasi keanggotaan himpunan kelembapan
float kelembapanKering[] = {75, 85};     // <75, 75, 85
float kelembapanNormal[] = {75, 85, 95}; // 75, 85, 95
float kelembapanBasah[] = {85, 95};      // 85, 95, 95>

// deklarasi Keanggotaan himpunan output kipas
float kipasMati[] = {1600, 3200};  // <1600, 1600, 3200
float kipasHidup[] = {1600, 3200}; // 1600, 3200, 3200>

// deklarasi Keanggotaan himpunan output atomizer
float atomizerMati[] = {850, 1700};  // <850, 850, 1.700
float atomizerHidup[] = {850, 1700}; // 850, 1700, 1700>

float MUsuhuDin, MUsuhuNor, MUsuhuPan; // deklarasi variabel derajat keanggotaan suhu
float MUkelembapanKer, MUkelembapanNor, MUkelembapanBas; // deklarasi variabel derajat keanggotaan kelembapan

float R1kipas, R2kipas, R3kipas;             // deklarasi variabel Rules untuk pengendalian Kipas
float Z1kipas, Z2kipas, Z3kipas;             // deklarasi variabel Rules untuk pengendalian Kipas

float R1atomizer, R2atomizer, R3atomizer; // deklarasi variabel Rules untuk pengendalian Atomizer
float Z1atomizer, Z2atomizer, Z3atomizer; // deklarasi variabel Rules untuk pengendalian Atomizer

float OutputKipas, OutputAtomizer;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // Input nilai Variabel
  float suhuValue = 32;
  float kelembapanValue = 79;

  // Hitung Fuzzy Output Kipas menggunakan fungsi
  OutputKipas = fuzzyTsukamotoKipas(suhuValue);
  // Hitung Fuzzy Output Atomizer menggunakan fungsi
  OutputAtomizer = fuzzyTsukamotoAtomizer(kelembapanValue);

  Serial.print("Suhu: ");
  Serial.println(suhuValue);
  Serial.print("Kelembapan: ");
  Serial.println(kelembapanValue);

  Serial.print("Fuzzy Output Kipas: ");
  Serial.println(OutputKipas);
  Serial.print("Fuzzy Output Atomizer: ");
  Serial.println(OutputAtomizer);

  Serial.println();

  delay(2000);
}

float fuzzyTsukamotoKipas(float suhuValue) {
  // fuzzifikasi
  // Mencari Fungsi Keanggotaan Oksigen Rendah
  if (suhuValue >= suhuDingin[1])
  {
    MUsuhuDin = 0;
  }
  else if (suhuValue > suhuDingin[0] && suhuValue < suhuDingin[1])
  {
    MUsuhuDin = (suhuDingin[1] - suhuValue) / (suhuDingin[1] - suhuDingin[0]);
  }
  else if (suhuValue <= suhuDingin[0])
  {
    MUsuhuDin = 1;
  }

  // Mencari Fungsi Keanggotaan Suhu Normal
  if (suhuValue <= suhuNormal[0] || suhuValue >= suhuNormal[2])
  {
    MUsuhuNor = 0;
  }
  else if (suhuValue > suhuNormal[0] && suhuValue < suhuNormal[1])
  {
    MUsuhuNor = (suhuValue - suhuNormal[0]) / (suhuNormal[1] - suhuNormal[0]);
  }
  else if (suhuValue >= suhuNormal[1] && suhuValue < suhuNormal[2])
  {
    MUsuhuNor = (suhuNormal[2] - suhuValue) / (suhuNormal[2] - suhuNormal[1]);
  }

  // Mencari Fungsi Keanggotaan Suhu Panas
  if (suhuValue <= suhuPanas[0])
  {
    MUsuhuPan = 0;
  }
  else if (suhuValue > suhuPanas[0] && suhuValue < suhuPanas[1])
  {
    MUsuhuPan = (suhuValue - suhuPanas[0]) / (suhuPanas[1] - suhuPanas[0]);
  }
  else if (suhuValue >= suhuPanas[1])
  {
    MUsuhuPan = 1;
  }

  // Proses Inferensi Kendali kipas
  // [R1] Jika suhu dingin maka kipas mati
  R1kipas = MUsuhuDin;
  Z1kipas = kipasMati[1] - ((kipasMati[1] - kipasMati[0]) * R1kipas);

  // [R2] Jika suhu normal maka kipas mati
  R2kipas = MUsuhuNor;
  Z2kipas = kipasMati[1] - ((kipasMati[1] - kipasMati[0]) * R2kipas);

  // [R3] Jika suhu panas maka kipas hidup
  R3kipas = MUsuhuPan;
  Z3kipas = kipasHidup[0] + ((kipasHidup[1] - kipasHidup[0]) * R3kipas);

  // Defuzzyfikasi Kendali Kipas
  OutputKipas = ((Z1kipas * R1kipas) + (Z2kipas * R2kipas) + (Z3kipas * R3kipas)) / (R1kipas + R2kipas + R3kipas);

  return OutputKipas;
}

float fuzzyTsukamotoAtomizer(float kelembapanValue) {
  // fuzzifikasi
  // Mencari Fungsi Keanggotaan Kelembapan Kering
  if (kelembapanValue >= kelembapanKering[1])
  {
    MUkelembapanKer = 0;
  }
  else if (kelembapanValue > kelembapanKering[0] && kelembapanValue < kelembapanKering[1])
  {
    MUkelembapanKer = (kelembapanKering[1] - kelembapanValue) / (kelembapanKering[1] - kelembapanKering[0]);
  }
  else if (kelembapanValue <= kelembapanKering[0])
  {
    MUkelembapanKer = 1;
  }

  // Mencari Fungsi Keanggotaan Kelembapan Normal
  if (kelembapanValue <= kelembapanNormal[0] || kelembapanValue >= kelembapanNormal[2])
  {
    MUkelembapanNor = 0;
  }
  else if (kelembapanValue > kelembapanNormal[0] && kelembapanValue < kelembapanNormal[1])
  {
    MUkelembapanNor = (kelembapanValue - kelembapanNormal[0]) / (kelembapanNormal[1] - kelembapanNormal[0]);
  }
  else if (kelembapanValue >= kelembapanNormal[1] && kelembapanValue < kelembapanNormal[2])
  {
    MUkelembapanNor = (kelembapanNormal[2] - kelembapanValue) / (kelembapanNormal[2] - kelembapanNormal[1]);
  }

  // Mencari Fungsi Keanggotaan Kelembapan Basah
  if (kelembapanValue <= kelembapanBasah[0])
  {
    MUkelembapanBas = 0;
  }
  else if (kelembapanValue > kelembapanBasah[0] && kelembapanValue < kelembapanBasah[1])
  {
    MUkelembapanBas = (kelembapanValue - kelembapanBasah[0]) / (kelembapanBasah[1] - kelembapanBasah[0]);
  }
  else if (kelembapanValue >= kelembapanBasah[1])
  {
    MUkelembapanBas = 1;
  }

  // Proses Inferensi Kendali atomizer
  // [R1] Jika kelembapan kering maka atomizer hidup
  R1atomizer = MUkelembapanKer;
  Z1atomizer = atomizerHidup[0] + ((atomizerHidup[1] - atomizerHidup[0]) * R1atomizer);

  // [R2] Jika kelembapan normal maka atomizer mati
  R2atomizer = MUkelembapanNor;
  Z2atomizer = atomizerMati[1] - ((atomizerMati[1] - atomizerMati[0]) * R2atomizer);

  // [R3] Jika kelembapan basah maka atomizer mati
  R3atomizer = MUkelembapanBas;
  Z3atomizer = atomizerMati[1] - ((atomizerMati[1] - atomizerMati[0]) * R3atomizer);

  // Defuzzyfikasi Kendali Atomizer
  OutputAtomizer = ((Z1atomizer * R1atomizer) + (Z2atomizer * R2atomizer) + (Z3atomizer * R3atomizer)) / (R1atomizer + R2atomizer + R3atomizer);

  return OutputAtomizer;
}
