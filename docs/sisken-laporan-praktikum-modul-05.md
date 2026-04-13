Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<h1>
<center>

LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 05: Sistem Kendali PID Kasus P dan D

</center>
</h1>

---

## 1. JUDUL PRAKTIKUM

Sistem Kendali PID Kasus P dan D

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah :
1. Mahasiswa dapat memahami fungsi dan cara kerja PID pada motor DC
2. Mahasiswa dapat membuat program sistem kendali berbasis PID dengan error yang dihubungkan dengan konstanta proporsional dan derivatif

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

### 4.1. Sistem Kendali PD

Teknik kendali proporsional-derivatif (PD) adalah pengendali yang merupakan gabungan antara teknik kendali proporsional (P) dengan teknik kendali derivatif (D). Gambar 1 merupakan gambar diagram blok sistem kendali PD.

Persamaan hubungan antara keluaran sistem dengan sinyal error pada kombinasi aksi kendali proporsional-derivative adalah sebagai berikut.

**Persamaan 1 (Bentuk Standar):**
$$u(t) = K_p e(t) + K_d \frac{d}{dt} e(t)$$

**Persamaan 2 (Bentuk dengan Konstanta Waktu $T_d$):**
$$u(t) = K_p e(t) + K_p T_d \frac{de(t)}{dt}$$

Dalam penerapannya di software, kondisi ideal pada robot adalah bergerak maju lurus mengikuti garis, dengan kata lain error = 0 . Dari sini dapat diasumsikan bahwa Set Point (SP) / kondisi ideal adalah saat SP = 0. Nilai sensor yang dibaca oleh sensor disebut Process Variable (PV) / nilai aktual pembacaan. Menyimpangnya posisi robot dari garis disebut sebagai error (e), yang didapat dari e = SP – PV. Dengan mengetahui besar error, mikrokontroler dapat memberikan nilai PWM motor kiri dan kanan yang sesuai agar dapat menuju ke posisi ideal (SP = 0). Besarnya nilai PWM ini dapat diperoleh dengan menggunakan kontrol Proporsional (P), dimana P = e x Kp (Kp adalah konstanta proporsional yang nilainya diset sendiri dari hasil tuning/trial and error).

Jika pergerakan robot masih terlihat bergelombang, dapat ditambahkan parameter kontrol Derivatif (D). Kontrol D digunakan untuk mengukur seberapa cepat robot bergerak dari kiri ke kanan atau dari kanan ke kiri. Semakin cepat bergerak dari satu sisi ke sisi lainnya, maka
semakin besar nilai D. Konstanta D (K d) digunakan untuk menambah atau mengurangi imbas dari derivatif. Dengan mendapatkan nilai Kd yang tepat pergerakan sisi ke sisi yang bergelombang akibat dari kontrol proporsional dapat diminimalisasi. Dengan mendapatkan nilai K d yang tepat pergerakan sisi ke sisi yang bergelombang akibat dari kontrol proporsional bisa diminimalisasi. Nilai D didapat dari D = Kd/Ts x rate, dimana Ts adalah time sampling atau waktu cuplik dan rate = e(n) – e(n-1). Dalam program, nilai error (SP – PV) saat itu menjadi
nilai last_error, sehingga rate didapat dari error – last_error.

### **Tabel Parameter PID**

| Parameter | Rise Time | Overshoot | Settling Time | Steady State Error |
| :--- | :--- | :--- | :--- | :--- |
| **$K_p$** | Menurun | Meningkat | Perubahan kecil | Menurun |
| **$K_i$** | Menurun | Meningkat | Meningkat | Menurun signifikan |
| **$K_d$** | Sedikit turun | Sedikit turun | Sedikit turun | Tidak ada efek |


## 5. KEGIATAN PRAKTIKUM


Memodifikasi  program sistem kendali pada praktikum sebelumnya dengan menambahkan sebuah sub program untuk melakukan kalibrasi sensor secara otomatis (auto calibration). Kalibrasi ini berfungsi untuk menentukan nilai pembacaan sensor untuk warna hitam dan putih atau warna lainnya. Apabila push button yang terhubung dengan pin D2 (pin switch Left Down (Kiri bawah) pada Gambar 2) ditekan, eksekusi program berikut:
- Tambahkan variabel sensor[0] sampai sensor[7]
- Tambahkan variabel NilaiMaxSensor[0] sampai NilaiMaxSensor[7] dengan nilai awal 1023
- Tambahkan variabel NilaiMinSensor[0] sampai NilaiMinSensor[7] dengan nilai awal 0
- Tambahkan variabel NilaiTengahSensor[0] sampai NilaiTengahSensor[7]
- Baca kondisi sensor 1 sampai 8 dan simpan di variabel sensor[0] sampai sensor[7]. Untuk nilai i dari 0 hingga 7:
    - Jika nilai variabel sensor[i] > NilaiMinSensor[i] maka NilaiMinSensor[i]=sensor[i]
    - Jika nilai variabel sensor[i] < NilaiMaxSensor[i] maka NilaiMaxSensor[i]=sensor[i]
    - NilaiTengahSensor[i] = (NilaiMinSensor[i]+NilaiMaxSensor[i])/2
    - Saat proses kalibrasi dilakukan, LED LEFT (Pin D7) dan LED RIGHT (Pin D8) akan berkedap-kedip (blinking) sebagai indikator bahwa proses kalibrasi sedang berjalan.
