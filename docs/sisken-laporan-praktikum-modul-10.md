Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<center> <h1>
LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 10: Praktikum Sistem Kendali berbasis Artificial Intelligence (AI) : Kasus Garis Intersection dan Warna
</h1> </center>

---

## 1. JUDUL PRAKTIKUM
Praktikum Sistem Kendali berbasis Artificial Intelligence (AI) : Kasus Garis Intersection dan Warna

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah :
1. Mahasiswa dapat membuat program Sistem Kendali berbasis Artificial Intelligence (AI): Kasus Garis Intersection dan Warna

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

### 4.1. Kecerdasan Buatan dalam Sistem Kendali Robot *Line Follower*

Kecerdasan buatan atau *Artificial Intelligence* (AI) dalam sistem kendali memberikan kemampuan bagi robot untuk mengambil keputusan secara cerdas berdasarkan data yang diterima dari sensor. Pada praktikum ini, AI diimplementasikan dalam bentuk logika pengambilan keputusan yang memungkinkan robot mendeteksi kondisi *START* dan *FINISH* secara otomatis, menghitung jumlah putaran (*lap*), serta menentukan kapan harus berhenti. Sistem kendali berbasis AI ini bekerja dengan memanfaatkan data dari 8 sensor yang dipasang pada robot untuk membaca kondisi lintasan, kemudian memprosesnya menggunakan algoritma *state machine* dan kontrol PID yang telah diprogram pada mikrokontroler Arduino Nano.

### 4.2. Deteksi Warna dan Garis menggunakan Sensor Optik

Sensor optik yang digunakan pada robot *line follower* bekerja berdasarkan prinsip pemantulan cahaya. Sensor terdiri dari LED pemancar cahaya dan fotodioda sebagai penerima. Ketika cahaya dipancarkan ke permukaan lintasan, intensitas cahaya yang dipantulkan kembali ke fotodioda akan berbeda tergantung pada warna permukaan tersebut. Permukaan berwarna terang (putih) akan memantulkan lebih banyak cahaya dibandingkan permukaan berwarna gelap (hitam). Perbedaan intensitas ini dibaca oleh mikrokontroler sebagai nilai ADC (*Analog to Digital Converter*) yang berkisar antara 0 hingga 1023.

Proses *thresholding* digunakan untuk mengubah nilai ADC menjadi nilai biner (0 atau 1) yang merepresentasikan deteksi garis. Nilai *threshold* ditentukan melalui proses kalibrasi, yaitu dengan mengambil nilai minimum dan maksimum dari setiap sensor kemudian dihitung nilai tengahnya. Ketika seluruh sensor mendeteksi garis hitam (semua sensor bernilai 1), kondisi ini disebut sebagai *all-black condition* yang menjadi penanda kotak *START* atau *FINISH* pada lintasan.

### 4.3. Konsep *START*/*FINISH* dan *Lap Counting* pada Lintasan Robot

Kondisi *START*/*FINISH* merupakan sebuah area khusus pada lintasan yang berbentuk kotak berwarna hitam dengan ukuran yang cukup besar sehingga seluruh sensor robot membaca nilai hitam secara bersamaan. Ketika robot mendeteksi kondisi ini, sistem akan mencatatnya sebagai satu kali putaran (*lap*). Mekanisme *lap counting* diimplementasikan dengan menggunakan variabel `lapCount` yang akan bertambah setiap kali sensor mendeteksi kondisi *all-black*.

Pada praktikum ini, robot dirancang untuk melakukan 2 kali putaran secara otomatis. Ketika *lap* pertama terdeteksi, robot akan menjalankan *finish sequence* berupa putaran balik (*U-turn*) untuk melanjutkan ke putaran kedua. Ketika *lap* kedua terdeteksi, robot akan berhenti secara otomatis. Untuk menghindari deteksi ganda pada area yang sama, digunakan variabel `startBoxCleared` yang memastikan robot hanya mendeteksi kondisi *START*/*FINISH* setelah benar-benar meninggalkan area kotak hitam tersebut.

### 4.4. *Finite State Machine* dalam Pengendalian Robot

*State machine* atau mesin keadaan digunakan untuk mengatur mode operasi robot secara terstruktur. Terdapat beberapa *state* utama dalam program ini, yaitu *state* 0 (*standby*), *state* 1 (kalibrasi), *state* 2 (*line following*), *state* 3 hingga 8 (penyesuaian parameter PID), dan *state* 9 (*finish sequence*). Setiap *state* memiliki fungsi dan perilaku yang berbeda. Perpindahan antar *state* dipicu oleh kombinasi tekanan tombol atau kondisi tertentu yang terdeteksi oleh sensor.

