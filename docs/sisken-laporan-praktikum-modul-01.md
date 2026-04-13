Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<h1>
<center>

LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 01

</center>
</h1>

---

## 1. JUDUL PRAKTIKUM

Pengenalan Sistem Kendali On/Off dan Sensor pada Robot Line Follower

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah:
1. Mahasiswa dapat memahami fungsi dan cara kerja dari sensor pada robot line follower 
2. Mahasiswa dapat membuat program sistem kendali on/off pada robot line follower pada arena yang telah dibuat.

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

Robot line follower adalah tipe robot beroda atau berkaki yang bergerak mengikuti garis, baik garis hitam atau garis putih. Pada dasarnya terdapat dua jenis line follower robot yaitu line follower hitam dan line follower putih.

### 4.1. Konsep Sensor dari line Follower

Sensor pada robot line follower pada dasarnya memanfaatnya prinsip pemantulan cahaya LED pada permukaan lintasan yang berwarna putih atau hitam yang kemudian ditangkap oleh sensor photodioda. Ketika cahaya LED jatuh pada permukaan putih, cahaya dipantulkan hampir 100 persen tetapi apabila cahaya jatuh pada permukaan hitam maka cahaya diserap. Pada Gambar 2 dapat dilihat ilustrasi penggambaran cara kerja sensor pada robot line follower.

### 4.2. Driver Motor Bagian Driver Motor terdiri dari Motor Driver dan 2 motor DC

Driver motor yang digunakan untuk motor mengemudi karena Arduino tidak menyediakan cukup tegangan dan arus ke motor. Jadi kita tambahkan rangkaian driver motor untuk mendapatkan cukup tegangan dan arus untuk motor. Arduino mengirimkan perintah ke driver motor ini dan kemudian menggerakkan motor.

### 4.3. Sistem Kendali On/Off pada Robot Line Follower menggunakan Arduino

Sistem kendali ON-OFF berfungsi untuk menghasilkan sistem kontrol yang tetap dan bersifat diskrit (discrete). Salah satu contoh sistem kendali on/off adalah pada saat menyalakan dan mematikan sebuah motor listrik pada sebuah robot line follower. Sistem kendali ini hanya memiliki dua perintah untuk motor listrik tersebut, yaitu perintah start (1) dan stop (0) saja. Sedangkan pada sisi motor, hanya terdapat dua buah feedback yaitu motor berputar dan motor berhenti berputar.

Pada sistem kendali on/off di robot line follower, posisi pemasangan sensor, aktuator dan struktur mekanik robot sangat berperan dalam kestabilan gerak. Hal ini disebabkan proses sensing hanya bergantung pada dua buah sensor yang akan mendeteksi garis hitam atau putih. Hasil pembacaan sensor ini akan dikirimkan ke Arduino. Kemudian Arduino akan mengirimkan sinyal ke modul driver motor untuk mengaktifkan kedua motor sesuai output sensor. 

## 5. PROSEDUR PRAKTIKUM

### 5.1. KEGIATAN PRAKTIKUM

1. Membuat program pada Arduino yang dapat mengaktifkan sensor photodiode pada robot line follower kemudian menampilkan hasil pembacaan kedelapan sensor tersebut di Serial Monitor.
2. Membuat sebuah aplikasi sistem kendali on/off pada robot dengan ketentuan sebagai berikut.
    - Menggunakan 8 LED dan 8 sensor photodiode sebagai input sensor.
    - Flowchart program sistem kendali on/off.
3. Ujicoba program pada robot di lintasan dan perlihatkan pada asisten

### 5.1. PERTANYAAN PRAKTIKUM

1. Apa yang terjadi ketika sensor dan LED dihadapkan pada permukaan yang berwarna hitam dan putih?
    Jawab:
2. Apabila sudah selesai dilaksanakan, perlihatkan hasil pada asisten.
    Jawab:

### 5.2. KODE PROGRAM

