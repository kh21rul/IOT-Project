// deklarasi Keanggotaan himpunan oksigen
float oksigenRendah[] = {4, 8}; // <4, 4, 8
float oksigenTinggi[] = {4, 8}; // 4, 8, 8>
// deklarasi Keanggotaan himpunan suhu
float suhuDingin[] = {25, 30}; // <25, 25, 30
float suhuNetral[] = {25, 30, 35}; // 25, 30, 35
float suhuPanas[] = {30, 35}; // 30, 35, 35>
// deklarasi Keanggotaan himpunan turbidity
float turbiJernih[] = {25, 400}; // <25, 25, 400
float turbiKeruh[] = {25, 400}; // 25, 400, 400>
// deklarasi Keanggotaan himpunan pH
float pHAsam[] = {6, 7}; // <6, 6, 7
float pHNetral[] = {6, 7, 8}; // 6, 7, 8
float pHBasa[] = {7, 8};      // 7, 8, 8>

float MUoksigenRen, MUoksigenTin; // deklarasi variabel derajat keanggotaan oksigen
float MUsuhuDin, MUsuhuNet, MUsuhuPan; // deklarasi variabel derajat keanggotaan suhu
float MUturbiJer, MUturbiKer; // deklarasi variabel derajat keanggotaan turbidity
float MUpHAs, MUpHNet, MUpHBas; // deklarasi variabel derajat keanggotaan pH

