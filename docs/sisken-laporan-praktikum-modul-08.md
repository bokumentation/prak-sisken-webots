Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<center> <h1>
LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 08: Sistem Kendali berbasis Fuzzy logic 1: Adaptive Speed Control
</h1> </center>

---

## 1. JUDUL PRAKTIKUM
Sistem Kendali berbasis Fuzzy logic 1 : Adaptive Speed Control

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah :
1. Mahasiswa dapat memahami fungsi dan cara kerja fuzzy logic untuk mengendalikan kecepatan motor DC pada robot line follower
2. Mahasiswa dapat membuat program berbasis timer untuk melakukan algoritma fuzzy logic pada berbagai kondisi kecepatan.

## 3. PERALATAN DAN BAHAN

Alat dan Bahan:
1. Robot Kit Line Follower
2. Baterai LiPo 2-Cell 1300 mAh
3. Kabel Mini-USB
4. Arduino Nano
5. Battery Checker
6. Battery Balancer

Perangkat Lunak:
1. Software IDE Arduino
2. Software Proteus (untuk simulasi)


## 4. TEORI DASAR

### 4.1. Sejarah Perkembangan Fuzzy logic
Fuzzy Set pertama kali diperkenalkan oleh Prof. Lotfi Zadeh, 1965 orang Iran yang menjadi guru besar di University of California at Berkeley dalam papernya yang monumental “Fuzzy Set”. Dalam paper tersebut dipaparkan ide dasar fuzzy set yang meliputi inclusion, union, intersection, complement, relation dan convexity.

Lotfi Zadeh mengatakan Integrasi Logika Fuzzy kedalam sistem informasi dan rekayasa proses adalah menghasilkan aplikasi seperti sistem kontrol, alat alat rumah tangga, dan sistem pengambil keputusan yang lebih fleksibel, mantap, dan canggih dibandingkan dengan sistem konvensional. Produk produk berikut telah menggunakan logika fuzzy dalam alat alat rumah tangga seperti mesin cuci, video dan kamera refleksi lensa tunggal, pendingin ruangan, oven microwave, dan banyak sistem diagnosa mandiri. Pelopor aplikasi fuzzy set dalam bidang kontrol, adalah Prof. Ebrahim Mamdani dkk dari Queen Mary College London. (masih dalam skala lab). Penerapan secara nyata di industri banyak dipelopori oleh para ahli dari Jepang misalnya Prof. Sugeno dkk dari Tokyo Institute of Technology.

### 4.2. Tahap Pemodelan pada Fuzzy logic untuk Robot Line Follower

Proses fuzzy yang dilakukan pada sistem kendali robot line follower umumnya sama dengan proses fuzzy pada sistem kendali lainnya yaitu meliputi fuzzifikasi, evaluasi rule dan defuzzifikasi. Dengan menggunakan algoritma ini robot diharapkan dapat bergerak mengikuti jalur dengan sesuai dan cepat.

Dari gambar tersebut dapat dipahami bahwa di dalam fuzzy logic terdapat tiga hal terpenting yang harus diperhatikan yaitu:
1. Fuzzifikasi adalah proses untuk mengubah variabel non fuzzy (variabel numerik) menjadi variabel fuzzy (variabel linguistik).
2. Inferencing (Ruled Based) , pada umumnya aturan-aturan fuzzy dinyatakan dalam bentuk “IF……THEN” yang merupakan inti dari relasi fuzzy.
3. Defuzzifikasi adalah proses pengubahan data-data fuzzy tersebut menjadi data-data numerik yang dapat dikirimkan ke peralatan pengendalian yang dalam hal ini adalah mengubah kecepatan dari motor DC.

## 5. KEGIATAN PRAKTIKUM

a. Kalibrasi kedua motor DC pada robot line follower dengan cara membuat program untuk menjalankan kedua motor tersebut dengan nilai PWM yang sama. Apabila terdapat perbedaan kecepatan di antara kedua motor tersebut, lakukan proses tuning dengan cara menambah atau mengurangi nilai PWM dari motor tersebut.

b. Modifikasi program pada praktikum sebelumnya dengan variable dan fungsi berikut ini :
- Tambahkan variabel integer dengan nama 'kecepatanNow'

- Di dalam void setup tambahkan :
    - Baca nilai eeprom yang terdapat di EEPROM dengan alamat 0 hingga 7 (dengan ketentuan alamat 0 adalah untuk data nilai tengah kalibrasi sensor 1, alamat 1 untuk nilai tengah kalibrasi sensor 2, dan seterusnya hingga sensor 8) dengan perintah EEPROM.read(0) hingga EEPROM.read(7), kemudian simpan didalam variabel peka[0] hingga peka[7] dengan mengkalikan dengan angka 4 (contoh : peka[0]=EEPROM.read[0]*4).
    - Baca nilai eeprom yang terdapat di EEPROM dengan alamat 8 (ketentuan alamat 8 adalah data nilai Kp), kemudian simpan didalam variabel Kp.
    - Baca nilai eeprom yang terdapat di EEPROM dengan alamat 9 (ketentuan alamat 9 adalah data nilai Kd), kemudian simpan didalam variabel Kp.
    - Tampilkan data peka[0] hingga peka[7], Kp dan Kd yang berasal dari data EEPROM ke Serial Monitor.

- Di dalam void loop ditambahkan conditional sebagai berikut :
    - Jika tombol kiri belakang ditekan dan setting bernilai 0 maka 'state' akan bernilai 1.
    - Jika tombol kiri depan ditekan dan setting bernilai 0 maka 'state' akan bernilai 2 dan 'setting' akan bernilai 1.
    - Jika tombol kiri belakang ditekan dan setting bernilai 1 maka 'state' akan bernilai 3.
    - Jika tombol kiri depan ditekan dan setting bernilai 1 maka 'state' akan bernilai 4.
    - Jika tombol kanan belakang ditekan dan setting bernilai 1 maka 'state' akan bernilai 5.
    - Jika tombol kanan depan ditekan dan setting bernilai 1 maka 'state' akan bernilai 6.
    - Jika tombol kanan dan kiri depan ditekan secara bersamaan dan setting bernilai 1 maka 'state' akan bernilai 7.
    - Jika tombol kanan dan kiri belakang ditekan secara bersamaan dan setting bernilai 1 maka 'state' akan bernilai 8.
    - Jika 'state' bernilai 1 maka proses auto calibration berlangsung.
    - Jika 'state' bernilai 2 maka nilai terakhir variabel 'peka[0]' hingga 'peka[7]' disimpan ke dalam EEPROM dengan alamat 0 hingga 7 dan menjalankan perintah proses line follower dengan sistem kendali PID dengan nilai peka[0] hingga peka[7], Kp dan Kd yang diperoleh dari data EEPROM dan atau berasal dari hasil autocallibration.
    - Perintah EEPROM.write(alamat,value) dengan value yang dibagi dengan 4. Jelaskan mengapa value harus dikali dan dibagi 4! Contoh : EEPROM.write(0, peka[0]/4)
    - Jika 'state' bernilai 3 maka LED kiri akan menyala dan setelah 500 milidetik LED kiri akan mati, mengurangi nilai Kp(contoh : Kp=Kp-1;), menyimpan nilai Kp kedalam EEPROM pada alamat 8.
    - Jika 'state' bernilai 4 maka LED kiri akan menyala dan setelah 500 milidetik LED kiri akan mati, menambah nilai Kp(contoh : Kp=Kp+1;), menyimpan nilai Kp kedalam EEPROM pada alamat 8.
    - Jika 'state' bernilai 5 maka LED kanan akan menyala dan setelah 500 milidetik LED kiri akan mati, mengurangi nilai Kd(contoh : Kd=Kd-1;), menyimpan nilai Kd kedalam EEPROM pada alamat 9
    - Jika 'state' bernilai 6 maka LED kanan akan menyala dan setelah 500 milidetik LED kiri akan mati, menambah nilai Kd(contoh : Kd=Kd+1;), menyimpan nilai Kd kedalam EEPROM pada alamat 10


