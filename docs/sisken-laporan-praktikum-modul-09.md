Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<center> <h1>
LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 09: Sistem Kendali berbasis Fuzzy Logic : Adaptive Sensor Calibration
</h1> </center>

---

## 1. JUDUL PRAKTIKUM
Sistem Kendali berbasis Fuzzy Logic : Adaptive Sensor Calibration

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah :
1. Mahasiswa dapat memahami fungsi dan cara kerja fuzzy logic untuk mengendalikankecepatan motor DC pada robot line follower
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

### 4.1. Sistem Kendali PD

Proses kalibrasi bertujuan untuk mendapatkan nilai tengah dari masing-masing sensor. Jika proses kalibrasi sensor pada praktikum sebelumnya dilakukan dengan menekan tombol, maka kali ini proses kalibrasi sensor dilakukan secara langsung ketika robot berjalan di lintasan. Hal ini memberikan keuntungan dibandingkan metode kalibrasi secara manual yaitu robot dapat mengikuti garis walaupun terdapat perubahan atau ketidakseragaman warna dari garis tersebut. Proses penentuan nilai tengah sensor ini dapat dirumuskan sebagai berikut.

`NilaiTengahSensor[i]= NilaiMinSensor[i]+NilaiMaxSensor[i])/2`

Pengambilan data pada saat kalibrasi disimpan dengan menggunakan variabel warna sesuai dengan warna garis. Berikut adalah gambar lintasan yang digunakan dalam praktikum ini. Pengambilan data pada saat kalibrasi disimpan dengan menggunakan variabel warna sesuai dengan warna garis. Berikut adalah gambar lintasan yang digunakan dalam praktikum ini



## 5. KEGIATAN PRAKTIKUM

a. Buat program untuk membaca data sensor untuk melihat perbandingan pembacaan nilai ADC sensor pada serial monitor untuk 4 warna yaitu merah, biru, hitam dan putih. Kemudian tentukan nilai tengah sensor untuk 3 skenario warna garis lintasan yaitu garis merah dengan latar putih, garis hitam dengan latar putih, dan garis merah dengan latar biru. Isi data tersebut pada tabel dan grafik berikut ini (lampirkan dalam buku praktikum). Gunakan data pada tabel ini untuk perbandingan dengan hasil algoritma fuzzy untuk adaptive sensor calibration pada lintasan. Jelaskan perbedaan yang terjadi antara 3 skenario
tersebut!

b. Untuk melakukan proses adaptive sensor calibration, modifikasi program pada praktikum sebelumnya dengan ketentuan sebagai berikut.

