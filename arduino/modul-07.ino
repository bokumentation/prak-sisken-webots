#include <EEPROM.h>

// ================= EEPROM ==============
const int ADDR_SIGNATURE = 0;
const int ADDR_THRESHOLD = 1;
const int ADDR_KP = 17;
const int ADDR_KD = 21;
const int ADDR_KI = 25;

const int btnLeftDown  = 2;
const int btnLeftUp    = 3;
const int btnRightUp   = 11;
const int btnRightDown = 12;

const int ledLeft = 7;
const int ledRight = 8;

const int motorKiri = 9;
const int motorKanan = 6;

const int sensorPin[8] = {A4,A5,A6,A7,A0,A1,A2,A3};

int threshold[8];

// konstanta PID hasil tuning
float Kp = 15;
float Ki = 2;
float Kd = 5;

// menyimpan error dari iterasi sebelumnya untuk kalkulasi derivative
int LastError = 0;
// akumulasi error untuk komponen integral
float IntegralError = 0;
// state machine: 0=idle, 1=kalibrasi, 2=berjalan, 3-8=tuning parameter
int systemState = 0;

unsigned long now = 0;
unsigned long lastTime = 0;
// selisih waktu antar iterasi kontrol dalam detik
float timeChange = 0;

unsigned long lastBlink = 0;
bool ledState = false;

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

  for(int i=0;i<8;i++) pinMode(sensorPin[i], INPUT);

  bacaEEPROM();

  lastTime = millis();

  Serial.println("LeftDown=Kalibrasi | LeftUp=Start");
}