Pada *state* 2 (*line following*), robot secara terus menerus membaca sensor, menghitung *error*, dan menggerakkan motor berdasarkan kontrol PID. Ketika sensor mendeteksi kondisi *all-black* dan robot sudah meninggalkan kotak *start* (`startBoxCleared = true`), sistem akan menghitung *lap* dan berpindah ke *state* 9 untuk menjalankan *finish sequence* yang terdiri dari serangkaian gerakan putaran untuk membalikkan arah robot.

### 4.5. *PID Controller* untuk Pengendalian Gerak Robot

PID (*Proportional-Integral-Derivative*) *controller* merupakan algoritma kendali yang digunakan untuk menjaga robot tetap berada di jalur garis. Nilai *error* dihitung berdasarkan posisi garis yang terdeteksi oleh sensor, dengan *error* 0 menunjukkan robot berada tepat di tengah garis. Nilai *error* ini kemudian diproses melalui tiga komponen:

- Komponen proporsional (Kp) menghasilkan koreksi yang sebanding dengan *error* saat ini.
- Komponen integral (Ki) mengakumulasi *error* dari waktu ke waktu untuk menghilangkan *offset*.
- Komponen derivatif (Kd) memberikan koreksi berdasarkan laju perubahan *error* untuk mengurangi *overshoot*.

Nilai PID dapat disesuaikan secara langsung melalui tombol pada robot tanpa harus memprogram ulang, dan parameter tersebut disimpan ke dalam EEPROM agar tetap tersimpan meskipun robot dimatikan.

### 4.6. Kalibrasi Sensor Adaptif dan Penyimpanan Parameter

Kalibrasi sensor adaptif memungkinkan robot untuk menentukan nilai tengah (*threshold*) dari setiap sensor secara otomatis pada saat proses kalibrasi awal. Robot akan menggerakkan sensornya di atas area gelap dan terang untuk mendapatkan nilai minimum dan maksimum dari setiap sensor, kemudian menghitung nilai tengahnya. Hasil kalibrasi ini disimpan ke dalam memori EEPROM pada mikrokontroler agar tidak hilang ketika daya dimatikan.

EEPROM (*Electrically Erasable Programmable Read-Only Memory*) digunakan untuk menyimpan data kalibrasi *threshold* (16 byte untuk 8 sensor), serta parameter PID yaitu Kp, Ki, dan Kd (masing-masing 4 byte). Dengan penyimpanan ini, robot dapat langsung beroperasi tanpa perlu melakukan kalibrasi ulang setiap kali dinyalakan, selama parameter yang tersimpan masih sesuai dengan kondisi lintasan.

## 5. KEGIATAN PRAKTIKUM

a. Dengan menggunakan tabel warna dan program pada praktikum sebelumnya, buatlah program agar sensor pada robot dapat mendeteksi perbedaan warna antara garis dan lingkungan sekitar garis secara otomatis. Kondisi ini disebut dengan kondisi START/FINISH.

b. Apabila kondisi ini terdeteksi sebanyak 1 kali, maka robot secara otomatis akan mengeksekusi algoritma fuzzy untuk adaptive sensor calibration pada praktikum sebelumnya (kondisi START).

c.Apabila kondisi ini terdeteksi sebanyak 2 kali, maka robot akan berhenti secara otomatis (kondisi FINISH). Ujicoba robot dimulai dari titik START dan FINISH di posisi yang sama dalam 1 lap!

d. Modifikasi program agar robot dapat mulai bergerak di lintasan sebanyak 2 lap kemudian berhenti di kotak START/FINISH.

e. Screenshoot keluaran serial monitor untuk setiap kondisi. Cetak dan tempelkan pada buku jurnal praktikum.


### 5.1. KODE PROGRAM