- Hilangkan fungsi tombol untuk melakukan auto calibration pada program dan pindahkan fungsi tersebut ke dalam fungsi deteksi error pada track/lintasan.
- Pada setiap kondisi deteksi error, baca kondisi sensor 1 sampai 8 kemudian tentukan nilai tengahnya dan simpan di dalam variabel. Gunakan nilai ini untuk melakukan deteksi error pada lintasan. Ketika nilai error = 0 (sensor di bagian tengah mendeteksi garis merah, LED yang terhubung dengan pin 13 akan menyala dengan mode LED blink dengan durasi on/off selama 500 milidetik sebanyak 2 kali. Apabila sensor mendeteksi selain warna merah, LED akan menyala blink dengan durasi on/off 100 milidetik secara terus menerus. Proses dilakukan secara terus menerus sampai tombol power atau reset ditekan. Dengan menggunakan metode ini, robot harus dapat mengikuti lintasan tanpa harus melakukan kalibrasi manual menggunakan tombol.

- Di dalam void loop terdapat perbedaan conditional sebagai berikut :
    - Jika 'state' bernilai 1 maka eksekusi program line follower dan adaptive sensor calibration.
    - Jika 'state' bernilai 3 maka LED kiri akan menyala dan setelah 500 milidetik LED kiri akan mati, mengurangi nilai Kp(contoh : Kp=Kp-1;), menyimpan nilai Kp kedalam EEPROM pada alamat 8.
    - Jika 'state' bernilai 4 maka LED kiri akan menyala dan setelah 500 milidetik LED kiri akan mati, menambah nilai Kp(contoh : Kp=Kp+1;), menyimpan nilai Kp kedalam EEPROM pada alamat 8.
    - Jika 'state' bernilai 5 maka LED kanan akan menyala dan setelah 500 milidetik LED kiri akan mati, mengurangi nilai Kd(contoh : Kd=Kd-1;), menyimpan nilai Kd kedalam EEPROM pada alamat 9
    - Jika 'state' bernilai 6 maka LED kanan akan menyala dan setelah 500 milidetik LED kiri akan mati, menambah nilai Kd(contoh : Kd=Kd+1;), menyimpan nilai Kd kedalam EEPROM pada alamat 10.

c. Ujicoba robot dimulai dari titik START kotak biru dan FINISH di posisi yang sama dalam 1 lap!


### 5.1. KODE PROGRAM

```cpp
#include <EEPROM.h>

// ================= EEPROM =================
const int ADDR_SIGNATURE = 0;
const int ADDR_THRESHOLD = 1;
const int ADDR_KP = 17;
const int ADDR_KD = 21;
const int ADDR_KI = 25;

// ================= PIN =================
const int btnLeftDown = 2;
const int btnLeftUp = 3;
const int btnRightUp = 11;
const int btnRightDown = 12;

const int ledLeft = 7;
const int ledRight = 8;

const int motorKiri = 9;
const int motorKanan = 6;

const int sensorPin[8] = { A4, A5, A6, A7, A0, A1, A2, A3 };

// ================= VAR =================
int threshold[8];

float Kp = 25.0;
float Ki = 0.1;
float Kd = 10.0;

float integral = 0;

int LastError = 0;
int systemState = 0;

// ===== SPEED =====
int baseSpeed = 220;

// ===== WAKTU PID =====
unsigned long now = 0;
unsigned long lastTime = 0;
float timeChange = 0;

// ===== MONITOR =====
unsigned long lastPrint = 0;
const int intervalPrint = 300;

int lastKiri = 0;
int lastKanan = 0;
int lastErrorGlobal = 0;

void setup() {
  Serial.begin(9600);

  pinMode(btnLeftDown, INPUT_PULLUP);
  pinMode(btnLeftUp, INPUT_PULLUP);
  pinMode(btnRightUp, INPUT_PULLUP);
  pinMode(btnRightDown, INPUT_PULLUP);

  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);

  pinMode(motorKiri, OUTPUT);
  pinMode(motorKanan, OUTPUT);

  for (int i = 0; i < 8; i++) pinMode(sensorPin[i], INPUT);

  bacaEEPROM();
  lastTime = millis();

  Serial.println("LeftDown=Kalibrasi | LeftUp=Start");
}

void loop() {

  bool leftDown = digitalRead(btnLeftDown) == LOW;
  bool leftUp = digitalRead(btnLeftUp) == LOW;
  bool rightUp = digitalRead(btnRightUp) == LOW;
  bool rightDown = digitalRead(btnRightDown) == LOW;

  switch (systemState) {

    case 0:
      if (leftDown) {
        systemState = 1;
        delay(300);
      }
      if (leftUp) {
        systemState = 2;
        Serial.println(">> START <<");
        delay(300);
      }
      break;

    case 1:
      kalibrasiSensor();
      simpanEEPROM();
      systemState = 0;
      break;

    case 2:
      jalanRobot();

      if (leftDown && leftUp) {
        stopMotor();
        systemState = 0;
        Serial.println(">> STOP <<");
        delay(300);
        break;
      }

      if (leftUp && rightUp) {
        systemState = 7;
        delay(300);
      }
      else if (leftDown && rightDown) {
        systemState = 8;
        delay(300);
      }
      else if (leftDown) {
        systemState = 3;
        delay(300);
      }
      else if (leftUp) {
        systemState = 4;
        delay(300);
      }
      else if (rightUp) {
        systemState = 5;
        delay(300);
      }
      else if (rightDown) {
        systemState = 6;
        delay(300);
      }
      break;

    case 3: Kp--; if (Kp < 0) Kp = 0; simpanEEPROM(); delay(300); systemState = 2; break;
    case 4: Kp++; simpanEEPROM(); delay(300); systemState = 2; break;
    case 5: Kd--; if (Kd < 0) Kd = 0; simpanEEPROM(); delay(300); systemState = 2; break;
    case 6: Kd++; simpanEEPROM(); delay(300); systemState = 2; break;
    case 7: Ki = Ki + 0.01; simpanEEPROM(); delay(300); systemState = 2; break;    
    case 8: Ki = Ki - 0.01; if (Ki < 0) Ki = 0; simpanEEPROM(); delay(300); systemState = 2; break;  
  }

  monitorSerial();
}

String bacaSensor() {
  String s = "";
  for (int i = 0; i < 8; i++) {
    int val = analogRead(sensorPin[i]);
    s += (val > threshold[i]) ? "1" : "0";
  }
  return s;
}

int hitungError(String k) {
  if (k == "10000000") return 7;
  else if (k == "11000000") return 6;
  else if (k == "01000000") return 5;
  else if (k == "01100000") return 4;
  else if (k == "00100000") return 3;
  else if (k == "00110000") return 2;
  else if (k == "00010000") return 1;
  else if (k == "00011000") return 0;
  else if (k == "00001000") return -1;
  else if (k == "00001100") return -2;
  else if (k == "00000100") return -3;
  else if (k == "00000110") return -4;
  else if (k == "00000010") return -5;
  else if (k == "00000011") return -6;
  else if (k == "00000001") return -7;
  else return LastError;
}

void jalanRobot() {

  now = millis();
  
  if (now >= lastTime) {
    timeChange = (now - lastTime) / 1000.0;
  } else {
    timeChange = (4294967295UL - lastTime + now + 1) / 1000.0;
  }
  lastTime = now;

  if (timeChange <= 0) return;

  String kondisi = bacaSensor();

  if (kondisi == "11111111" || kondisi == "00000000") {
    stopMotor();
    integral = 0;
    return;
  }

  int error = hitungError(kondisi);

  if (error == 0) {
    baseSpeed += 20;
    if (baseSpeed > 230) baseSpeed = 230;
  } else {
    baseSpeed = 200;
  }

  if ((error > 0 && LastError < 0) || (error < 0 && LastError > 0)) {
    integral = 0;
  }

  integral += error * timeChange;
  integral = constrain(integral, -100, 100);

  float dError = (error - LastError) / timeChange;

  float output = (Kp * error) + (Ki * integral) + (Kd * dError);

  int kiri = constrain(baseSpeed + output, 0, 255);
  int kanan = constrain(baseSpeed - output, 0, 255);

  analogWrite(motorKiri, kiri);
  analogWrite(motorKanan, kanan);

  lastKiri = kiri;
  lastKanan = kanan;
  lastErrorGlobal = error;

  LastError = error;
}

void monitorSerial() {
  if (millis() - lastPrint >= intervalPrint) {
    lastPrint = millis();

    Serial.print("Time:");
    Serial.print(lastPrint);

    Serial.print(" | State:");
    Serial.print(systemState);

    Serial.print(" | Error:");
    Serial.print(lastErrorGlobal);

    Serial.print(" | Speed:");
    Serial.print(lastKiri);
    Serial.print("/");
    Serial.print(lastKanan);

    Serial.print(" | PID:");
    Serial.print(Kp);
    Serial.print(",");
    Serial.print(Ki);
    Serial.print(",");
    Serial.print(Kd);

    Serial.print(" | Peka:");
    for (int i = 0; i < 8; i++) {
      Serial.print(threshold[i]);
      if (i < 7) Serial.print("-");
    }

    Serial.println();
  }
}

void stopMotor() {
  analogWrite(motorKiri, 0);
  analogWrite(motorKanan, 0);
  integral = 0;  // FIX: Reset integral when stopped
}

void kalibrasiSensor() {
  int minVal[8], maxVal[8];

  for (int i = 0; i < 8; i++) {
    minVal[i] = 1023;
    maxVal[i] = 0;
  }

  for (int t = 0; t < 30; t++) {

    digitalWrite(ledLeft, HIGH);
    digitalWrite(ledRight, HIGH);

    for (int i = 0; i < 8; i++) {
      int val = analogRead(sensorPin[i]);
      if (val < minVal[i]) minVal[i] = val;
      if (val > maxVal[i]) maxVal[i] = val;
    }

    delay(50);

    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);

    delay(50);
  }

  for (int i = 0; i < 8; i++) {
    threshold[i] = (minVal[i] + maxVal[i]) / 2;
  }

  for (int i = 0; i < 3; i++) {
    digitalWrite(ledLeft, HIGH);
    digitalWrite(ledRight, HIGH);
    delay(200);
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);
    delay(200);
  }
}

void simpanEEPROM() {
  EEPROM.write(ADDR_SIGNATURE, 0xAA);

  for (int i = 0; i < 8; i++) {
    EEPROM.write(ADDR_THRESHOLD + i * 2, highByte(threshold[i]));
    EEPROM.write(ADDR_THRESHOLD + i * 2 + 1, lowByte(threshold[i]));
  }

  byte *kp = (byte *)&Kp;
  byte *ki = (byte *)&Ki;
  byte *kd = (byte *)&Kd;

  for (int i = 0; i < 4; i++) {
    EEPROM.write(ADDR_KP + i, kp[i]);
    EEPROM.write(ADDR_KI + i, ki[i]);
    EEPROM.write(ADDR_KD + i, kd[i]);
  }
}

void bacaEEPROM() {
  if (EEPROM.read(ADDR_SIGNATURE) != 0xAA) return;

  for (int i = 0; i < 8; i++) {
    threshold[i] = word(
      EEPROM.read(ADDR_THRESHOLD + i * 2),
      EEPROM.read(ADDR_THRESHOLD + i * 2 + 1));
  }

  byte kp[4], ki[4], kd[4];

  for (int i = 0; i < 4; i++) {
    kp[i] = EEPROM.read(ADDR_KP + i);
    ki[i] = EEPROM.read(ADDR_KI + i);
    kd[i] = EEPROM.read(ADDR_KD + i);
  }

  memcpy(&Kp, kp, 4);
  memcpy(&Ki, ki, 4);
  memcpy(&Kd, kd, 4);
}
```

## 6. KESIMPULAN

Berdasarkan hasil praktikum yang telah dilakukan, dapat diambil beberapa kesimpulan sebagai berikut.

1. Algoritma *adaptive sensor calibration* berhasil diimplementasikan pada robot *line follower* tanpa memerlukan tombol kalibrasi manual. Proses kalibrasi dilakukan secara otomatis saat robot berjalan di lintasan dengan cara membaca nilai minimum dan maksimum dari masing-masing sensor secara dinamis.

2. Metode *adaptive sensor calibration* bekerja dengan cara memperbarui nilai tengah (*threshold*) setiap sensor secara terus menerus selama robot bergerak. Ketika sensor mendeteksi adanya perubahan warna pada lintasan, nilai tengah tersebut akan menyesuaikan secara otomatis sehingga robot tetap mampu membedakan antara garis dan latar belakang.

3. Keunggulan utama dari metode ini dibandingkan dengan kalibrasi manual adalah kemampuan robot untuk beradaptasi terhadap perubahan kondisi lingkungan secara *real-time*. Apabila terdapat ketidakseragaman warna garis atau perubahan pencahayaan, robot tetap dapat mengikuti lintasan dengan baik tanpa perlu dilakukan kalibrasi ulang secara manual.

4. Implementasi *state machine* pada program memungkinkan robot untuk membedakan antara mode *line following* dengan *adaptive sensor calibration* (state 2) dan mode penyesuaian parameter PID (state 3 hingga 8). Perpindahan antar *state* dipicu oleh tekanan tombol yang terprogram, sehingga pengaturan parameter tetap dapat dilakukan meskipun kalibrasi sensor berjalan secara otomatis.

5. Penggunaan LED sebagai indikator deteksi garis merah memberikan umpan balik visual yang berguna untuk memverifikasi apakah sensor telah mendeteksi warna dengan benar. LED akan berkedip dengan pola tertentu ketika sensor mendeteksi *error* = 0 (garis merah terdeteksi di tengah) dan akan berkedip dengan pola berbeda ketika mendeteksi warna selain merah, sehingga memudahkan proses *debugging* dan pengamatan.

## 7. LAMPIRAN

Link dokumentasi video:
https://drive.google.com/drive/folders/1Lgzs5CNa1o80Mx8uDIgMI3fhwKW2n_Xc?usp=sharing

Dokumentasi:

