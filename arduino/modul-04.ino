// =====================================================
// PROGRAM LINE FOLLOWER 8 SENSOR - MODUL 4 PRAKTIKUM
// REV: 13 APRIL 2026
// NOTE: Penambahan parameter Kp
// =====================================================

// === DEFINISI PIN SENSOR (Urutan S1 - S8) ===
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

// === VARIABEL SESUAI INSTRUKSI MODUL ===
int Kp = 18; // Dinaikkan dari 5 ke 15 untuk koreksi lebih kuat
int moveControl = 0;
int error = 0;
int lastError = 0;
int kecepatanMotorKanan = 0;
int kecepatanMotorKiri = 0;
int kecepatanSetPoint = 150;
int stop = 100;
bool bDetekPutih = false;

// === KONFIGURASI LAIN ===
const int NILAI_THRESHOLD = 500; // Nilai batas hitam/putih

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

  // Pastikan motor berhenti saat awal nyala
  stopMotors();
  Serial.println("Robot Line Follower Ready!");
  delay(2000); // Delay persiapan 2 detik
}

void loop() {
  // 1. BACA SENSOR SEKALIGUS BUAT POLA STRING
  String pola = "";
  String nilai_sensor = "";
  for (int i = 0; i < 8; i++) {
    int nilai_baca = analogRead(S[i]);
    nilai_sensor += String(nilai_baca);
    if (i < 7)
      nilai_sensor += ",";

    // PERBAIKAN: Balikkan logika threshold
    // 1 = PUTIH (nilai tinggi), 0 = HITAM (nilai rendah)
    if (nilai_baca > NILAI_THRESHOLD) {
      pola += "1"; // Putih
    } else {
      pola += "0"; // Hitam
    }
  }

  // 2. LOGIKA PENENTUAN ERROR (Sesuai Ketentuan Modul)
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
    error = lastError;
    bDetekPutih = true;
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

  // KALKULASI KONTROL GERAK (Poin 2, 3, 4 Modul)
  // Simpan nilai moveControl = perkalian Kp dan error
  moveControl = Kp * error;

  // Hitung kecepatan motor (Diferensial)
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
  if (bDetekPutih == false) {
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
  }

  if (bDetekPutih == true) {
    Serial.print(" | D: ");
    Serial.println("Garis Hitam Tidak terdeteksi");
  }

  delay(10);
}

void stopMotors() {
  analogWrite(PIN_M_KIRI_MAJU, 0);
  analogWrite(PIN_M_KANAN_MAJU, 0);
  digitalWrite(PIN_M_KIRI_MUNDUR, LOW);
  digitalWrite(PIN_M_KANAN_MUNDUR, LOW);
}