```cpp
const int pin_led_left = 7;
const int pin_led_middle = 13;
const int pin_led_right = 8;

// PWM output for Motor
const int pin_pwm_right_motor_forward = 9;
const int pin_pwm_left_motor_forward = 6;

// Sensor ANALOG
const int pin_sensor_sayap_kanan_s1 = A4;  
const int pin_sensor_sayap_kanan_s2 = A5;  
const int pin_sensor_sayap_kanan_s3 = A6;  
const int pin_sensor_sayap_kanan_s4 = A7;  
const int pin_sensor_sayap_kiri_s5 = A0;  
const int pin_sensor_sayap_kiri_s6 = A1;  
const int pin_sensor_sayap_kiri_s7 = A2;  
const int pin_sensor_sayap_kiri_s8 = A3;  

const int THRESHOLD = 500; 
const int MOTOR_SPEED = 255;  
const int TURN_SPEED = 125;   

void setMotors(int leftSpeed, int rightSpeed);

void setup() {
  Serial.begin(9600);
  pinMode(pin_led_left, OUTPUT);
  pinMode(pin_led_middle, OUTPUT);
  pinMode(pin_led_right, OUTPUT);
  pinMode(pin_pwm_right_motor_forward, OUTPUT);
  pinMode(pin_pwm_left_motor_forward, OUTPUT);
  setMotors(0, 0);

  Serial.println("=== LINE FOLLOWER WITH 2 SENSORS (S4 & S5) ===");
  Serial.println("System Ready: Place sensors over black line.");
  Serial.println("S4 (Right): A7, S5 (Left): A0");
  Serial.println("Threshold: " + String(THRESHOLD));
  Serial.println("==============================================");
}

// ================= MAIN LOOP =================
void loop() {
  int s1 = analogRead(pin_sensor_sayap_kanan_s1);
  int s2 = analogRead(pin_sensor_sayap_kanan_s2);
  int s3 = analogRead(pin_sensor_sayap_kanan_s3);
  int s4 = analogRead(pin_sensor_sayap_kanan_s4);  
  int s5 = analogRead(pin_sensor_sayap_kiri_s5);
  int s6 = analogRead(pin_sensor_sayap_kiri_s6);
  int s7 = analogRead(pin_sensor_sayap_kiri_s7);
  int s8 = analogRead(pin_sensor_sayap_kiri_s8);
  
  bool s1_black = s1 > THRESHOLD;  
  bool s2_black = s2 > THRESHOLD;  
  bool s3_black = s3 > THRESHOLD;  
  bool s4_black = s4 > THRESHOLD;  
  bool s5_black = s5 > THRESHOLD;  
  bool s6_black = s6 > THRESHOLD;  
  bool s7_black = s7 > THRESHOLD;  
  bool s8_black = s8 > THRESHOLD;  

  // Indikator LED
  digitalWrite(pin_led_left, (s6_black || s5_black) ? HIGH : LOW);      
  digitalWrite(pin_led_right, (s3_black || s4_black) ? HIGH : LOW); 
  digitalWrite(pin_led_middle, (s4_black && s5_black) ? HIGH : LOW);    
  
  // Motor control logic based on sensor readings
  if (s4_black && s5_black) {
    // Kedua sensor S4 dan S5 detek hitam: Jalan maju
    setMotors(MOTOR_SPEED, MOTOR_SPEED);
    Serial.println("STRAIGHT  - Both sensors on line");
  }
  else if (s4_black && !s5_black) {
    // Jika sensor kanan (S4) detek hitam - Belok Kiri
    setMotors(0, TURN_SPEED);  // Right motor forward, left motor stop
    Serial.println("TURN LEFT - Right sensor on line");
  }
  else if (!s4_black && s5_black) {
    // Jika sensor kiri (S5) detek hitam - Belok Kanan
    setMotors(TURN_SPEED, 0);  // Left motor forward, right motor stop
    Serial.println("TURN RIGHT - Left sensor on line");
  }
  else {
    // Jika sensors S4 dan S5 detek putih: stop
    setMotors(0, 0);
    Serial.println("STOP      - Line lost");
  }
  
  Serial.print("S4 (Right): ");
  Serial.print(s4);
  Serial.print(" | S5 (Left): ");
  Serial.print(s5);
  Serial.print(" | S4_black: ");
  Serial.print(s4_black ? "YES" : "NO");
  Serial.print(" | S5_black: ");
  Serial.println(s5_black ? "YES" : "NO");
  
  delay(10);  
}

void setMotors(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);
  analogWrite(pin_pwm_left_motor_forward, leftSpeed);
  analogWrite(pin_pwm_right_motor_forward, rightSpeed);
}
```

## 6. KESIMPULAN


## 7. LAMPIRAN