```cpp
// Modul 10
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

// ===== LAP & FINISH =====
int lapCount = 0;
const unsigned long TURN_TIME_90 = 800;   // calibrate for 90° right turn
const unsigned long EXIT_TIME    = 400;   // time to drive off the start box
const unsigned long START_DELAY  = 1500;  // time on start box before auto‑start

// ===== LED NON‑BLOCKING =====
unsigned long turnBlinkTimer   = 0;
bool turnBlinkState            = false;
bool leftTurnActive            = false;
bool rightTurnActive           = false;

unsigned long policeTimer      = 0;
int  policePhase               = 0;
bool policeActive              = false;

// ===== SEQUENCE STATE MACHINE (systemState 9) =====
int  seqStep                   = 0;
unsigned long seqStartTime     = 0;

// ===== AUTO START GLOBALS =====
bool autoStartCounting = false;
unsigned long autoStartTimer = 0;

// ===== BUGFIX: Prevent finish detection right after start =====
bool startBoxCleared = false;   // true only after we've left the start box

// ================= SETUP =================
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

// ================= LOOP =================
void loop() {
  // Update LEDs in every loop iteration
  updatePoliceLights();
  updateTurnLEDs();

  bool leftDown = digitalRead(btnLeftDown) == LOW;
  bool leftUp   = digitalRead(btnLeftUp) == LOW;
  bool rightUp  = digitalRead(btnRightUp) == LOW;
  bool rightDown= digitalRead(btnRightDown) == LOW;

  switch (systemState) {

    case 0:  // Standby (manual & auto start)
      if (leftDown) {
        systemState = 1;
        delay(300);
      }
      if (leftUp) {
        systemState = 2;
        Serial.println(">> START (manual) <<");
        lapCount = 0;
        startBoxCleared = false;   // we are on the start box
        delay(300);
      }
      checkAutoStart();
      break;

    case 1:  // Calibration
      kalibrasiSensor();
      simpanEEPROM();
      systemState = 0;
      break;

    case 2:  // Line following
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

    case 9:  // Finish sequence (U-turn)
      runFinishSequence();
      break;
  }

  monitorSerial();
}

// ================= AUTO START =================
void checkAutoStart() {
  String kondisi = bacaSensor();

  if (kondisi == "11111111") {
    if (!autoStartCounting) {
      autoStartCounting = true;
      autoStartTimer = millis();
    }
    policeActive = true;

    if (millis() - autoStartTimer >= START_DELAY) {
      systemState = 2;
      policeActive = false;
      autoStartCounting = false;
      lapCount = 0;
      startBoxCleared = false;   // we start ON the box, so not cleared yet
      Serial.println(">> AUTO START <<");
      delay(100);
    }
  } else {
    autoStartCounting = false;
    policeActive = false;
  }
}

// ================= SENSOR =================
String bacaSensor() {
  String s = "";
  for (int i = 0; i < 8; i++) {
    int val = analogRead(sensorPin[i]);
    s += (val > threshold[i]) ? "1" : "0";
  }
  return s;
}

// ================= ERROR (ORIGINAL) =================
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

// ================= PID & LINE FOLLOWING =================
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

  // ---- Finish box detection (only after we've left the start) ----
  if (kondisi == "11111111" && startBoxCleared) {
    lapCount++;
    stopMotor();
    integral = 0;
    baseSpeed = 220;
    LastError = 0;
    startBoxCleared = false;   // will be reset again after the U-turn

    if (lapCount == 1) {
      systemState = 9;
      seqStep = 0;
      Serial.println(">> LAP 1 FINISH – turning around <<");
    } else if (lapCount == 2) {
      lapCount = 0;
      systemState = 0;
      Serial.println(">> RACE FINISHED <<");
    }
    return;
  }

  // ---- Lost line (all white) ----
  if (kondisi == "00000000") {
    stopMotor();
    integral = 0;
    return;
  }

  int error = hitungError(kondisi);

  // ---- Mark that we've left the start box (sensors not all black) ----
  if (kondisi != "11111111") {
    startBoxCleared = true;
  }

  // ---- Speed control ----
  if (error == 0) {
    baseSpeed += 20;
    if (baseSpeed > 230) baseSpeed = 230;
  } else {
    baseSpeed = 200;
  }

  // ---- Integral windup prevention ----
  if ((error > 0 && LastError < 0) || (error < 0 && LastError > 0)) {
    integral = 0;
  }

  integral += error * timeChange;
  integral = constrain(integral, -100, 100);

  float dError = (error - LastError) / timeChange;
  float output = (Kp * error) + (Ki * integral) + (Kd * dError);

  int kiri  = constrain(baseSpeed + output, 0, 255);
  int kanan = constrain(baseSpeed - output, 0, 255);

  analogWrite(motorKiri, kiri);
  analogWrite(motorKanan, kanan);

  // ---- Turn indicator LEDs ----
  leftTurnActive  = (kiri < kanan);
  rightTurnActive = (kiri > kanan);

  lastKiri = kiri;
  lastKanan = kanan;
  lastErrorGlobal = error;
  LastError = error;
}

// ================= FINISH SEQUENCE (state 9) =================
void runFinishSequence() {

  switch (seqStep) {
    
    case 0:  // Start first right turn
      analogWrite(motorKiri,  200);  // left forward
      analogWrite(motorKanan, 0);    // right stop -> turn right
      seqStartTime = millis();
      seqStep = 1;
      break;

    case 1:  // Wait for turn to complete
      if (millis() - seqStartTime >= TURN_TIME_90) {
        stopMotor();
        seqStep = 2;  // now try to leave the finish box
      }
      break;

    case 2:  // Drive forward until we LEAVE the finish box (sensors all black)
      analogWrite(motorKiri,  180);
      analogWrite(motorKanan, 180);
      if (bacaSensor() != "11111111") {
        // We just left the black box – keep driving and go to next step
        seqStep = 3;
      }
      break;

    case 3:  // Continue forward until we FIND the start box (all black again)
      analogWrite(motorKiri,  180);
      analogWrite(motorKanan, 180);
      if (bacaSensor() == "11111111") {
        stopMotor();
        seqStep = 4;  // start box found, prepare second turn
      }
      break;

    case 4:  // Second right turn
      analogWrite(motorKiri,  200);
      analogWrite(motorKanan, 0);
      seqStartTime = millis();
      seqStep = 5;
      break;

    case 5:  // Wait second turn done
      if (millis() - seqStartTime >= TURN_TIME_90) {
        stopMotor();
        seqStep = 6;
        seqStartTime = millis();  // for exit timing
      }
      break;

    case 6:  // Drive forward to exit the start box (clear it)
      analogWrite(motorKiri,  150);
      analogWrite(motorKanan, 150);
      // We exit after a fixed time OR when sensors leave black – whichever is safer
      // Using a fixed time avoids getting stuck if sensors misread.
      if (millis() - seqStartTime >= EXIT_TIME) {
        stopMotor();
        seqStep = 0;           // sequence complete
        systemState = 2;       // back to line following for lap 2
        startBoxCleared = false;  // we are still on the box area, not cleared yet
        Serial.println(">> LAP 2 – GO! <<");
      }
      break;
  }
}

// ================= LED UPDATES =================
void updateTurnLEDs() {
  if (systemState != 2) {
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);
    leftTurnActive = false;
    rightTurnActive = false;
    return;
  }

  if (leftTurnActive || rightTurnActive) {
    if (millis() - turnBlinkTimer >= 150) {
      turnBlinkTimer = millis();
      turnBlinkState = !turnBlinkState;

      if (leftTurnActive) {
        digitalWrite(ledLeft, turnBlinkState);
        digitalWrite(ledRight, LOW);
      } else {
        digitalWrite(ledRight, turnBlinkState);
        digitalWrite(ledLeft, LOW);
      }
    }
  } else {
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);
  }
}

void updatePoliceLights() {
  if (!policeActive) {
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);
    return;
  }

  if (millis() - policeTimer >= 100) {
    policeTimer = millis();
    policePhase = (policePhase + 1) % 4;
    switch (policePhase) {
      case 0: digitalWrite(ledLeft, HIGH); digitalWrite(ledRight, LOW); break;
      case 1: digitalWrite(ledLeft, LOW);  digitalWrite(ledRight, LOW); break;
      case 2: digitalWrite(ledLeft, LOW);  digitalWrite(ledRight, HIGH); break;
      case 3: digitalWrite(ledLeft, LOW);  digitalWrite(ledRight, LOW); break;
    }
  }
}

// ================= MONITOR =================
void monitorSerial() {
  if (millis() - lastPrint >= intervalPrint) {
    lastPrint = millis();

    Serial.print("Time:"); Serial.print(lastPrint);
    Serial.print(" | State:"); Serial.print(systemState);
    Serial.print(" | Error:"); Serial.print(lastErrorGlobal);
    Serial.print(" | Speed:"); Serial.print(lastKiri); Serial.print("/"); Serial.print(lastKanan);
    Serial.print(" | PID:"); Serial.print(Kp); Serial.print(","); Serial.print(Ki); Serial.print(","); Serial.print(Kd);
    Serial.print(" | Peka:");
    for (int i = 0; i < 8; i++) {
      Serial.print(threshold[i]);
      if (i < 7) Serial.print("-");
    }
    Serial.println();
  }
}

// ================= STOP =================
void stopMotor() {
  analogWrite(motorKiri, 0);
  analogWrite(motorKanan, 0);
  integral = 0;
}

// ================= KALIBRASI =================
void kalibrasiSensor() {
  int minVal[8], maxVal[8];
  for (int i = 0; i < 8; i++) { minVal[i] = 1023; maxVal[i] = 0; }

  for (int t = 0; t < 30; t++) {
    digitalWrite(ledLeft, HIGH); digitalWrite(ledRight, HIGH);
    for (int i = 0; i < 8; i++) {
      int val = analogRead(sensorPin[i]);
      if (val < minVal[i]) minVal[i] = val;
      if (val > maxVal[i]) maxVal[i] = val;
    }
    delay(50);
    digitalWrite(ledLeft, LOW); digitalWrite(ledRight, LOW);
    delay(50);
  }

  for (int i = 0; i < 8; i++) threshold[i] = (minVal[i] + maxVal[i]) / 2;

  for (int i = 0; i < 3; i++) {
    digitalWrite(ledLeft, HIGH); digitalWrite(ledRight, HIGH); delay(200);
    digitalWrite(ledLeft, LOW); digitalWrite(ledRight, LOW); delay(200);
  }
}

// ================= EEPROM =================
void simpanEEPROM() {
  EEPROM.write(ADDR_SIGNATURE, 0xAA);
  for (int i = 0; i < 8; i++) {
    EEPROM.write(ADDR_THRESHOLD + i*2, highByte(threshold[i]));
    EEPROM.write(ADDR_THRESHOLD + i*2+1, lowByte(threshold[i]));
  }
  byte *kp = (byte*)&Kp, *ki = (byte*)&Ki, *kd = (byte*)&Kd;
  for (int i=0; i<4; i++) {
    EEPROM.write(ADDR_KP+i, kp[i]);
    EEPROM.write(ADDR_KI+i, ki[i]);
    EEPROM.write(ADDR_KD+i, kd[i]);
  }
}

void bacaEEPROM() {
  if (EEPROM.read(ADDR_SIGNATURE) != 0xAA) return;
  for (int i = 0; i < 8; i++)
    threshold[i] = word(EEPROM.read(ADDR_THRESHOLD+i*2), EEPROM.read(ADDR_THRESHOLD+i*2+1));
  byte kp[4], ki[4], kd[4];
  for (int i=0; i<4; i++) {
    kp[i] = EEPROM.read(ADDR_KP+i);
    ki[i] = EEPROM.read(ADDR_KI+i);
    kd[i] = EEPROM.read(ADDR_KD+i);
  }
  memcpy(&Kp, kp, 4); memcpy(&Ki, ki, 4); memcpy(&Kd, kd, 4);
}
```

