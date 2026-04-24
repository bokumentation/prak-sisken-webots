// =====================================================
// PROGRAM LINE FOLLOWER 8 SENSOR - MODUL 6 PRAKTIKUM
// REV: 24 APRIL 2026
// NOTE: Penambahan parameter Kp dan Kd, EEPROM dan Push Button
// =====================================================
// KP 15; KD 5

#include <EEPROM.h>

// === DEFINISI PIN SENSOR ===
const byte S[8] = {A4, A5, A6, A7, A0, A1, A2, A3};

// === DEFINISI PIN MOTOR ===
const byte PIN_M_KANAN_MAJU = 9;
const byte PIN_M_KANAN_MUNDUR = 10;
const byte PIN_M_KIRI_MAJU = 6;
const byte PIN_M_KIRI_MUNDUR = 5;

// === LED ===
const int PIN_LED_KIRI = 7;
const int PIN_LED_TENGAH = 13;
const int PIN_LED_KANAN = 8;

// === TOMBOL ===
const int pin_btn_LB = 2;  // Kiri Belakang (D2)
const int pin_btn_LF = 3;  // Kiri Depan (D3)
const int pin_btn_RB = 12; // Kanan Belakang (D12)
const int pin_btn_RF = 11; // Kanan Depan (D11)

// === VARIABEL KALIBRASI & KENDALI ===
int sensor[8];
int NilaiTengahSensor[8];
int Kp, Kd;
int error = 0, lastError = 0;
int kecepatanSetPoint = 150;
bool isMoving = false;
bool bDetekPutih = false;
// Alamat EEPROM
const int ADDR_KP = 100;
const int ADDR_KD = 102;
const int ADDR_SENS = 0; // Alamat 0-15 untuk 8 sensor (int = 2 bytes)

void setup() {
  Serial.begin(9600);

  // Inisialisasi Pin
  pinMode(PIN_M_KANAN_MAJU, OUTPUT);
  pinMode(PIN_M_KANAN_MUNDUR, OUTPUT);
  pinMode(PIN_M_KIRI_MAJU, OUTPUT);
  pinMode(PIN_M_KIRI_MUNDUR, OUTPUT);
  pinMode(PIN_LED_KIRI, OUTPUT);
  pinMode(PIN_LED_TENGAH, OUTPUT);
  pinMode(PIN_LED_KANAN, OUTPUT);

  pinMode(pin_btn_LB, INPUT_PULLUP);
  pinMode(pin_btn_LF, INPUT_PULLUP);
  pinMode(pin_btn_RB, INPUT_PULLUP);
  pinMode(pin_btn_RF, INPUT_PULLUP);

  stopMotors();

  // KASUS 2: Baca konfigurasi dari EEPROM saat startup
  loadSettings();
  Serial.println("Sistem Siap.");
}

void loop() {
  checkButtons();

  if (isMoving) {
    bDetekPutih = false;
    runLineFollower();
  }
}

// === KASUS 1: Kalibrasi & Simpan EEPROM ===
void doCalibration() {
  Serial.println("Proses Kalibrasi Otomatis...");
  int minVal[8], maxVal[8];
  for (int i = 0; i < 8; i++) {
    minVal[i] = 1023;
    maxVal[i] = 0;
  }

  unsigned long start = millis();
  while (millis() - start < 5000) { // Kalibrasi 5 detik
    for (int i = 0; i < 8; i++) {
      int val = analogRead(S[i]);
      if (val < minVal[i])
        minVal[i] = val;
      if (val > maxVal[i])
        maxVal[i] = val;
      NilaiTengahSensor[i] = (minVal[i] + maxVal[i]) / 2;
    }
    // LED Blinking
    digitalWrite(PIN_LED_KIRI, !digitalRead(PIN_LED_KIRI));
    digitalWrite(PIN_LED_KANAN, !digitalRead(PIN_LED_KANAN));
    delay(100);
  }

  // Simpan ke EEPROM
  for (int i = 0; i < 8; i++) {
    EEPROM.put(ADDR_SENS + (i * 2), NilaiTengahSensor[i]);
  }

  digitalWrite(PIN_LED_KIRI, LOW);
  digitalWrite(PIN_LED_KANAN, LOW);
  displaySettings();
}

// === KASUS 2, 3, 4: Logika Tombol & EEPROM ===
void checkButtons() {
  // Tombol Kiri Belakang (Kasus 1: Kalibrasi | Kasus 3: Kp--)
  if (digitalRead(pin_btn_LB) == LOW) {
    delay(200); // Debounce
    if (!isMoving) {
      doCalibration(); // State 1
    } else {
      Kp--; // State 3
      EEPROM.put(ADDR_KP, Kp);
      Serial.print("Kp berkurang: ");
      Serial.println(Kp);
    }
  }

  // Tombol Kiri Depan (Kasus 2: Start | Kasus 3: Kp++)
  if (digitalRead(pin_btn_LF) == LOW) {
    delay(200);
    if (!isMoving) {
      isMoving = true; // State 2
      Serial.println("Robot Jalan (Line Follower)");
    } else {
      Kp++; // State 4
      EEPROM.put(ADDR_KP, Kp);
      Serial.print("Kp bertambah: ");
      Serial.println(Kp);
    }
  }

  // Tombol Kanan Belakang (Kasus 4: Kd--)
  if (digitalRead(pin_btn_RB) == LOW && isMoving) {
    delay(200);
    Kd--; // State 5
    EEPROM.put(ADDR_KD, Kd);
    Serial.print("Kd berkurang: ");
    Serial.println(Kd);
  }

  // Tombol Kanan Depan (Kasus 4: Kd++) - Berdasarkan Instruksi Kiri Depan
  // (State 6) Catatan: Karena instruksi Kasus 4 menyebutkan "tombol kiri depan"
  // lagi untuk Kd++, saya asumsikan ini menggunakan tombol kanan depan agar
  // tidak bentrok dengan Kp++.
  if (digitalRead(pin_btn_RF) == LOW && isMoving) {
    delay(200);
    Kd++; // State 6
    EEPROM.put(ADDR_KD, Kd);
    Serial.print("Kd bertambah: ");
    Serial.println(Kd);
  }
}

void loadSettings() {
  for (int i = 0; i < 8; i++) {
    EEPROM.get(ADDR_SENS + (i * 2), NilaiTengahSensor[i]);
  }
  EEPROM.get(ADDR_KP, Kp);
  EEPROM.get(ADDR_KD, Kd);

  // Proteksi jika EEPROM kosong/nan
  if (Kp < 0 || Kp > 100)
    Kp = 15;
  if (Kd < 0 || Kd > 100)
    Kd = 5;
}

void displaySettings() {
  Serial.println("\n--- DATA EEPROM ---");
  for (int i = 0; i < 8; i++) {
    Serial.print("Peka[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(NilaiTengahSensor[i]);
  }
  Serial.print("Kp: ");
  Serial.println(Kp);
  Serial.print("Kd: ");
  Serial.println(Kd);
  Serial.println("-------------------\n");
}

void runLineFollower() {
  String pola = "";
  for (int i = 0; i < 8; i++) {
    if (analogRead(S[i]) > NilaiTengahSensor[i])
      pola += "1";
    else
      pola += "0";
  }

  // Logika Error (Sama seperti kode awal)
  // if (pola == "00011000") error = 0;
  // else if (pola == "00010000") error = -1;
  // else if (pola == "00001000") error = 1;
  // ... (lanjutkan pemetaan error lainnya dari pola sensor) ...
  // else if (pola == "00000000") { stopMotors(); return; }

  // 2. CEK JIKA SEMUA SENSOR PUTIH (00000000) - GARIS HILANG
  if (pola == "00000000") {
    stopMotors();
    bDetekPutih = true;
    digitalWrite(PIN_LED_KIRI, LOW);
    digitalWrite(PIN_LED_TENGAH, LOW);
    digitalWrite(PIN_LED_KANAN, LOW);
    Serial.println("GARIS HILANG - ROBOT BERHENTI");
    delay(100);
    return;
  }
  // 3. LOGIKA PENENTUAN ERROR (Sesuai Ketentuan Modul)
  if (pola == "10000000")
    error = -7;
  else if (pola == "11000000")
    error = -6;
  else if (pola == "01000000")
    error = -5;
  else if (pola == "01100000")
    error = -4;
  else if (pola == "00100000")
    error = -3;
  else if (pola == "00110000")
    error = -2;
  else if (pola == "00010000")
    error = -1;
  else if (pola == "00011000")
    error = 0;
  else if (pola == "00001000")
    error = 1;
  else if (pola == "00001100")
    error = 2;
  else if (pola == "00000100")
    error = 3;
  else if (pola == "00000110")
    error = 4;
  else if (pola == "00000010")
    error = 5;
  else if (pola == "00000011")
    error = 6;
  else if (pola == "00000001")
    error = 7;
  else {
    // Jika pola tidak dikenali, gunakan lastError
    error = lastError;
  }

  int rate = error - lastError;
  int moveControl = (Kp * error) + (Kd * rate);
  lastError = error;

  int pwmKiri = constrain(kecepatanSetPoint + moveControl, 0, 255);
  int pwmKanan = constrain(kecepatanSetPoint - moveControl, 0, 255);

  analogWrite(PIN_M_KIRI_MAJU, pwmKiri);
  analogWrite(PIN_M_KANAN_MAJU, pwmKanan);
  digitalWrite(PIN_M_KIRI_MUNDUR, LOW);
  digitalWrite(PIN_M_KANAN_MUNDUR, LOW);
}

void stopMotors() {
  analogWrite(PIN_M_KIRI_MAJU, 0);
  analogWrite(PIN_M_KANAN_MAJU, 0);
}
