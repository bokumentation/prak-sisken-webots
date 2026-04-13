Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<h1>
<center>

LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 03: Sistem Kendali PWM (Pulse Width Modulation) dengan Error Detection Memory

</center>
</h1>

---

## 1. JUDUL PRAKTIKUM

Sistem Kendali PWM (Pulse Width Modulation) dengan Error Detection Memory

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah:
1. Mahasiswa dapat memahami fungsi dan cara kerja PWM pada motor DC
2. Mahasiswa dapat membuat program sistem kendali berbasis PWM dengan error detection memory pada robot line follower untuk menyimpan kondisi error terakhir (last error condition).

## 3. ERALATAN DAN BAHAN

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

Sistem kendali berbasis Pulse Width Modulation (PWM) pada robot line follower secara umum hanya dapat digunakan untuk mengendalikan kecepatan motor kiri dan kanan. Akan tetapi, agar robot dapat selalu dapat mengikuti garis hitam dibutuhkan suatu algoritma atau mekanisme yang dapat menanggulangi keadaan error. Kondisi tujuan (goal condition) yang menjadi target dari suatu sistem kendali disebut dengan setpoint.

Nilai setpoint didapatkan ketika kedua sensor di bagian tengah mendeteksi garis hitam. Kemampuan robot untuk memposisikan diri ke posisi setpoint dipengaruhi oleh kemampuan sistem mendeteksi kondisi error yang merupakan selisih antara setpoint dengan kondisi pembacaan sensor saat ini

## 5. KEGIATAN PRAKTIKUM

1. Modifikasi program sistem kendali PWM pada praktikum sebelumnya dengan menambahkan sebuah kondisi string berikut dengan nama stringkondisi.Variabel stringkondisi akan memiliki nilai awal "00000000" dengan bit paling kiri adalah kondisi sensor no. 1 (S1) dan bit paling kanan kondisi sensor no. 8 (S8). Program harus dapat mendeteksi perubahan ilai pada sensor dan mengirimkannya ke serial monitor dengan ketentuan sebagai berikut.
    - Jika kondisi sensor "10000000", print di serial monitor error = -6 motor kiri 50% motor kanan 0%.
    - Jika kondisi sensor "11000000", print di serial monitor error = -5,motor kiri 50% motor kanan 5%.
    - Jika kondisi sensor "01000000", print di serial monitor error = -4,motor kiri 50% motor kanan 10%.
    - Jika kondisi sensor "01100000", print di serial monitor error = -3, motor kiri 50% motor kanan 15%.
    - Jika kondisi sensor "00100000", print di serial monitor error = -2, motor kiri 50% motor kanan 20%.
    - Jika kondisi sensor "00110000", print di serial monitor error = -1, motor kiri 50% motor kanan 25%.
    - Jika kondisi sensor "00010000", print di serial monitor error = 0, motor kiri 50% motor kanan 30%.
    - Jika kondisi sensor "00011000", print di serial monitor error = 0, motor kiri 60% motor kanan 60%.
    - Jika kondisi sensor "00001000", print di serial monitor error = 0, motor kiri 30% motor kanan 50%.
    - Jika kondisi sensor "00001100", print di serial monitor error = 1, motor kiri 25% motor kanan 50%.
    - Jika kondisi sensor "00000100", print di serial monitor error = 2, motor kiri 20% motor kanan 50%.
    - Jika kondisi sensor "00000110", print di serial monitor error = 3, motor kiri 15% motor kanan 50%.
    - Jika kondisi sensor "00000010", print di serial monitor error = 4, motor kiri 10% motor kanan 50%.
    - Jika kondisi sensor "00000011", print di serial monitor error = 5, motor kiri 5% motor kanan 50%.
    - Jika kondisi sensor "00000001", print di serial monitor error = 6, motor kiri 0% motor kanan 50%.

### 6. PERTANYAAN PRAKTIKUM

1. Jelaskan fungsi dari stringkondisi terhadap mekanisme sistem kendali pada robot line follower!
    Jawab:

### 7. KODE PROGRAM

```cpp
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
```

## 8. KESIMPULAN


## 9. LAMPIRAN