c. Gunakan fungsi millis() untuk menghitung jumlah milidetik semenjak program berjalan. Output dari fungsi ini memiliki tipe data Unsigned Long. Buatlah sub program dengan menggunakan fungsi millis() yang akan melakukan fungsi timer 2 detik (nilai timer dapat disesuaikan dengan kebutuhan dan kondisi di lapangan) secara berulang-ulang dan memeriksa nilai error saat ini. Apabila ketika nilai timer bernilai 2 detik, maka lakukan proses adaptive speed control sebagai berikut:
- Cek nilai error, apabila nilai error saat ini = 0 maka tingkatkan nilai kecepatanSetPoint bertambah 20 (sesuaikan penambahan setpoint dengan kondisi) dan disimpan dalam variable kecepatanNow. e.- bila nilai error selain 0, maka nilai setpoint kembali ke 150.
- Simpan nilai kecepatanMotorKanan = kecepatanNow dikurang moveControl
- Simpan nilai kecepatanMotorKiri = kecepatanNow ditambah moveControl
- Kecepatan Motor Kiri dengan nilai analog sebesar kecepatanMotorKiri 
- Kecepatan Motor Kanan dengan nilai analog sebesar kecepatanMotorKanan
- Proses ini dilakukan secara berulang (counter) selama program berjalan.
- Gunakan nilai Kp dan Kd yang paling optimal pada praktikum sebelumnya kemudian amati perubahan yang terlihat setelah ditambahkan algoritma fuzzy logic!
- Sesuaikan nilai waktu pada timer/counter pada program kemudian ujicoba pada lintasan dan amati hasil perubahan nilai tersebut pada robot line follower! Apa yang menjadi kesimpulan dari hasil percobaan ini?

### 5.1. KODE PROGRAM

