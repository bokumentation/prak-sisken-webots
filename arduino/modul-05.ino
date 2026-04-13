// =====================================================
// PROGRAM LINE FOLLOWER 8 SENSOR - MODUL 5 PRAKTIKUM
// REV: 13 APRIL 2026
// NOTE: Penambahan parameter Kp dan Kd
// =====================================================

// === DEFINISI PIN SENSOR (Urutan S1 - S8) ===
// A4=S1, A5=S2, A6=S3, A7=S4, A0=S5, A1=S6, A2=S7, A3=S8
const byte S[8] = { A4, A5, A6, A7, A0, A1, A2, A3 };

// === DEFINISI PIN MOTOR ===
const byte PIN_M_KANAN_MAJU = 9;
const byte PIN_M_KANAN_MUNDUR = 10;
const byte PIN_M_KIRI_MAJU = 6;
const byte PIN_M_KIRI_MUNDUR = 5;

// === LED ===
const int PIN_LED_KIRI = 7;
const int PIN_LED_TENGAH = 13;
const int PIN_LED_KANAN = 8;

const int pin_button_left_up = 3;
const int pin_button_left_down = 2;
const int pin_button_right_up = 11;
const int pin_button_right_down = 12;

// === VARIABEL KALIBRASI ===
int sensor[8];
int NilaiMaxSensor[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
int NilaiMinSensor[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int NilaiTengahSensor[8];

// === VARIABEL KENDALI ===
int Kp = 15;
int Kd = 5;
int moveControl = 0;
int error = 0;
int lastError = 0;
int rate = 0;
int kecepatanMotorKanan = 0;
int kecepatanMotorKiri = 0;
int kecepatanSetPoint = 150;
int stop = 100;
bool bDetekPutih = false;
bool isCalibrating = false;
bool isMoving = false;

// === KONFIGURASI LAIN ===
const int NILAI_THRESHOLD = 500;

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(9600);

  // Inisialisasi Pin Motor sebagai Output
  pinMode(PIN_M_KANAN_MAJU, OUTPUT);
  pinMode(PIN_M_KANAN_MUNDUR, OUTPUT);
  pinMode(PIN_M_KIRI_MAJU, OUTPUT);
  pinMode(PIN_M_KIRI_MUNDUR, OUTPUT);

  // Inisialisasi LED
  pinMode(PIN_LED_KIRI, OUTPUT);
  pinMode(PIN_LED_TENGAH, OUTPUT);
  pinMode(PIN_LED_KANAN, OUTPUT);

  // Inisialisasi Pin Tombol sebagai Input dengan Pull-up
  pinMode(pin_button_left_down, INPUT_PULLUP);
  pinMode(pin_button_left_up, INPUT_PULLUP);
  pinMode(pin_button_right_down, INPUT_PULLUP);
  pinMode(pin_button_right_up, INPUT_PULLUP);

  // Pastikan motor berhenti saat awal nyala
  stopMotors();
  
  // Inisialisasi nilai tengah sensor dengan nilai default
  for (int i = 0; i < 8; i++) {
    NilaiTengahSensor[i] = NILAI_THRESHOLD;
  }
  
  Serial.println("Robot Line Follower Ready!");
  Serial.println("Tekan tombol kiri bawah (D2) untuk kalibrasi");
  Serial.println("Tekan tombol kiri atas (D3) untuk mulai");
  delay(2000);  // Delay persiapan 2 detik
}

void loop() {
  // Cek tombol kalibrasi (pin_button_left_down / D2)
  if (digitalRead(pin_button_left_down) == LOW && !isCalibrating && !isMoving) {
    isCalibrating = true;
    isMoving = false;
    doCalibration();
    isCalibrating = false;
  }

  // Cek tombol start (pin_button_left_up / D3)
  if (digitalRead(pin_button_left_up) == LOW && !isCalibrating && !isMoving) {
    isMoving = true;
    isCalibrating = false;
    Serial.println("Mode Running diaktifkan");
  }

  // Jika tidak dalam mode running, tunggu saja
  if (!isMoving) {
    delay(100);
    return;
  }

  // Reset flag deteksi putih setiap loop
  bDetekPutih = false;
  
  // 1. BACA SENSOR SEKALIGUS BUAT POLA STRING
  String pola = "";
  String nilai_sensor = "";
  for (int i = 0; i < 8; i++) {
    int nilai_baca = analogRead(S[i]);
    sensor[i] = nilai_baca;
    nilai_sensor += String(nilai_baca);
    if (i < 7) nilai_sensor += ",";

    // Gunakan threshold dinamis dari kalibrasi
    if (nilai_baca > NilaiTengahSensor[i]) {
      pola += "1";
    } else {
      pola += "0";
    }
  }

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

  // === TAMBAHAN: LOGIKA LED INDIKATOR ===
  if (error < 0) {
    // Garis di kiri, robot harus koreksi ke kiri
    digitalWrite(PIN_LED_KIRI, HIGH);
    digitalWrite(PIN_LED_TENGAH, LOW);
    digitalWrite(PIN_LED_KANAN, LOW);
  } else if (error > 0) {
    // Garis di kanan, robot harus koreksi ke kanan
    digitalWrite(PIN_LED_KIRI, LOW);
    digitalWrite(PIN_LED_TENGAH, LOW);
    digitalWrite(PIN_LED_KANAN, HIGH);
  } else {
    // Error = 0 (Pas di tengah)
    digitalWrite(PIN_LED_KIRI, LOW);
    digitalWrite(PIN_LED_TENGAH, HIGH);
    digitalWrite(PIN_LED_KANAN, LOW);
  }

  // KALKULASI KONTROL GERAK PD (Poin 2, 3, 4 Modul)
  rate = error - lastError;
  moveControl = (Kp * error) + (Kd * rate);

  kecepatanMotorKiri = kecepatanSetPoint + moveControl;
  kecepatanMotorKanan = kecepatanSetPoint - moveControl;

  // EKSEKUSI KE MOTOR (Poin 5 & 6 Modul)
  // Membatasi nilai agar tetap di range PWM 0-255
  int pwmKiri = constrain(kecepatanMotorKiri, 0, 255);
  int pwmKanan = constrain(kecepatanMotorKanan, 0, 255);

  // Maju
  analogWrite(PIN_M_KIRI_MAJU, pwmKiri);
  analogWrite(PIN_M_KANAN_MAJU, pwmKanan);

  // Memastikan pin mundur tetap LOW
  digitalWrite(PIN_M_KIRI_MUNDUR, LOW);
  digitalWrite(PIN_M_KANAN_MUNDUR, LOW);

  // UPDATE LAST ERROR (Poin 1 Modul)
  // lastError = error;
  lastError = error;


  // KIRIM NILAI ERROR KE SERIAL MONITOR (Ketentuan Poin b)
  Serial.print("Sensor: ");
  Serial.print(nilai_sensor);
  Serial.print(" | Pola: ");
  Serial.print(pola);
  Serial.print(" | error = ");
  Serial.print(error);
  Serial.print(" | PWM Kiri: ");
  Serial.print(pwmKiri);
  Serial.print(" | PWM Kanan: ");
  Serial.println(pwmKanan);

  delay(10);  // Penundaan kecil untuk stabilitas loop
}

// Fungsi pembantu untuk menghentikan motor
void stopMotors() {
  analogWrite(PIN_M_KIRI_MAJU, 0);
  analogWrite(PIN_M_KANAN_MAJU, 0);
  digitalWrite(PIN_M_KIRI_MUNDUR, LOW);
  digitalWrite(PIN_M_KANAN_MUNDUR, LOW);
}

void doCalibration() {
  Serial.println("Memulai kalibrasi sensor...");
  Serial.println("Gerakkan robot di atas garis hitam dan latar putih");
  Serial.println("Kalibrasi berlangsung 10 detik");
  
  unsigned long startTime = millis();
  unsigned long calibrationDuration = 10000;
  
  while (millis() - startTime < calibrationDuration) {
    // Baca semua sensor
    for (int i = 0; i < 8; i++) {
      sensor[i] = analogRead(S[i]);
      
      if (sensor[i] > NilaiMinSensor[i]) {
        NilaiMinSensor[i] = sensor[i];
      }
      
      if (sensor[i] < NilaiMaxSensor[i]) {
        NilaiMaxSensor[i] = sensor[i];
      }
      
      NilaiTengahSensor[i] = (NilaiMinSensor[i] + NilaiMaxSensor[i]) / 2;
    }
    
    // LED berkedip sebagai indikator
    digitalWrite(PIN_LED_KIRI, HIGH);
    digitalWrite(PIN_LED_TENGAH, HIGH);
    digitalWrite(PIN_LED_KANAN, HIGH);
    delay(200);
    digitalWrite(PIN_LED_KIRI, LOW);
    digitalWrite(PIN_LED_TENGAH, LOW);
    digitalWrite(PIN_LED_KANAN, LOW);
    delay(200);
    
    // Tampilkan progress setiap 2 detik
    unsigned long elapsed = millis() - startTime;
    if (elapsed % 2000 < 100) {
      Serial.print("Progress: ");
      Serial.print(elapsed / 1000);
      Serial.println(" detik");
    }
  }
  
  // Tampilkan hasil kalibrasi
  Serial.println("Kalibrasi selesai!");
  Serial.println("Hasil kalibrasi:");
  for (int i = 0; i < 8; i++) {
    Serial.print("Sensor ");
    Serial.print(i+1);
    Serial.print(": Min=");
    Serial.print(NilaiMinSensor[i]);
    Serial.print(", Max=");
    Serial.print(NilaiMaxSensor[i]);
    Serial.print(", Tengah=");
    Serial.println(NilaiTengahSensor[i]);
  }
  
  // LED menyala tetap sebagai indikasi selesai
  digitalWrite(PIN_LED_KIRI, HIGH);
  digitalWrite(PIN_LED_TENGAH, HIGH);
  digitalWrite(PIN_LED_KANAN, HIGH);
  delay(1000);
  digitalWrite(PIN_LED_KIRI, LOW);
  digitalWrite(PIN_LED_TENGAH, LOW);
  digitalWrite(PIN_LED_KANAN, LOW);
  
  Serial.println("Kalibrasi berhasil. Tekan tombol kiri atas (D3) untuk mulai.");
}