- Baca kondisi sensor 1 sampai 8 dan simpan di variabel sensor[0] sampai sensor[7]. Untuk nilai i dari 0 hingga 7 :
    - Jika nilai variabel sensor[i] > NilaiMinSensor[i] maka NilaiMinSensor[i]=sensor[i]
    - Jika nilai variabel sensor[i] < NilaiMaxSensor[i] maka NilaiMaxSensor[i]=sensor[i]
    - NilaiTengahSensor[i] = (NilaiMinSensor[i]+NilaiMaxSensor[i])/2
    - Saat proses kalibrasi dilakukan, LED LEFT (Pin D7) dan LED RIGHT (Pin D8) akan berkedap-kedip (blinking) sebagai indikator bahwa proses kalibrasi sedang berjalan.

Apabila proses kalibrasi sudah selesai dan tombol push button yang terhubung dengan pin D3 (pin switch Left Up/kiri atas pada Gambar 2) ditekan, maka eksekusi program berikut ini:

- Inisialisasi variabel dengan tipe int Kp dengan nilai awal 15, int Kd dengan nilai awal : 5, int moveControl dengan nilai awal 0, int error dengan nilai awal 0, int lastError dengan nilai awal 0. int kecepatanMotorKanan dengan nilai awal 0, int kecepatanMotorKiri dengan nilai awal 0, int kecepatanSetPoint dengan nilai awal 150 dan int rate (selisih error dengan lastError).
- Program harus dapat mendeteksi perubahan nilai pada sensor dan mengirimkannya ke serial monitor dengan ketentuan sebagai berikut.
    - Jika kondisi sensor "10000000", error = -7, print di serial monitor error = -7,
    - Jika kondisi sensor "11000000", error = -6, print di serial monitor error = -6,
    - Jika kondisi sensor "01000000", error = -5, print di serial monitor error = -5,
    - Jika kondisi sensor "01100000", error = -4, print di serial monitor error = -4,
    - Jika kondisi sensor "00100000", error = -3, print di serial monitor error = -3,
    - Jika kondisi sensor "00110000", error = -2, print di serial monitor error = -2,
    - Jika kondisi sensor "00010000", error = -1, print di serial monitor error = -1,
    - Jika kondisi sensor "00011000", error = 0, print di serial monitor error = 0,
    - Jika kondisi sensor "00001000", error = 1, print di serial monitor error = 1,
    - Jika kondisi sensor "00001100", error = 2, print di serial monitor error = 2,
    - Jika kondisi sensor "00000100", error = 3, print di serial monitor error = 3,
    - Jika kondisi sensor "00000110", error = 4, print di serial monitor error = 4,
    - Jika kondisi sensor "00000010", error = 5, print di serial monitor error = 5,
    - Jika kondisi sensor "00000011", error = 6, print di serial monitor error = 6,
    - Jika kondisi sensor "00000001", error = 7, print di serial monitor error = 7,

Kemudian menambahkan kode program dengan ketentuan sebagai berikut:
1. Simpan nilai error saat ini dengan variabel Error dan nilai error sebelumnya dengan variable lastError.
2. Simpan nilai selisih antara lastError dengan Error dalam variabel rate.
3. Simpan nilai moveControl = perkalian Kp dan Error ditambah hasil perkalian Kd dan rate.
4. Simpan nilai kecepatanMotorKanan = kecepatanSetPoint dikurang moveControl.
5. Simpan nilai kecepatanMotorKiri = kecepatanSetPoint ditambah moveControl.
6. Kecepatan Motor Kiri dengan nilai analog sebesar kecepatanMotorKiri.
7. Kecepatan Motor Kanan dengan nilai analog sebesar kecepatanMotorKanan.


### 5.1. PERTANYAAN PRAKTIKUM
1. Jelaskan fungsi dari variabel yang telah ditambahkan pada program di atas terhadap mekanisme sistem kendali pada robot line follower!
    Jawab:
2. Jelaskan fungsi dari Kp dan Kd pada kode program di atas!
    Jawab:

### 5.2. KODE PROGRAM

```cpp
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
```

## 6. KESIMPULAN




## 7. LAMPIRAN
