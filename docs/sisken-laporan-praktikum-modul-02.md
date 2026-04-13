Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<h1>
<center>

LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 02: Sistem Kendali PWM

</center>
</h1>

---

## 1. JUDUL PRAKTIKUM

Sistem Kendali PWM (_Pulse Width Modulation_)

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah:
1. Mahasiswa dapat memahami fungsi dan cara kerja PWM pada motor DC
2. Mahasiswa dapat membuat program sistem kendali berbasis PWM pada robot line follower pada arena yang telah dibuat.

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

### 4.1. Pengertian PWM (Pulse Width Modulation)

Pulse Width Modulation (PWM) adalah sebuah metode memanipulasi lebar sinyal yang dinyatakan dengan pulsa dalam satu periode, untuk mendapatkan tegangan rata-rata yang berbeda. Bebarapa contoh aplikasi PWM adalah pengontrolan daya atau tegangan yang masuk ke beban, regulator tegangan, pengendalian kecepatan motor, dan lain-lain.

Sinyal PWM pada umumnya memiliki amplitudo dan frekuensi dasar yang tetap tetapi memiliki lebar pulsa yang bervariasi. Lebar pulsa PWM berbanding lurus dengan amplitude sinyal asli yang belum termodulasi. Oleh karena itu, sinyal PWM memiliki frekuensi gelombang yang tetap namun duty cycle bervariasi antara 0% hingga 100%.

### 4.2. Sistem Kendali PWM pada Robot Line Follower menggunakan Arduino

Seperti yang telah dibahas pada praktikum modul 1, sistem kendali on/off tidak dapat digunakan untuk mengendalikan kecepatan motor pada robot line follower. Oleh karena itu, dibutuhkan PWM untuk mengatur kecepatan motor. Dengan menggunakan PWM pengaturan kecepatan motor dapat diubah dengan memvariasikan nilai besarnya duty cycle pulsa. Pulsa yang yang nilai duty cycle-nya divariasikan inilah yang menentukan kecepatan
motor. Besarnya amplitudo dan frekuensi pulsa adalah tetap, sedangkan besarnya duty cycle berubah-ubah sesuai dengan kecepatan yang diinginkan. Semakin besar duty cycle maka semakin cepat pula kecepatan motor, dan sebaliknya semakin kecil duty cycle maka semakin rendah pula kecepatan motor. Sebagai contoh bentuk pulsa yang dikirimkan adalah seperti pada Gambar 6. Pulsa kotak ini memiliki duty cycle dengan lebar 50%.

Pada rangkaian elektronika digital, setiap perubahan PWM dipengaruhi oleh resolusi PWM itu sendiri. Resolusi adalah jumlah variasi perubahan nilai dalam PWM tersebut. Misalnya suatu PWM pada Arduino memiliki resolusi 8 bit, berarti PWM ini memiliki variasi perubahan nilai sebanyak 256 variasi mulai dari 0 – 225 perubahan nilai yang mewakili duty cycle 0% – 100% dari keluaran PWM tersebut. Sebagian kaki / pin Arduino telah mendukung fitur PWM. Pin Arduino Nano yang mendukung PWM ditandai dengan adanya tanda tilde (~) di depan angka pinnya, seperti 3, 5, 6, 9,10, dan 11. Frekuensi yang digunakan dalam Arduino untuk PWM adalah 500Hz (500 siklus dalam 1 detik).

Untuk menggunakan PWM, kita bisa menggunakan fungsi analogWrite(). Nilai yang dapat dimasukkan pada fungsi tersebut yaitu antara 0 hingga 255. Nilai 0 berarti pulsa yang diberikan untuk setiap siklus selalu 0 volt, sedangkan nilai 255 berarti pulsa yang diberikan selalu bernilai 5 volt.

Semakin besar duty cycle pulsa kotak, maka semakin lama pula posisi logika HIGH. Jika misalnya motor diatur agar berjalan dengan duty cycle 50% (analogWrite 127), ketika diberi logika HIGH maka motor akan berada pada kondisi “nyala-mati-nyala-mati” sesuai dengan bentuk pulsa tersebut. Semakin lama motor berada pada kondisi “menyala” maka semakin cepat pula kecepatan motor tersebut. Motor akan berputar dengan kecepatan
maksimum apabila mendapat pulsa dengan duty cycle 100% (analogWrite  55). Dengan kata lain motor mendapat logika high terus menerus.

Pada praktikum ini PWM akan digunakan pada beberapa kondisi. Ketika sensor di bagian tengah mendeteksi garis hitam, maka robot bergerak maju dengan duty cycle 60%.

## 5. PROSEDUR PRAKTIKUM

### 5.1. KEGIATAN PRAKTIKUM

Membuat sebuah aplikasi sistem kendali PWM dengan ketentuan sebagai berikut:

1. Menggunakan seluruh sensor photodiode sebagai input sensor dengan contoh urutan sensor seperti pada Gambar. Kemudian, hasil pembacaan sensor akan mempengaruhi duty cycle pada motor kiri dan kanan dengan ketentuan sebagai berikut.
    - Sensor 4 dan 5 mendeteksi hitam, sisanya putih -> Duty cycle 60% pada kedua motor (kedua motor maju).
    - Sensor 1 dan 2 mendeteksi hitam, sisanya putih -> Duty cycle 50% Motor Kiri, 0% Motor Kanan.
    - Sensor 2 dan 3 mendeteksi hitam, sisanya putih -> Duty cycle 50% Motor Kiri, 20% Motor Kanan.
    - Sensor 3 dan 4 mendeteksi hitam, sisanya putih -> Duty cycle 50% Motor Kiri, 40% Motor Kanan.
    - Sensor 7 dan 8 mendeteksi hitam, sisanya putih -> Duty cycle 50% Motor Kanan, 0% Motor Kiri.
    - Sensor 6 dan 7 mendeteksi hitam, sisanya putih -> Duty cycle 50% Motor Kanan, 20% Motor Kiri.
    - Sensor 5 dan 6 mendeteksi hitam, sisanya putih -> Duty cycle 50% Motor Kanan, 40% Motor Kiri.
3. Membuat sub program yang dapat menyimpan kondisi terakhir dari pembacaan sensor dalam sebuah variabel/ EEPROM dan jika hanya terdapat 1 (satu) buah nilai sensor, program harus dapat mengeksekusi kondisi terakhir yang telah disimpan pada variabel/EEPROM.
4. Jika seluruh sensor mendeteksi nilai putih maka seluruh motor harus berhenti.

### 5.1. PERTANYAAN PRAKTIKUM

1. Jelaskan fungsi dari pengubahan berbagai duty cycle pada kondisi-kondisi di atas!
    Jawab: 

### 5.2. KODE PROGRAM

```cpp
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

const int DUTY_60_PERCENT = 220; 
const int DUTY_50_PERCENT = 150; 
const int DUTY_40_PERCENT = 122; 
const int DUTY_20_PERCENT = 100; 
const int DUTY_0_PERCENT = 0;    

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

  delay(50); 
}

void setMotors(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);
  analogWrite(PIN_PWM_MOTOR_KIRI_MAJU, leftSpeed);
  analogWrite(PIN_PWM_MOTOR_KANAN_MAJU, rightSpeed);
}
```

## 6. KESIMPULAN


## 7. LAMPIRAN
