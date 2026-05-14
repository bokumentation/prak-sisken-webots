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