```cpp
#include <EEPROM.h>

const int S[8] = {A4, A5, A6, A7, A0, A1, A2, A3};
int threshold[8];

const int ML_MAJU   = 5;
const int ML_MUNDUR = 6;
const int MR_MAJU   = 10;
const int MR_MUNDUR = 9;

const int BT_START = 3;
const int BT_CAL   = 2;
const int BT_KD_P  = 11;
const int BT_KD_M  = 12;
const int LED_L    = 7;
const int LED_R    = 8;

float Kp, Ki, Kd;
float P_val, I_val, D_val;
float integralError = 0;
int error = 0, lastError = 0;

int defaultBaseSpeed = 120;
int currentBaseSpeed = 120;

bool robotJalan = false;
int  state      = 0;

unsigned long lastSerialPrint      = 0;
const unsigned long serialInterval       = 150; // ms
unsigned long lastSpeedCheck       = 0;
const unsigned long speedCheckInterval   = 30;  // ms
unsigned long lastPIDTime          = 0;
unsigned long lastButtonPress      = 0;
const unsigned long debounceDelay        = 300;

int    rawSensor[8];
String sensorBits = "";

int greenLow[8];
int greenHigh[8];
int greenCount = 0;
const int greenDebounce = 3;

void setup() {
  Serial.begin(9600);

  pinMode(ML_MAJU,   OUTPUT); pinMode(ML_MUNDUR, OUTPUT);
  pinMode(MR_MAJU,   OUTPUT); pinMode(MR_MUNDUR, OUTPUT);
  pinMode(BT_START,  INPUT_PULLUP);
  pinMode(BT_CAL,    INPUT_PULLUP);
  pinMode(BT_KD_P,   INPUT_PULLUP);
  pinMode(BT_KD_M,   INPUT_PULLUP);
  pinMode(LED_L,     OUTPUT);
  pinMode(LED_R,     OUTPUT);

  loadFromEEPROM();
  lastPIDTime = millis();

  Serial.println(F("======================================"));
  Serial.println(F("   MODUL 8 - LINE FOLLOWER READY"));
  Serial.println(F("======================================"));
  printKPID();
  printThreshold();
}

void loop() {
  if (!robotJalan) {
    state = 0;
    gerak(0, 0);
    handleButtonStandby();
  } else {
    state = 2;

    if (digitalRead(BT_START) == LOW && digitalRead(BT_CAL) == LOW) {
      robotJalan = false; state = 0;
      integralError = 0;
      gerak(0, 0);
      Serial.println(F("[STOP] Robot berhenti."));
      delay(500);
      return;
    }

    bacaSensor();

    bool semuaHijau = true;
    for (int i = 0; i < 8; i++) {
      if (rawSensor[i] < greenLow[i] || rawSensor[i] > greenHigh[i]) {
        semuaHijau = false;
        break;
      }
    }

    if (semuaHijau) {
      greenCount++;
      if (greenCount >= greenDebounce) {
        robotJalan = false; state = 0;
        integralError = 0; greenCount = 0;
        gerak(0, 0);
        Serial.println(F("[FINISH] Deteksi hijau, robot berhenti."));
        delay(500);
        return;
      }
    } else {
      greenCount = 0;
    }

    petakanError();
    adaptiveSpeed();
    hitungPID();
    cetakSerial();
  }
}

void bacaSensor() {
  sensorBits = "";
  for (int i = 0; i < 8; i++) {
    rawSensor[i]  = analogRead(S[i]);
    sensorBits   += (rawSensor[i] > threshold[i]) ? "1" : "0";
  }
}

void petakanError() {
  if (sensorBits == "00000000") return; // garis hilang, pertahankan error lama

  int prevError = error;

  // Pola 2 sensor (normal)
  if      (sensorBits == "11000000") error = -6;
  else if (sensorBits == "01100000") error = -4;
  else if (sensorBits == "00110000") error = -2;
  else if (sensorBits == "00011000") error =  0;
  else if (sensorBits == "00001100") error =  2;
  else if (sensorBits == "00000110") error =  4;
  else if (sensorBits == "00000011") error =  6;
  // Pola 1 sensor (tikungan tajam)
  else if (sensorBits == "10000000") error = -7;
  else if (sensorBits == "01000000") error = -5;
  else if (sensorBits == "00100000") error = -3;
  else if (sensorBits == "00010000") error = -1;
  else if (sensorBits == "00001000") error =  1;
  else if (sensorBits == "00000100") error =  3;
  else if (sensorBits == "00000010") error =  5;
  else if (sensorBits == "00000001") error =  7;
  // Pola 3 sensor
  else if (sensorBits == "11100000") error = -5;
  else if (sensorBits == "01110000") error = -3;
  else if (sensorBits == "00111000") error = -1;
  else if (sensorBits == "00011100") error =  1;
  else if (sensorBits == "00001110") error =  3;
  else if (sensorBits == "00000111") error =  5;
  // Semua aktif = perpotongan
  else if (sensorBits == "11111111") error =  0;

  lastError = prevError;
}

void adaptiveSpeed() {
  if (millis() - lastSpeedCheck >= speedCheckInterval) {
    lastSpeedCheck = millis();

    // Toleransi deviasi (error antara -1 hingga 1 dianggap lurus)
    if (error >= -2 && error <= 2) {
      // Garis lurus -> setpoint naik otomatis
      currentBaseSpeed += 55;
      if (currentBaseSpeed > 255) currentBaseSpeed = 255;
    } else {
      // Belok atau menyimpang jauh -> kembali ke default
      currentBaseSpeed = defaultBaseSpeed;
    }
  }
}

void hitungPID() {
  unsigned long now = millis();
  float dt = (float)(now - lastPIDTime) / 1000.0;
  if (dt < 0.001) dt = 0.001;

  P_val = Kp * error;

  integralError += (error * dt);
  integralError  = constrain(integralError, -500.0, 500.0); // anti-windup
  I_val = Ki * integralError;

  D_val = Kd * ((float)(error - lastError)) / dt;

  float PID_total = P_val + I_val + D_val;

  gerak(currentBaseSpeed + (int)PID_total,
        currentBaseSpeed - (int)PID_total);

  // LED indikator arah belok
  digitalWrite(LED_L, (error < 0) ? HIGH : LOW);
  digitalWrite(LED_R, (error > 0) ? HIGH : LOW);

  lastPIDTime = now;
}

void cetakSerial() {
  if (millis() - lastSerialPrint < serialInterval) return;
  lastSerialPrint = millis();

  Serial.print(F("T:"));    Serial.print(millis());
  Serial.print(F(" St:"));  Serial.print(state);
  Serial.print(F(" E:"));   Serial.print(error);
  Serial.print(F(" Spd:")); Serial.print(currentBaseSpeed);
  Serial.print(F(" P:"));   Serial.print(P_val, 2);
  Serial.print(F(" I:"));   Serial.print(I_val, 4);
  Serial.print(F(" D:"));   Serial.print(D_val, 2);
  Serial.print(F(" Kp:"));  Serial.print(Kp, 1);
  Serial.print(F(" Ki:"));  Serial.print(Ki, 4);
  Serial.print(F(" Kd:"));  Serial.print(Kd, 1);
  Serial.print(F(" S:[")); Serial.print(sensorBits);
  Serial.print(F("] Thr:["));
  for (int i = 0; i < 8; i++) {
    Serial.print(threshold[i]);
    if (i < 7) Serial.print(F(","));
  }
  Serial.println(F("]"));
}

void handleButtonStandby() {
  bool bS = (digitalRead(BT_START) == LOW);
  bool bC = (digitalRead(BT_CAL)   == LOW);
  bool bP = (digitalRead(BT_KD_P)  == LOW);
  bool bM = (digitalRead(BT_KD_M)  == LOW);

  if (!bS && !bC && !bP && !bM) return;
  if (millis() - lastButtonPress < debounceDelay) return;
  lastButtonPress = millis();

  if (bS && !bC && !bP && !bM) {
    // START
    robotJalan = true; state = 2;
    integralError = 0; currentBaseSpeed = defaultBaseSpeed;
    lastPIDTime = millis();
    Serial.println(F("[START] Robot mulai jalan."));
  }
  else if (bC && !bS && !bP && !bM) { autoCalibration(); }
  else if (bS && bP && !bC && !bM)  { Kp += 0.5; saveKPID(); Serial.print(F("[Kp+] ")); printKPID(); }
  else if (bS && bM && !bC && !bP)  { if (Kp >= 0.5) Kp -= 0.5; saveKPID(); Serial.print(F("[Kp-] ")); printKPID(); }
  else if (bC && bP && !bS && !bM)  { Ki += 0.005; saveKPID(); Serial.print(F("[Ki+] ")); printKPID(); }
  else if (bC && bM && !bS && !bP)  { if (Ki >= 0.005) Ki -= 0.005; saveKPID(); Serial.print(F("[Ki-] ")); printKPID(); }
  else if (bP && !bS && !bC && !bM) { Kd += 0.5; saveKPID(); Serial.print(F("[Kd+] ")); printKPID(); }
  else if (bM && !bS && !bC && !bP) { if (Kd >= 0.5) Kd -= 0.5; saveKPID(); Serial.print(F("[Kd-] ")); printKPID(); }
}

void autoCalibration() {
  state = 1;
  int sMin[8], sMax[8];
  for (int i = 0; i < 8; i++) { sMin[i] = 1023; sMax[i] = 0; }

  Serial.println(F("--- MULAI KALIBRASI (geser robot di atas garis 5 detik) ---"));
  unsigned long startCal = millis();

  while (millis() - startCal < 5000) {
    bool nyala = ((millis() / 100) % 2 == 0);
    digitalWrite(LED_L, nyala);
    digitalWrite(LED_R, nyala);
    for (int i = 0; i < 8; i++) {
      int v = analogRead(S[i]);
      if (v < sMin[i]) sMin[i] = v;
      if (v > sMax[i]) sMax[i] = v;
    }
  }

  for (int i = 0; i < 8; i++) {
    threshold[i] = (sMax[i] + sMin[i]) / 2;
    EEPROM.put(i * 2, threshold[i]);

    int range = sMax[i] - threshold[i];
    greenLow[i]  = threshold[i] + range / 4;
    greenHigh[i] = sMax[i] - range / 4;
    if (greenLow[i] >= greenHigh[i]) {
      greenLow[i]  = threshold[i] + 10;
      greenHigh[i] = sMax[i] - 10;
    }
  }
  digitalWrite(LED_L, LOW); digitalWrite(LED_R, LOW);
  Serial.println(F("--- KALIBRASI SELESAI & TERSIMPAN ---"));
  printThreshold();
  printGreenRange();
}

void loadFromEEPROM() {
  for (int i = 0; i < 8; i++) {
    EEPROM.get(i * 2, threshold[i]);
    if (threshold[i] < 0 || threshold[i] > 1023) threshold[i] = 512;
  }
  EEPROM.get(20, Kp); EEPROM.get(25, Ki); EEPROM.get(30, Kd);
  if (isnan(Kp) || Kp <= 0) Kp = 18.0;
  if (isnan(Ki) || Ki <  0) Ki =  0.01;
  if (isnan(Kd) || Kd <= 0) Kd = 25.0;
}

void saveKPID() {
  EEPROM.put(20, Kp);
  EEPROM.put(25, Ki);
  EEPROM.put(30, Kd);
}

void printKPID() {
  Serial.print(F("Kp=")); Serial.print(Kp, 1);
  Serial.print(F(" Ki=")); Serial.print(Ki, 4);
  Serial.print(F(" Kd=")); Serial.println(Kd, 1);
}

void printThreshold() {
  Serial.print(F("Threshold: ["));
  for (int i = 0; i < 8; i++) {
    Serial.print(threshold[i]);
    if (i < 7) Serial.print(F(","));
  }
  Serial.println(F("]"));
}

void printGreenRange() {
  Serial.print(F("GreenRange: ["));
  for (int i = 0; i < 8; i++) {
    Serial.print(greenLow[i]);
    Serial.print(F("-"));
    Serial.print(greenHigh[i]);
    if (i < 7) Serial.print(F(","));
  }
  Serial.println(F("]"));
}

void gerak(int ki, int ka) {
  ki = constrain(ki, 0, 255);
  ka = constrain(ka, 0, 255);
  analogWrite(ML_MAJU,    255 - ki);
  digitalWrite(ML_MUNDUR, HIGH);
  analogWrite(MR_MAJU,    255 - ka);
  digitalWrite(MR_MUNDUR, HIGH);
}
```