void loop() {

  bool leftDown  = digitalRead(btnLeftDown)==LOW;
  bool leftUp    = digitalRead(btnLeftUp)==LOW;
  bool rightUp   = digitalRead(btnRightUp)==LOW;
  bool rightDown = digitalRead(btnRightDown)==LOW;

  switch(systemState){

    case 0:
      if(leftDown){
        systemState = 1;
        delay(300);
      }

      if(leftUp){
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

      // STOP
      if(leftDown && leftUp){
        stopMotor();
        systemState = 0;
        Serial.println(">> STOP <<");
        delay(300);
        break;
      }

      if(leftUp && rightUp){ // depan + depan
        systemState = 7;
        break;
      }

      if(leftDown && rightDown){ // belakang + belakang
        systemState = 8;
        break;
      }

      if(leftDown){ systemState = 3; break; }
      if(leftUp){ systemState = 4; break; }
      if(rightUp){ systemState = 5; break; }
      if(rightDown){ systemState = 6; break; }

    break;

    case 3:
      Kp -= 1; if(Kp < 0) Kp = 0;
      Serial.print("Kp: "); Serial.println(Kp);
      simpanEEPROM();
      delay(300);
      systemState = 2;
    break;

    case 4:
      Kp += 1;
      Serial.print("Kp: "); Serial.println(Kp);
      simpanEEPROM();
      delay(300);
      systemState = 2;
    break;

    case 5:
      Kd -= 1; if(Kd < 0) Kd = 0;
      Serial.print("Kd: "); Serial.println(Kd);
      simpanEEPROM();
      delay(300);
      systemState = 2;
    break;

    case 6:
      Kd += 1;
      Serial.print("Kd: "); Serial.println(Kd);
      simpanEEPROM();
      delay(300);
      systemState = 2;
    break;

    // ===== SOAL NO 2 =====
    case 7: // Ki naik
      digitalWrite(ledRight, HIGH);
      delay(500);
      digitalWrite(ledRight, LOW);

      Ki += 1;

      Serial.print("Ki: "); Serial.println(Ki);

      simpanEEPROM();
      delay(300);
      systemState = 2;
    break;

    case 8: // Ki turun
      digitalWrite(ledRight, HIGH);
      delay(500);
      digitalWrite(ledRight, LOW);

      Ki -= 1;
      if(Ki < 0) Ki = 0;

      Serial.print("Ki: "); Serial.println(Ki);

      simpanEEPROM();
      delay(300);
      systemState = 2;
    break;
  }
}

// mencari nilai minimum dan maksimum tiap sensor untuk menentukan threshold
void kalibrasiSensor(){
  int minVal[8], maxVal[8];

  for(int i=0;i<8;i++){
    minVal[i]=1023;
    maxVal[i]=0;
  }

  // sampling selama 100 iterasi
  for(int t=0;t<100;t++){
    blink();

    for(int i=0;i<8;i++){
      int val = analogRead(sensorPin[i]);
      if(val<minVal[i]) minVal[i]=val;
      if(val>maxVal[i]) maxVal[i]=val;
    }
    delay(50);
  }

  // threshold = titik tengah antara nilai minimum dan maksimum
  for(int i=0;i<8;i++){
    threshold[i]=(minVal[i]+maxVal[i])/2;
  }

  digitalWrite(ledLeft,LOW);
  digitalWrite(ledRight,LOW);
}

void blink(){
  if(millis()-lastBlink>100){
    lastBlink=millis();
    ledState=!ledState;
    digitalWrite(ledLeft,ledState);
    digitalWrite(ledRight,ledState);
  }
}

// membaca 8 sensor dan mengembalikan string biner (1=gelap, 0=terang)
String bacaSensor(){
  String s="";
  for(int i=0;i<8;i++){
    int val=analogRead(sensorPin[i]);
    s += (val>threshold[i])?"1":"0";
  }
  return s;
}

// memetakan pola pembacaan sensor ke nilai error posisi garis
// nilai positif berarti garis di kanan, negatif di kiri
int hitungError(String k){
  if(k=="10000000") return 7;
  else if(k=="11000000") return 6;
  else if(k=="01000000") return 5;
  else if(k=="01100000") return 4;
  else if(k=="00100000") return 3;
  else if(k=="00110000") return 2;
  else if(k=="00010000") return 1;
  else if(k=="00011000") return 0;
  else if(k=="00001000") return -1;
  else if(k=="00001100") return -2;
  else if(k=="00000100") return -3;
  else if(k=="00000110") return -4;
  else if(k=="00000010") return -5;
  else if(k=="00000011") return -6;
  else if(k=="00000001") return -7;
  else return LastError;
}

// menghitung selisih waktu antar iterasi untuk kalkulasi integral dan derivative
void hitungWaktu(){
  now = millis();
  timeChange = (now - lastTime) / 1000.0;
  lastTime = now;
}

// fungsi utama kontrol robot dengan kalkulasi PID
void jalanRobot(){

  hitungWaktu();

  String kondisi = bacaSensor();
  int error = hitungError(kondisi);

  // akumulasi integral dengan anti-windup
  IntegralError += error * timeChange;
  if(IntegralError > 1000) IntegralError = 1000;
  if(IntegralError < -1000) IntegralError = -1000;

  int dError = error - LastError;
  // output PID = proporsional + integral + derivative
  int output = (Kp * error) + (Ki * IntegralError) + (Kd * dError);

  // kecepatan dasar 200, output dikurangkan/tambahkan untuk koreksi arah
  int baseSpeed = 200;

  // batasi nilai PWM antara 0-255
  int kiri = constrain(baseSpeed + output,0,255);
  int kanan= constrain(baseSpeed - output,0,255);

  analogWrite(motorKiri,kiri);
  analogWrite(motorKanan,kanan);

  // simpan error sekarang untuk iterasi berikutnya
  LastError=error;

  Serial.print("State: "); Serial.print(systemState);
  Serial.print(" | Error: "); Serial.print(error);
  Serial.print(" | Integral: "); Serial.print(IntegralError,4);
  Serial.print(" | Time: "); Serial.print(timeChange,4);
  Serial.print(" | Kp: "); Serial.print(Kp);
  Serial.print(" | Ki: "); Serial.print(Ki);
  Serial.print(" | Kd: "); Serial.print(Kd);

  Serial.print(" | peka: ");
  for(int i=0;i<8;i++){
    Serial.print(threshold[i]);
    Serial.print(" ");
  }

  Serial.println();
}

void stopMotor(){
  analogWrite(motorKiri,0);
  analogWrite(motorKanan,0);
}

// menyimpan threshold dan parameter PID ke EEPROM agar tidak hilang saat mati
void simpanEEPROM(){
  EEPROM.write(ADDR_SIGNATURE,0xAA);

  for(int i=0;i<8;i++){
    EEPROM.write(ADDR_THRESHOLD+i*2, highByte(threshold[i]));
    EEPROM.write(ADDR_THRESHOLD+i*2+1, lowByte(threshold[i]));
  }

  byte *kp=(byte*)(void*)&Kp;
  byte *ki=(byte*)(void*)&Ki;
  byte *kd=(byte*)(void*)&Kd;

  for(int i=0;i<4;i++){
    EEPROM.write(ADDR_KP+i,kp[i]);
    EEPROM.write(ADDR_KI+i,ki[i]);
    EEPROM.write(ADDR_KD+i,kd[i]);
  }
}

// membaca parameter dari EEPROM saat startup
void bacaEEPROM(){
  if(EEPROM.read(ADDR_SIGNATURE)!=0xAA){
    Serial.println("EEPROM kosong");
    return;
  }

  for(int i=0;i<8;i++){
    int h=EEPROM.read(ADDR_THRESHOLD+i*2);
    int l=EEPROM.read(ADDR_THRESHOLD+i*2+1);
    threshold[i]=word(h,l);
  }

  byte kp[4], ki[4], kd[4];

  for(int i=0;i<4;i++){
    kp[i]=EEPROM.read(ADDR_KP+i);
    ki[i]=EEPROM.read(ADDR_KI+i);
    kd[i]=EEPROM.read(ADDR_KD+i);
  }

  memcpy(&Kp,kp,4);
  memcpy(&Ki,ki,4);
  memcpy(&Kd,kd,4);

  Serial.println("EEPROM Loaded");
}