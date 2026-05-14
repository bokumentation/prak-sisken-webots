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
  // Steps:
  // 0: start first 90° right turn
  // 1: wait turn done
  // 2: drive forward until NOT black (leave finish box)
  // 3: drive forward until black again (find start box)
  // 4: start second 90° right turn
  // 5: wait turn done
  // 6: drive forward to exit start box (clear)
  // In all movement steps we use simple forward/stop because no line is followed.

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