## 6. KESIMPULAN

Berdasarkan hasil praktikum yang telah dilakukan, dapat diambil beberapa kesimpulan sebagai berikut.

1. Logika *fuzzy* berhasil diimplementasikan pada sistem kendali robot *line follower* untuk melakukan *adaptive speed control*. Algoritma ini memungkinkan robot untuk menyesuaikan kecepatan secara otomatis berdasarkan kondisi lintasan yang terdeteksi oleh sensor.

2. Mekanisme *adaptive speed control* bekerja dengan cara memeriksa nilai *error* secara periodik menggunakan fungsi `millis()`. Apabila nilai *error* berada dalam rentang -2 hingga 2 (menandakan robot berada di garis lurus), maka kecepatan dasar (*base speed*) akan ditingkatkan secara bertahap hingga mencapai batas maksimal 255. Sebaliknya, apabila *error* bernilai besar (menandakan robot berada di tikungan), maka kecepatan akan dikembalikan ke nilai *default* 120.

3. Proses fuzzifikasi pada sistem ini dilakukan dengan memetakan nilai *error* hasil pembacaan sensor menjadi variabel linguistik yang menentukan kondisi lintasan (lurus atau belok). Evaluasi *rule* dilakukan dengan logika *IF-THEN* untuk menentukan aksi yang sesuai, dan defuzzifikasi menghasilkan nilai PWM yang dikirimkan ke motor DC.

4. Kalibrasi motor DC menjadi langkah awal yang penting untuk memastikan kedua motor memiliki kecepatan yang sama pada nilai PWM yang diberikan. Ketidakseimbangan kecepatan antara motor kiri dan kanan dapat menyebabkan robot tidak dapat mengikuti garis dengan lurus meskipun algoritma kendali telah bekerja dengan baik.

5. Penggunaan *timer* dengan interval 30 milidetik untuk pengecekan kecepatan memberikan respons yang cukup cepat terhadap perubahan kondisi lintasan. Penambahan kecepatan secara bertahap saat garis lurus dan pengembalian cepat ke kecepatan *default* saat tikungan menghasilkan pergerakan robot yang lebih efisien dan stabil.

## 7. LAMPIRAN

Link dokumentasi video:
https://drive.google.com/drive/folders/1Lgzs5CNa1o80Mx8uDIgMI3fhwKW2n_Xc?usp=sharing

Dokumentasi:

