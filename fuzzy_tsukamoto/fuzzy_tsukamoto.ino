// deklarasi Keanggotaan himpunan oksigen
float oksigenRendah[] = {4, 8}; // <4, 4, 8
float oksigenTinggi[] = {4, 8}; // 4, 8, 8>
// deklarasi Keanggotaan himpunan suhu
float suhuDingin[] = {25, 30};     // <25, 25, 30
float suhuNetral[] = {25, 30, 35}; // 25, 30, 35
float suhuPanas[] = {30, 35};      // 30, 35, 35>
// deklarasi Keanggotaan himpunan output aerator
float aeratorMati[] = {500, 1200};  // <500, 500, 1200
float aeratorHidup[] = {500, 1200}; // 500, 1200, 1200>
// deklarasi Keanggotaan himpunan turbidity
float turbiJernih[] = {25, 400}; // <25, 25, 400
float turbiKeruh[] = {25, 400};  // 25, 400, 400>
// deklarasi Keanggotaan himpunan pH
float pHAsam[] = {6, 7};      // <6, 6, 7
float pHNetral[] = {6, 7, 8}; // 6, 7, 8
float pHBasa[] = {7, 8};      // 7, 8, 8>
// deklarasi Keanggotaan himpunan output waterpump
float waterpumpMati[] = {500, 1200};  // <500, 500, 1200
float waterpumpHidup[] = {500, 1200}; // 500, 1200, 1200>

float MUoksigenRen, MUoksigenTin;      // deklarasi variabel derajat keanggotaan oksigen
float MUsuhuDin, MUsuhuNet, MUsuhuPan; // deklarasi variabel derajat keanggotaan suhu
float MUturbiJer, MUturbiKer;          // deklarasi variabel derajat keanggotaan turbidity
float MUpHAs, MUpHNet, MUpHBas;        // deklarasi variabel derajat keanggotaan pH

float R1aerator, R2aerator, R3aerator, R4aerator, R5aerator, R6aerator;             // deklarasi variabel Rules untuk pengendalian Aerator
float Z1aerator, Z2aerator, Z3aerator, Z4aerator, Z5aerator, Z6aerator;             // deklarasi variabel Rules untuk pengendalian Aerator
float R1waterpump, R2waterpump, R3waterpump, R4waterpump, R5waterpump, R6waterpump; // deklarasi variabel Rules untuk pengendalian Water Pump
float Z1waterpump, Z2waterpump, Z3waterpump, Z4waterpump, Z5waterpump, Z6waterpump; // deklarasi variabel Rules untuk pengendalian Water Pump
float OutputAerator, OutputWaterpump;                                               // deklarasi variabel Output Aerator dan Water Pump

void setup()
{
  // Inisialisasi Serial Monitor
  Serial.begin(115200);
}

void loop()
{
  // Input nilai Variabel
  float DOValue = 7;
  float TempValue = 31;
  float TurbiValue = 100;
  float pHValue = 7;

  // Hitung Fuzzy Output Aerator menggunakan fungsi
  OutputAerator = fuzzyTsukamotoAerator(DOValue, TempValue);
  // Hitung Fuzzy Output Water Pump menggunakan fungsi
  OutputWaterpump = fuzzyTsukamotoWaterPump(TurbiValue, pHValue);

  Serial.print("Kekeruhan Air: ");
  Serial.println(TurbiValue);
  Serial.print("pH Air: ");
  Serial.println(pHValue);
  Serial.print("Oksigen Air: ");
  Serial.println(DOValue);
  Serial.print("Suhu Air: ");
  Serial.println(TempValue);
  Serial.print("Fuzzy Output Water Pump: ");
  Serial.println(OutputWaterpump);
  Serial.print("Fuzzy Output Aerator: ");
  Serial.println(OutputAerator);

  Serial.println();

  delay(2000);
}