## 6. KESIMPULAN

Berdasarkan hasil praktikum yang telah dilakukan, dapat diambil beberapa kesimpulan sebagai berikut.

1. Robot *line follower* berhasil mendeteksi kondisi *START*/*FINISH* secara otomatis melalui pembacaan 8 sensor yang mendeteksi seluruh permukaan hitam (*all-black condition* dengan nilai biner 11111111). Kondisi ini menjadi penanda awal dan akhir lintasan yang digunakan sebagai acuan perhitungan jumlah putaran.

2. Mekanisme *lap counting* berjalan dengan baik menggunakan variabel `lapCount` yang bertambah setiap kali sensor mendeteksi kondisi *all-black*. Robot dirancang untuk mengeksekusi putaran sebanyak 2 kali secara otomatis. Pada putaran pertama, robot menjalankan *finish sequence* berupa putaran balik (*U-turn*) untuk melanjutkan ke putaran kedua, sedangkan pada putaran kedua robot berhenti secara otomatis.

3. Implementasi *state machine* dengan 10 *state* berhasil mengelola berbagai mode operasi robot, mulai dari *standby*, kalibrasi, *line following*, penyesuaian parameter PID, hingga *finish sequence*. Perpindahan antar *state* dipicu oleh kombinasi tekanan tombol dan kondisi sensor yang terdeteksi.

4. Kontrol PID yang digunakan pada *state* 2 (*line following*) mampu menjaga robot tetap berada di jalur garis dengan melakukan koreksi kecepatan motor kiri dan kanan berdasarkan nilai *error* yang dihitung dari posisi garis terhadap sensor.

5. Proses kalibrasi sensor adaptif dan penyimpanan parameter ke dalam EEPROM memungkinkan robot untuk langsung beroperasi tanpa perlu melakukan kalibrasi ulang setiap kali dinyalakan, selama parameter yang tersimpan masih sesuai dengan kondisi lintasan.

## 7. LAMPIRAN

Link dokumentasi video:
https://drive.google.com/drive/folders/1Lgzs5CNa1o80Mx8uDIgMI3fhwKW2n_Xc?usp=sharing

Dokumentasi:


