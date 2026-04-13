Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---
<center>
<h1>
LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 04: Sistem Kendali PID Kasus P
</h1>
</center>

---

## 1. JUDUL PRAKTIKUM
Sistem Kendali PID Kasus P

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah:
1. Mahasiswa dapat memahami fungsi dan cara kerja PID pada motor DC
2. Mahasiswa dapat membuat program sistem kendali berbasis PID dengan error yang dihubungkan dengan konstanta proporsional

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

### 4.1. Sistem Kendali PID

Teknik kendali PID adalah pengendali yang merupakan gabungan antara aksi kendali proporsional ditambah aksi kendali integral ditambah aksi kendali derivatif/turunan (Ogata, 1996). PID merupakan kependekan dari proportional integral derivative. Kombinasi ketiga jenis aksi kendali ini bertujuan untuk saling melengkapi kekurangan-kekurangan dari masing-masing aksi kendali. Untuk memudahkan dalam memahami konsep teknik kendali PID silakan menyermati diagram blok pengendali PID pada gambar 1 di bawah ini.

Dalam aksi kendali PID, ada beberapa parameter variabel (dapat diubah/berubah) yang dapat dimanipulasi untuk tujuan menghasilkan aksi kendali terbaik dalam aplikasinya. Cara manipulasi parameter ini sering dinamakan dengan Manipulated Variable (MV). Dalam notasi matematikanya dapat ditulis dengan MV(t) atau u(t).

Berikut persamaan matematik kendali PID.

$$
u(t) = K_p e(t) + K_i \int_0^t e(\tau) d\tau + K_d \frac{de(t)}{dt}
$$

Persamaan (2) dan (3) disubtitusikan ke dalam persamaan (1) makan akan menjadi:

Apabila kita terapkan transformasi Laplace pada persamaan (4) di atas, maka penulisannya adalah sebagai berikut:

$$
U(s) = \left(K_p + \frac{K_i}{s} + K_d s\right) E(s)
$$

Fungsi alih (transfer function) dari pengendali PID adalah:

$$
G_c(s) = \frac{U(s)}{E(s)} = K_p + \frac{K_i}{s} + K_d s
$$

### 4.2. Pengertian Sistem Kendali PID Kasus P (Proportional)

Aksi kendali proporsional (P) adalah aksi kendali yang memiliki karakter dapat mengurangi waktu naik (rise time), tetapi tidak menghilangkan kesalahan keadaan tunak (steady satate error).

Persamaan hubungan antara keluaran sistem u(t) dengan sinyal error e(t) pada aksi kendali proporsional adalah sebagai berikut.

$$
u(t) = K_p e(t)
$$

Sedangkan persamaan sinyal error -nya adalah:

$$
e(t) = SP - PV(t)
$$

Pada praktikum ini nilai PV (process value) adalah error dengan setpoint (SP) sensor dianggap 0.

### 4.3. Aplikasi PID pada Robot Line Follower

Sistem kendali PID ini bertujuan untuk menentukan paramater aksi kendali Proportional, Integratif, Derivatif pada robot line follower. Proses ini dapat dilakukan dengan cara trial and error . Keunggulan cara ini plant tidak perlu diidentifikasi dan membuat model matematis plant. Hanya dengan cara mencoba memberikan konstanta P-I-D pada formula PID ehingga di peroleh hasil yang optimal, dengan mengacu pada karakteristik masing-masing kontrol P-I-D. 

Tujuan penggunaan sistem kendali PID adalah untuk mengolah suatu sinyal kesalahan atau error, nilai error tersebut diolah dengan formula PID untuk dijadikan suatu sinyal kendali atau sinyal kontrol yang akan diteruskan ke aktuator.

Dari blok diagram di atas dapat dijelaskan sebagai berikut:
1. SP = Set point, suatu parameter nilai acuan atau nilai yang  inginkan.
2. PV = Present Value, nilai bobot pembacaan sensor saat itu atau variabel terukur yang di umpan balik oleh sensor (sinyal feedback dari sensor).
3. Error = nilai kesalahan, deviasi atau simpangan antar variabel terukur atau bobot sensor (PV) dengan nilai acuan (SP)

## 5. KEGIATAN PRAKTIKUM

1. Modifikasi program sistem kendali pada praktikum sebelumnya denganmenambahkan sebuah kondisi string berikut dengan menambahkan sebuah variabel dengan tipe int Kp dengan nilai awal 5, int moveControl dengan nilai awal 0, int error dengan nilai awal 0, int kecepatanMotorKanan dengan nilai awal 0, int kecepatanMotorKiri dengan nilai awal 0, int kecepatanSetPoint dengan nilai awal 150.

2. Program harus dapat mendeteksi perubahan nilai pada sensor dan mengirimkannya ke serial monitor dengan ketentuan sebagai berikut.
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
3. Kemudian tambahkan kode program dengan ketentuan sebagai berikut:
    - Simpan nilai error dengan variabel lastError.
    - Simpan nilai moveControl = perkalian Kp dan error
    - Simpan nilai kecepatanMotorKanan = kecepatanSetPoint dikurang (atau ditambah) moveControl
    - Simpan nilai kecepatanMotorKiri = kecepatanSetPoint dikurang (atau ditambah) moveControl
    - Kecepatan Motor Kiri dengan nilai analog sebesar kecepatanMotorKiri
    - Kecepatan Motor Kanan dengan nilai analog sebesar kecepatanMotorKanan

### 5.1. PERTANYAAN PRAKTIKUM

1. Jelaskan fungsi dari variabel yang telah ditambahkan pada program di atas terhadap mekanisme sistem kendali pada robot line follower!
    Jawab: Variabel-variabel tersebut memiliki fungsi spesifik dalam membangun algoritma kendali proporsional. Kp berfungsi sebagai pengali untuk menentukan sensitivitas respon motor terhadap penyimpangan, sementara error merepresentasikan jarak posisi robot saat ini dari _setpoint_. moveControl digunakan untuk menghitung besarnya nilai koreksi yang harus ditambahkan atau dikurangi pada kecepatanSetPoint agar menghasilkan kecepatanMotorKanan dan kecepatanMotorKiri yang sesuai untuk mengarahkan robot kembali ke tengah garis.
2. Jelaskan fungsi dari kode program di atas!
    Jawab: Kode program tersebut berfungsi untuk mengimplementasikan kendali diferensial pada penggerak robot berdasarkan input sensor biner. Program memetakan kombinasi pembacaan delapan sensor menjadi nilai numerik error, kemudian menghitung sinyal kendali proporsional untuk memanipulasi _duty cycle_ PWM pada masing-masing motor. Melalui fungsi _constrain_, program memastikan nilai output PWM tetap berada dalam batas operasional motor, sementara variabel _lastError_ menjamin robot tetap memiliki referensi arah saat kehilangan deteksi garis.

### 5.2. KODE PROGRAM

```cpp
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
```

## 6. KESIMPULAN

Berdasarkan hasil praktikum, dapat disimpulkan bahwa penerapan sistem kendali proporsional (P) mampu menghasilkan respon koreksi yang linear terhadap besarnya simpangan error pada robot _line follower_. Penggunaan konstanta $K_p$ yang dikalikan dengan nilai error menghasilkan nilai _moveControl_ untuk mengatur perbedaan kecepatan motor kiri dan kanan secara otomatis. Sistem kendali ini terbukti efektif dalam mempercepat waktu respon (_rise time_) robot saat kembali menuju _setpoint_ dibandingkan kendali PWM standar. Namun, nilai $K_p$ yang terlalu tinggi dapat menyebabkan osilasi pada robot, sehingga diperlukan proses _trial and error_ untuk mendapatkan nilai konstanta yang optimal. Navigasi robot menjadi lebih halus karena perubahan kecepatan terjadi secara gradual sesuai dengan posisi sensor terhadap garis.

## 7. LAMPIRAN
Link dokumentasi video:
https://drive.google.com/drive/folders/1Lgzs5CNa1o80Mx8uDIgMI3fhwKW2n_Xc?usp=sharing

Dokumentasi:
![[modul-04-01.jpg | 500]]

![[modul-04.jpg | 500]]