float R1aerator, R2aerator, R3aerator, R4aerator, R5aerator, R6aerator; // deklarasi variabel Rules untuk pengendalian Aerator
float R1waterpump, R2waterpump, R3waterpump, R4waterpump, R5waterpump, R6waterpump; // deklarasi variabel Rules untuk pengendalian Water Pump
float aeratorMati, aeratorHidup; // menampung perbandingan terbesar dari rule aerator mati & hidup
float waterpumpMati, waterpumpHidup; // menampung perbandingan terbesar dari rule water pump mati & hidup
float t1aerator, t2aerator; // deklarasi variabel titik potong kurva aerator
float t1waterpump, t2waterpump; // deklarasi variabel titik potong kurva water pump

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // Input nilai Variabel
  float DOValue = 7;
  float TempValue = 31;
  float TurbiValue = 100;
  float pHValue = 7;
 
  // Proses Fuzzyfikasi Kendali Aerator
  // Mencari Fungsi Keanggotaan Oksigen Rendah
  if (DOValue >= oksigenRendah[1]) {
    MUoksigenRen = 0;
  } else if (DOValue > oksigenRendah[0] && DOValue < oksigenRendah[1]) {
    MUoksigenRen = (oksigenRendah[1] - DOValue) / (oksigenRendah[1] - oksigenRendah[0]);
  } else if (DOValue <= oksigenRendah[0]) {
    MUoksigenRen = 1;
  }
  // Mencari Fungsi Keanggotaan Oksigen Tinggi
  if (DOValue <= oksigenTinggi[0]) {
    MUoksigenTin = 0;
  } else if (DOValue > oksigenTinggi[0] && DOValue < oksigenTinggi[1]) {
    MUoksigenTin = (DOValue - oksigenTinggi[0]) / (oksigenTinggi[1] - oksigenTinggi[0]);
  } else if (DOValue >= oksigenTinggi[1]) {
    MUoksigenTin = 1;
  }
  // Mencari Fungsi Keanggotaan Suhu Dingin
  if (TempValue >= suhuDingin[1]) {
    MUsuhuDin = 0;
  } else if (TempValue > suhuDingin[0] && TempValue < suhuDingin[1]) {
    MUsuhuDin = (suhuDingin[1] - TempValue) / (suhuDingin[1] - suhuDingin[0]);
  } else if(TempValue <= suhuDingin[0]) {
    MUsuhuDin = 1;
  }
  // Mencari Fungsi Keanggotaan Suhu Netral
  if (TempValue <= suhuNetral[0] || TempValue >= suhuNetral[2]) {
    MUsuhuNet = 0;
  } else if (TempValue > suhuNetral[0] && TempValue < suhuNetral[1]) {
    MUsuhuNet = (TempValue - suhuNetral[0]) / (suhuNetral[1] - suhuNetral[0]);
  } else if (TempValue >= suhuNetral[1] && TempValue < suhuNetral[2]) {
    MUsuhuNet = (suhuNetral[2] - TempValue) / (suhuNetral[2] - suhuNetral[1]);
  }
  // Mencari Fungsi Keanggotaan Suhu Panas
  if (TempValue <= suhuPanas[0]) {
    MUsuhuPan = 0;
  } else if (TempValue > suhuPanas[0] && TempValue < suhuPanas[1]) {
    MUsuhuPan = (TempValue - suhuPanas[0]) / (suhuPanas[1] - suhuPanas[0]);
  } else if (TempValue >= suhuPanas[1]) {
    MUsuhuPan = 1;
  }

  // Proses Fuzzyfikasi Kendali Waterpump
  // Mencari Fungsi Keanggotaan Turbidity jernih
  if (TurbiValue >= turbiJernih[1]) {
    MUturbiJer = 0;
  } else if (TurbiValue > turbiJernih[0] && TurbiValue < turbiJernih[1]) {
    MUturbiJer = (turbiJernih[1] - TurbiValue) / (turbiJernih[1] - turbiJernih[0]);
  } else if (TurbiValue <= turbiJernih[0]) {
    MUturbiJer = 1;
  }
  // Mencari FUngsi Keanggotaan Turbidity Keruh
  if (TurbiValue <= turbiKeruh[0]) {
    MUturbiKer = 0;
  } else if (TurbiValue > turbiKeruh[0] && TurbiValue < turbiKeruh[1]) {
    MUturbiKer = (TurbiValue - turbiKeruh[0]) / (turbiKeruh[1] - turbiKeruh[0]);
  } else if (TurbiValue >= turbiKeruh[1]) {
    MUturbiKer = 1;
  }
  // Mencari Fungsi Keanggotaan pH Asam
  if (pHValue >= pHAsam[1]) {
    MUpHAs = 0;
  } else if (pHValue > pHAsam[0] && pHValue < pHAsam[1]) {
    MUpHAs = (pHAsam[1] - pHValue) / (pHAsam[1] - pHAsam[0]);
  } else if (pHValue <= pHAsam[0]) {
    MUpHAs = 1;
  }
  // // Mencari Fungsi Keanggotaan pH Netral
  if (pHValue <= pHNetral[0] || pHValue >= pHNetral[2]) {
    MUpHNet = 0;
  } else if (pHValue > pHNetral[0] && pHValue < pHNetral[1]) {
    MUpHNet = (pHValue - pHNetral[0]) / (pHNetral[1] - pHNetral[0]);
    // MUpHNet = 100;
  } else if (pHValue >= pHNetral[1] && pHValue < pHNetral[2]) {
    MUpHNet = (pHNetral[2] - pHValue) / (pHNetral[2] - pHNetral[1]);
  }
  // // Mencari Fungsi Keanggotaan pH Basa
  if (pHValue <= pHBasa[0]) {
    MUpHBas = 0;
  } else if (pHValue > pHBasa[0] && pHValue < pHBasa[1]) {
    MUpHBas = (pHValue - pHBasa[0]) / (pHBasa[1] - pHBasa[0]);
  } else if (pHValue >= pHBasa[1]) {
    MUpHBas = 1;
  }

  // Proses Inferensi Kendali aerator
  // [R1] Jika oksigen rendah & suhu dingin, maka aerator hidup
  if (MUoksigenRen < MUsuhuDin) {
    R1aerator = MUoksigenRen;
  } else if (MUsuhuDin < MUoksigenRen) {
    R1aerator = MUsuhuDin;
  }
  // [R2] Jika oksigen rendah & suhu netral, maka aerator hidup
  if (MUoksigenRen < MUsuhuNet) {
    R2aerator = MUoksigenRen;
  } else if (MUsuhuNet < MUoksigenRen) {
    R2aerator = MUsuhuNet;
  }
  // [R3] Jika oksigen rendah & suhu panas, maka aerator hidup
  if (MUoksigenRen < MUsuhuPan) {
    R3aerator = MUoksigenRen;
  } else if (MUsuhuPan < MUoksigenRen) {
    R3aerator = MUsuhuPan;
  }
  // [R4] Jika oksigen tinggi & suhu dingin, maka aerator hidup
  if (MUoksigenTin < MUsuhuDin) {
    R4aerator = MUoksigenTin;
  } else if (MUsuhuDin < MUoksigenTin) {
    R4aerator = MUsuhuDin;
  }
  // [R5] Jika oksigen tinggi & suhu netral, maka aerator mati
  if (MUoksigenTin < MUsuhuNet) {
    R5aerator = MUoksigenTin;
  } else if (MUsuhuNet < MUoksigenTin) {
    R5aerator = MUsuhuNet;
  }
  // [R6] Jika oksigen tinggi & suhu panas, maka aerator hidup
  if (MUoksigenTin < MUsuhuPan) {
    R6aerator = MUoksigenTin;
  } else if (MUsuhuPan < MUoksigenTin) {
    R6aerator = MUsuhuPan;
  }

  // Proses Inferensy Kendali Water Pump
  //[R1] Jika kekeruhan jernih & pH asam, maka waterpump hidup
  if (MUturbiJer < MUpHAs) {
    R1waterpump = MUturbiJer;
  } else if (MUpHAs < MUturbiJer) {
    R1waterpump = MUpHAs;
  }
  // [R2] Jika kekeruhan jernih & pH netral, maka waterpump mati
  if (MUturbiJer < MUpHNet) {
    R2waterpump = MUturbiJer;
  } else if (MUpHNet < MUturbiJer) {
    R2waterpump = MUpHNet;
  }
  // [R3] Jika kekeruhan jernih & pH basa, maka waterpump hidup
  if (MUturbiJer < MUpHBas) {
    R3waterpump = MUturbiJer;
  } else if (MUpHBas < MUturbiJer) {
    R3waterpump = MUpHBas;
  }
  // [R4] Jika kekeruhan keruh & pH asam, maka waterpump hidup
  if (MUturbiKer < MUpHAs) {
    R4waterpump = MUturbiKer;
  } else if (MUpHAs < MUturbiKer) {
    R4waterpump = MUpHAs;
  }
  // [R5] Jika kekeruhan keruh & pH netral, maka waterpump hidup
  if (MUturbiKer < MUpHNet) {
    R5waterpump = MUturbiKer;
  } else if (MUpHNet < MUturbiKer) {
    R5waterpump = MUpHNet;
  }
  // [R6] Jika kekeruhan keruh & pH basa, maka waterpump hidup
  if (MUturbiKer < MUpHBas) {
    R6waterpump = MUturbiKer;
  } else if (MUpHBas < MUturbiKer) {
    R6waterpump = MUpHBas;
  }

  // Mencari Komposisi dengan aturan Max dari Rule hidup dan mati aerator:
  aeratorMati = R5aerator; // hasil inferensi terbesar dari rule kendali Mati

  aeratorHidup = R1aerator;
  if (R2aerator > aeratorHidup) {
    aeratorHidup = R2aerator;
  } else if (R3aerator > aeratorHidup) {
    aeratorHidup = R3aerator;
  } else if (R4aerator > aeratorHidup) {
    aeratorHidup = R4aerator;
  } else if (R6aerator > aeratorHidup) {
    aeratorHidup = R6aerator;
  } // hasil inferensi terbesar dari rule kendali Hidup

  // Mencari Komposisi dengan aturan Max dari Rule hidup dan mati waterpump:
  waterpumpMati = R2waterpump; // hasil inferensi terbesar dari rule kendali Mati

  waterpumpHidup = R1waterpump;
  if (R3waterpump > waterpumpHidup) {
    waterpumpHidup = R3waterpump;
  } else if (R4waterpump > waterpumpHidup) {
    waterpumpHidup = R4waterpump;
  } else if (R5waterpump > waterpumpHidup) {
    waterpumpHidup = R5waterpump;
  } else if (R6waterpump > waterpumpHidup) {
    waterpumpHidup = R6waterpump;
  } // hasil inferensi terbesar dari rule kendali Hidup

  // Mencari nilai t1 & t2 aerator
  t1aerator = (aeratorMati * (70 - 30)) + 30;
  t2aerator = (aeratorHidup * (70 - 30)) + 30;

  // Mencari nilai t1 & t2 waterpump
  t1waterpump = (waterpumpMati * (70 - 30)) + 30;
  t2waterpump = (waterpumpHidup * (70 - 30)) + 30;

  Serial.print("Komposisi Max Aerator Mati: ");
  Serial.println(aeratorMati);
  Serial.print("Komposisi Max Aerator Hidup: ");
  Serial.println(aeratorHidup);
  Serial.println();
  Serial.print("Nilai t1: ");
  Serial.println(t1aerator);
  Serial.print("nilai t2: ");
  Serial.println(t2aerator);
  Serial.println();
  Serial.print("Komposisi Max Waterpump Mati: ");
  Serial.println(waterpumpMati);
  Serial.print("Komposisi Max Waterpump Hidup: ");
  Serial.println(waterpumpHidup);
  Serial.println();
  Serial.print("Nilai t1: ");
  Serial.println(t1waterpump);
  Serial.print("nilai t2: ");
  Serial.println(t2waterpump);
  Serial.println();

  delay(10000);
}
