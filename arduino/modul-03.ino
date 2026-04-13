// =====================================================
// PROGRAM LINE FOLLOWER 8 SENSOR - MODUL 3 PRAKTIKUM
// REV: 10 APRIL 2026
// NOTE: menampilkan pola bentuk string
// =====================================================

// ================= PIN SENSOR =================
// A4=S1, A5=S2, A6=S3, A7=S4, A0=S5, A1=S6, A2=S7, A3=S8
int S[8] = {A4, A5, A6, A7, A0, A1, A2, A3};

int MOTOR_KIRI_MAJU = 6;
int MOTOR_KIRI_MUNDUR = 5;
int MOTOR_KANAN_MAJU = 9;
int MOTOR_KANAN_MUNDUR = 10;

int sensor[8];
int error = 0;
int lastError = 0;
const int THRESHOLD = 500;

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_KIRI_MAJU, OUTPUT);
  pinMode(MOTOR_KIRI_MUNDUR, OUTPUT);
  pinMode(MOTOR_KANAN_MAJU, OUTPUT);
  pinMode(MOTOR_KANAN_MUNDUR, OUTPUT);
}

void loop() {
  // Membaca Sensor
  for (int i = 0; i < 8; i++) {
    int nilai = analogRead(S[i]);
    if (nilai < THRESHOLD)
      sensor[i] = 1;
    else
      sensor[i] = 0;
  }

  // Membuat string untuk POLA
  String pola = "";
  for (int i = 0; i < 8; i++) {
    Serial.print(sensor[i]);
    pola += sensor[i];
  }

  int pwm_kiri = 0;
  int pwm_kanan = 0;

  // ===== LOGIKA =====
  if (pola == "10000000") {
    error = -6;
    pwm_kiri = 127;
    pwm_kanan = 0;
  } else if (pola == "11000000") {
    error = -5;
    pwm_kiri = 127;
    pwm_kanan = 13;
  } else if (pola == "01000000") {
    error = -4;
    pwm_kiri = 127;
    pwm_kanan = 25;
  } else if (pola == "01100000") {
    error = -3;
    pwm_kiri = 127;
    pwm_kanan = 38;
  } else if (pola == "00100000") {
    error = -2;
    pwm_kiri = 127;
    pwm_kanan = 51;
  } else if (pola == "00110000") {
    error = -1;
    pwm_kiri = 127;
    pwm_kanan = 64;
  }
  else if (pola == "00010000") { 
    error = 0;
    pwm_kiri = 127;                 
    pwm_kanan = 76;                 
  } else if (pola == "00011000") { 
    error = 0;
    pwm_kiri = 153;                 
    pwm_kanan = 153;                
  } else if (pola == "00001000") { 
    error = 0;
    pwm_kiri = 76;   
    pwm_kanan = 127; 
  }
  else if (pola == "00001100") {
    error = 2;
    pwm_kiri = 51;
    pwm_kanan = 127;
  } else if (pola == "00000100") {
    error = 3;
    pwm_kiri = 38;
    pwm_kanan = 127;
  } else if (pola == "00000011") {
    error = 4;
    pwm_kiri = 25;
    pwm_kanan = 127;
  } else if (pola == "00000010") {
    error = 5;
    pwm_kiri = 13;
    pwm_kanan = 127;
  } else if (pola == "00000001") {
    error = 6;
    pwm_kiri = 0;
    pwm_kanan = 127;
  }
  else {
    // menyimpan lastError jika tidak 
    // tidak sesuai pola
    error = lastError;
    pwm_kiri = 120;
    pwm_kanan = 120;
  }

  lastError = error;

  int nilai_persen_kiri = (pwm_kiri * 100) / 255;
  int nilai_persen_kanan = (pwm_kanan * 100) / 255;

  Serial.print(" | Error: ");
  Serial.print(error);
  Serial.print(" | Kiri: ");
  Serial.print(nilai_persen_kiri);
  Serial.print("%");
  Serial.print(" | Kanan: ");
  Serial.print(nilai_persen_kanan);
  Serial.println("%");

  analogWrite(MOTOR_KIRI_MAJU, pwm_kiri);
  analogWrite(MOTOR_KANAN_MAJU, pwm_kanan);
  digitalWrite(MOTOR_KIRI_MUNDUR, LOW);
  digitalWrite(MOTOR_KANAN_MUNDUR, LOW);

  delay(10);
}