// Deklarasi fungsi untuk menghitung Fuzzy Tsukamoto Aerator
float fuzzyTsukamotoAerator(float DOValue, float TempValue)
{
  // Proses Fuzzyfikasi Kendali Aerator
  // Mencari Fungsi Keanggotaan Oksigen Rendah
  if (DOValue >= oksigenRendah[1])
  {
    MUoksigenRen = 0;
  }
  else if (DOValue > oksigenRendah[0] && DOValue < oksigenRendah[1])
  {
    MUoksigenRen = (oksigenRendah[1] - DOValue) / (oksigenRendah[1] - oksigenRendah[0]);
  }
  else if (DOValue <= oksigenRendah[0])
  {
    MUoksigenRen = 1;
  }
  // Mencari Fungsi Keanggotaan Oksigen Tinggi
  if (DOValue <= oksigenTinggi[0])
  {
    MUoksigenTin = 0;
  }
  else if (DOValue > oksigenTinggi[0] && DOValue < oksigenTinggi[1])
  {
    MUoksigenTin = (DOValue - oksigenTinggi[0]) / (oksigenTinggi[1] - oksigenTinggi[0]);
  }
  else if (DOValue >= oksigenTinggi[1])
  {
    MUoksigenTin = 1;
  }
  // Mencari Fungsi Keanggotaan Suhu Dingin
  if (TempValue >= suhuDingin[1])
  {
    MUsuhuDin = 0;
  }
  else if (TempValue > suhuDingin[0] && TempValue < suhuDingin[1])
  {
    MUsuhuDin = (suhuDingin[1] - TempValue) / (suhuDingin[1] - suhuDingin[0]);
  }
  else if (TempValue <= suhuDingin[0])
  {
    MUsuhuDin = 1;
  }
  // Mencari Fungsi Keanggotaan Suhu Netral
  if (TempValue <= suhuNetral[0] || TempValue >= suhuNetral[2])
  {
    MUsuhuNet = 0;
  }
  else if (TempValue > suhuNetral[0] && TempValue < suhuNetral[1])
  {
    MUsuhuNet = (TempValue - suhuNetral[0]) / (suhuNetral[1] - suhuNetral[0]);
  }
  else if (TempValue >= suhuNetral[1] && TempValue < suhuNetral[2])
  {
    MUsuhuNet = (suhuNetral[2] - TempValue) / (suhuNetral[2] - suhuNetral[1]);
  }
  // Mencari Fungsi Keanggotaan Suhu Panas
  if (TempValue <= suhuPanas[0])
  {
    MUsuhuPan = 0;
  }
  else if (TempValue > suhuPanas[0] && TempValue < suhuPanas[1])
  {
    MUsuhuPan = (TempValue - suhuPanas[0]) / (suhuPanas[1] - suhuPanas[0]);
  }
  else if (TempValue >= suhuPanas[1])
  {
    MUsuhuPan = 1;
  }

  // Proses Inferensi Kendali aerator
  // [R1] Jika oksigen rendah & suhu dingin, maka aerator hidup
  if (MUoksigenRen < MUsuhuDin)
  {
    R1aerator = MUoksigenRen;
  }
  else if (MUsuhuDin < MUoksigenRen)
  {
    R1aerator = MUsuhuDin;
  }
  Z1aerator = aeratorHidup[0] + ((aeratorHidup[1] - aeratorHidup[0]) * R1aerator); // Mencari Z1 dari Rule 1

  // [R2] Jika oksigen rendah & suhu netral, maka aerator hidup
  if (MUoksigenRen < MUsuhuNet)
  {
    R2aerator = MUoksigenRen;
  }
  else if (MUsuhuNet < MUoksigenRen)
  {
    R2aerator = MUsuhuNet;
  }
  Z2aerator = aeratorHidup[0] + ((aeratorHidup[1] - aeratorHidup[0]) * R2aerator); // Mencari Z2 dari Rule 2

  // [R3] Jika oksigen rendah & suhu panas, maka aerator hidup
  if (MUoksigenRen < MUsuhuPan)
  {
    R3aerator = MUoksigenRen;
  }
  else if (MUsuhuPan < MUoksigenRen)
  {
    R3aerator = MUsuhuPan;
  }
  Z3aerator = aeratorHidup[0] + ((aeratorHidup[1] - aeratorHidup[0]) * R3aerator); // Mencari Z3 dari Rule 3

  // [R4] Jika oksigen tinggi & suhu dingin, maka aerator hidup
  if (MUoksigenTin < MUsuhuDin)
  {
    R4aerator = MUoksigenTin;
  }
  else if (MUsuhuDin < MUoksigenTin)
  {
    R4aerator = MUsuhuDin;
  }
  Z4aerator = aeratorHidup[0] + ((aeratorHidup[1] - aeratorHidup[0]) * R4aerator); // Mencari Z4 dari Rule 4

  // [R5] Jika oksigen tinggi & suhu netral, maka aerator mati
  if (MUoksigenTin < MUsuhuNet)
  {
    R5aerator = MUoksigenTin;
  }
  else if (MUsuhuNet < MUoksigenTin)
  {
    R5aerator = MUsuhuNet;
  }
  Z5aerator = aeratorMati[1] - ((aeratorMati[1] - aeratorMati[0]) * R5aerator); // Mencari Z5 dari Rule 5

  // [R6] Jika oksigen tinggi & suhu panas, maka aerator hidup
  if (MUoksigenTin < MUsuhuPan)
  {
    R6aerator = MUoksigenTin;
  }
  else if (MUsuhuPan < MUoksigenTin)
  {
    R6aerator = MUsuhuPan;
  }
  Z6aerator = aeratorHidup[0] + ((aeratorHidup[1] - aeratorHidup[0]) * R6aerator); // Mencari Z6 dari Rule 6

  // Defuzzyfikasi Kendali Aerator
  OutputAerator = ((Z1aerator * R1aerator) + (Z2aerator * R2aerator) + (Z3aerator * R3aerator) + (Z4aerator * R4aerator) + (Z5aerator * R5aerator) + (Z6aerator * R6aerator)) / (R1aerator + R2aerator + R3aerator + R4aerator + R5aerator + R6aerator);

  return OutputAerator;
}

// Deklarasi fungsi untuk menghitung Fuzzy Tsukamoto Water Pump
float fuzzyTsukamotoWaterPump(float TurbiValue, float pHValue)
{
  // Proses Fuzzyfikasi Kendali Waterpump
  // Mencari Fungsi Keanggotaan Turbidity jernih
  if (TurbiValue >= turbiJernih[1])
  {
    MUturbiJer = 0;
  }
  else if (TurbiValue > turbiJernih[0] && TurbiValue < turbiJernih[1])
  {
    MUturbiJer = (turbiJernih[1] - TurbiValue) / (turbiJernih[1] - turbiJernih[0]);
  }
  else if (TurbiValue <= turbiJernih[0])
  {
    MUturbiJer = 1;
  }
  // Mencari FUngsi Keanggotaan Turbidity Keruh
  if (TurbiValue <= turbiKeruh[0])
  {
    MUturbiKer = 0;
  }
  else if (TurbiValue > turbiKeruh[0] && TurbiValue < turbiKeruh[1])
  {
    MUturbiKer = (TurbiValue - turbiKeruh[0]) / (turbiKeruh[1] - turbiKeruh[0]);
  }
  else if (TurbiValue >= turbiKeruh[1])
  {
    MUturbiKer = 1;
  }
  // Mencari Fungsi Keanggotaan pH Asam
  if (pHValue >= pHAsam[1])
  {
    MUpHAs = 0;
  }
  else if (pHValue > pHAsam[0] && pHValue < pHAsam[1])
  {
    MUpHAs = (pHAsam[1] - pHValue) / (pHAsam[1] - pHAsam[0]);
  }
  else if (pHValue <= pHAsam[0])
  {
    MUpHAs = 1;
  }
  // Mencari Fungsi Keanggotaan pH Netral
  if (pHValue <= pHNetral[0] || pHValue >= pHNetral[2])
  {
    MUpHNet = 0;
  }
  else if (pHValue > pHNetral[0] && pHValue < pHNetral[1])
  {
    MUpHNet = (pHValue - pHNetral[0]) / (pHNetral[1] - pHNetral[0]);
    // MUpHNet = 100;
  }
  else if (pHValue >= pHNetral[1] && pHValue < pHNetral[2])
  {
    MUpHNet = (pHNetral[2] - pHValue) / (pHNetral[2] - pHNetral[1]);
  }
  // // Mencari Fungsi Keanggotaan pH Basa
  if (pHValue <= pHBasa[0])
  {
    MUpHBas = 0;
  }
  else if (pHValue > pHBasa[0] && pHValue < pHBasa[1])
  {
    MUpHBas = (pHValue - pHBasa[0]) / (pHBasa[1] - pHBasa[0]);
  }
  else if (pHValue >= pHBasa[1])
  {
    MUpHBas = 1;
  }

  // Proses Inferensi Kendali Water Pump
  //[R1] Jika kekeruhan jernih & pH asam, maka waterpump hidup
  if (MUturbiJer < MUpHAs)
  {
    R1waterpump = MUturbiJer;
  }
  else if (MUpHAs < MUturbiJer)
  {
    R1waterpump = MUpHAs;
  }
  Z1waterpump = waterpumpHidup[0] + ((waterpumpHidup[1] - waterpumpHidup[0]) * R1waterpump); // Mencari Z1 dari Rule 1

  // [R2] Jika kekeruhan jernih & pH netral, maka waterpump mati
  if (MUturbiJer < MUpHNet)
  {
    R2waterpump = MUturbiJer;
  }
  else if (MUpHNet < MUturbiJer)
  {
    R2waterpump = MUpHNet;
  }
  Z2waterpump = waterpumpHidup[1] - ((waterpumpHidup[1] - waterpumpHidup[0]) * R2waterpump); // Mencari Z2 dari Rule 2

  // [R3] Jika kekeruhan jernih & pH basa, maka waterpump hidup
  if (MUturbiJer < MUpHBas)
  {
    R3waterpump = MUturbiJer;
  }
  else if (MUpHBas < MUturbiJer)
  {
    R3waterpump = MUpHBas;
  }
  Z3waterpump = waterpumpHidup[0] + ((waterpumpHidup[1] - waterpumpHidup[0]) * R3waterpump); // Mencari Z3 dari Rule 3

  // [R4] Jika kekeruhan keruh & pH asam, maka waterpump hidup
  if (MUturbiKer < MUpHAs)
  {
    R4waterpump = MUturbiKer;
  }
  else if (MUpHAs < MUturbiKer)
  {
    R4waterpump = MUpHAs;
  }
  Z4waterpump = waterpumpHidup[0] + ((waterpumpHidup[1] - waterpumpHidup[0]) * R4waterpump); // Mencari Z4 dari Rule 4

  // [R5] Jika kekeruhan keruh & pH netral, maka waterpump hidup
  if (MUturbiKer < MUpHNet)
  {
    R5waterpump = MUturbiKer;
  }
  else if (MUpHNet < MUturbiKer)
  {
    R5waterpump = MUpHNet;
  }
  Z5waterpump = waterpumpHidup[0] + ((waterpumpHidup[1] - waterpumpHidup[0]) * R5waterpump); // Mencari Z5 dari Rule 5

  // [R6] Jika kekeruhan keruh & pH basa, maka waterpump hidup
  if (MUturbiKer < MUpHBas)
  {
    R6waterpump = MUturbiKer;
  }
  else if (MUpHBas < MUturbiKer)
  {
    R6waterpump = MUpHBas;
  }
  Z6waterpump = waterpumpHidup[0] + ((waterpumpHidup[1] - waterpumpHidup[0]) * R6waterpump); // Mencari Z6 dari Rule 6

  // Defuzzyfikasi Kendali Water Pump
  OutputWaterpump = ((Z1waterpump * R1waterpump) + (Z2waterpump * R2waterpump) + (Z3waterpump * R3waterpump) + (Z4waterpump * R4waterpump) + (Z5waterpump * R5waterpump) + (Z6waterpump * R6waterpump)) / (R1waterpump + R2waterpump + R3waterpump + R4waterpump + R5waterpump + R6waterpump);

  return OutputWaterpump;
}
