// =====================================================
// PROGRAM LINE FOLLOWER 8 SENSOR - MODUL 2 PRAKTIKUM
// REV: 10 APRIL 2026
// CATATAN: PWM sederhana menggunakan boolean
// =====================================================

// Yang berhasi: pwm kanan 6. pwm kiri 9

// === LED ===
const int PIN_LED_KIRI = 7;
const int PIN_LED_TENGAH = 13;
const int PIN_LED_KANAN = 8;

// === MOTOR ====
const int PIN_PWM_MOTOR_KANAN_MAJU = 6;   
const int PIN_PWM_MOTOR_KANAN_MUNDUR = 5; 
const int PIN_PWM_MOTOR_KIRI_MAJU = 9;
const int PIN_PWM_MOTOR_MUNDUR_MUNDUR = 10;

// === SENSOR ===
const int JUMLAH_SENSOR = 8;
const int S[8] = {A4, A5, A6, A7, A0, A1, A2, A3};

int SENSOR_BACA_HITAM[8];

const int NILAI_THRESHOLD = 600;
const int DUTY_60_PERCENT = 220; // 255 * 0.6 = 153
const int DUTY_50_PERCENT = 150; // 255 * 0.5 = 128
const int DUTY_40_PERCENT = 122; // 255 * 0.4 = 102
const int DUTY_20_PERCENT = 100; // 255 * 0.2 = 51
const int DUTY_0_PERCENT = 0;    // 255 * 0.0 = 0

// ================= PROTOTIPE FUNGSI =================
void setMotors(int leftSpeed, int rightSpeed);

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  pinMode(PIN_LED_KIRI, OUTPUT);
  pinMode(PIN_LED_TENGAH, OUTPUT);
  pinMode(PIN_LED_KANAN, OUTPUT);

  pinMode(PIN_PWM_MOTOR_KANAN_MAJU, OUTPUT);
  pinMode(PIN_PWM_MOTOR_KIRI_MAJU, OUTPUT);

  setMotors(0, 0);
}

// ================= LOOP UTAMA =================
void loop() {  
  // ===== BACA SENSOR =====
  for (int i = 0; i < 8; i++) {
    int nilai_baca_sensor = analogRead(S[i]);

    if (nilai_baca_sensor < NILAI_THRESHOLD)
      SENSOR_BACA_HITAM[i] = 1;
    else
      SENSOR_BACA_HITAM[i] = 0;
  }

  // ===== BUAT POLA =====
  String pola = "";
  for (int i = 0; i < 8; i++) {
    Serial.print(SENSOR_BACA_HITAM[i]);
    pola += SENSOR_BACA_HITAM[i];
  }

  // === LOGIKA KONTROL SENSOR ===
  if (s4_black && s5_black) {
    setMotors(DUTY_60_PERCENT, DUTY_60_PERCENT);
    Serial.println("MAJU     - Sensor S4 dan S5 hitam (60% kedua motor)");
  }
  // Kalau tidak maka Prioritas 2: Sensor 1 dan 2 mendeteksi hitam → Duty cycle
  // 50% Motor Kiri, 0% Motor Kanan
  else if (s1_black && s2_black) {
    setMotors(DUTY_50_PERCENT, DUTY_0_PERCENT);
    Serial.println(
        "BELOK KANAN KUAT - Sensor S1 dan S2 hitam (50% kiri, 0% kanan)");
  }
  // Kalau tidak maka Prioritas 3: Sensor 2 dan 3 mendeteksi hitam → Duty cycle
  // 50% Motor Kiri, 20% Motor Kanan
  else if (s2_black && s3_black) {
    setMotors(DUTY_50_PERCENT, DUTY_20_PERCENT);
    Serial.println(
        "BELOK KANAN SEDANG - Sensor S2 dan S3 hitam (50% kiri, 20% kanan)");
  }
  // Kalau tidak maka Prioritas 4: Sensor 3 dan 4 mendeteksi hitam → Duty cycle
  // 50% Motor Kiri, 40% Motor Kanan
  else if (s3_black && s4_black) {
    setMotors(DUTY_50_PERCENT, DUTY_40_PERCENT);
    Serial.println(
        "BELOK KANAN LEMAH - Sensor S3 dan S4 hitam (50% kiri, 40% kanan)");
  }
  // Kalau tidak maka Prioritas 5: Sensor 7 dan 8 mendeteksi hitam → Duty cycle
  // 50% Motor Kanan, 0% Motor Kiri
  else if (s7_black && s8_black) {
    setMotors(DUTY_0_PERCENT, DUTY_50_PERCENT);
    Serial.println(
        "BELOK KIRI KUAT - Sensor S7 dan S8 hitam (0% kiri, 50% kanan)");
  }
  // Kalau tidak maka Prioritas 6: Sensor 6 dan 7 mendeteksi hitam → Duty cycle
  // 50% Motor Kanan, 20% Motor Kiri
  else if (s6_black && s7_black) {
    setMotors(DUTY_20_PERCENT, DUTY_50_PERCENT);
    Serial.println(
        "BELOK KIRI SEDANG - Sensor S6 dan S7 hitam (20% kiri, 50% kanan)");
  }
  // Kalau tidak maka Prioritas 7: Sensor 5 dan 6 mendeteksi hitam → Duty cycle
  // 50% Motor Kanan, 40% Motor Kiri
  else if (s5_black && s6_black) {
    setMotors(DUTY_40_PERCENT, DUTY_50_PERCENT);
    Serial.println(
        "BELOK KIRI LEMAH - Sensor S5 dan S6 hitam (40% kiri, 50% kanan)");
  }
  // Kalau tidak ada kondisi yang terpenuhi → Cek kondisi lain atau berhenti
  else {
    // Kondisi tambahan untuk garis lebar atau posisi tidak jelas
    if (s4_black || s5_black) {
      // Jika salah satu sensor tengah hitam, coba pertahankan posisi
      if (s4_black && !s5_black) {
        setMotors(DUTY_40_PERCENT, DUTY_60_PERCENT);
        Serial.println("KOREKSI KANAN - S4 hitam (40% kiri, 60% kanan)");
      } else if (!s4_black && s5_black) {
        setMotors(DUTY_60_PERCENT, DUTY_40_PERCENT);
        Serial.println("KOREKSI KIRI - S5 hitam (60% kiri, 40% kanan)");
      } else {
        // Default: berhenti
        setMotors(DUTY_0_PERCENT, DUTY_0_PERCENT);
        Serial.println("BERHENTI - Pola sensor tidak jelas");
      }
    } else {
      // Tidak ada sensor tengah yang hitam → berhenti
      setMotors(DUTY_0_PERCENT, DUTY_0_PERCENT);
      Serial.println("BERHENTI - Garis hilang");
    }
  }

  delay(50); // Penundaan kecil untuk stabilitas
}

// ================= FUNGSI KONTROL MOTOR =================
void setMotors(int leftSpeed, int rightSpeed) {
  // Membatasi kecepatan ke rentang PWM yang valid (0-255)
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  // Mengatur kecepatan motor (hanya maju sesuai permintaan)
  analogWrite(PIN_PWM_MOTOR_KIRI_MAJU, leftSpeed);
  analogWrite(PIN_PWM_MOTOR_KANAN_MAJU, rightSpeed);
}