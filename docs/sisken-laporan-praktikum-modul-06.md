Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<center> <h1>
LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 06: Kasus P dan D dengan EEPROM dan Push Button
</h1> </center>

---

## 1. JUDUL PRAKTIKUM
Sistem Kendali PID Kasus P dan D dengan EEPROM

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah :
1. Mahasiswa dapat memahami fungsi dan cara kerja PID pada motor DC.
2. Mahasiswa dapat membuat program untuk menggunakan EEPROM untuk penyimpanan data sensor yang telah dikalibrasi.
3. Mahasiswa dapat menggunakan peripheral berupa push button untuk menambah konstanta Kp dan Kd.

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

Jika pergerakan robot masih terlihat bergelombang, dapat ditambahkan parameter kontrol Derivatif (D). Kontrol D digunakan untuk mengukur seberapa cepat robot bergerak dari kiri ke kanan atau dari kanan ke kiri. Semakin cepat bergerak dari satu sisi ke sisi lainnya, maka semakin besar nilai D. Konstanta D (K d) digunakan untuk menambah atau mengurangi imbas dari derivatif. Dengan mendapatkan nilai Kd yang tepat pergerakan sisi ke sisi yang bergelombang akibat dari kontrol proporsional dapat diminimalisasi. Dengan mendapatkan nilai K d yang tepat pergerakan sisi ke sisi yang bergelombang akibat dari kontrol proporsional bisa diminimalisasi. Nilai D didapat dari D = Kd/Ts x rate, dimana Ts adalah time sampling atau waktu cuplik dan rate = e(n) – e(n-1). Dalam program, nilai error (SP – PV) saat itu menjadi nilai last_error, sehingga rate didapat dari error – last_error.

Agar konfigurasi atau hasil kalibrasi sensor tidak hilang ketika robot dimatikan atau kehilangan daya, EEPROM pada Arduino Nano dimanfaatkan untuk menyimpan data tersebut. Arduino Nano dengan mikrokontroler ATmega328 memiliki EEPROM dengan kapasitas 1024 byte. Kemudian untuk mempermudah user dalam memanfaatkan EEPROM untuk menyimpan dan menggunakan data, 4 buah push button yang disediakan pada robot digunakan

## 5. KEGIATAN PRAKTIKUM

Memodifikasi program pada praktikum sebelumnya dengan variable dan fungsi berikut ini :

- Tambahkan variabel integer dengan nama 'state' dengan nilai awal adalah 0.
- Tambahkan variabel integer dengan nama 'setting' dengan nilai awal adalah 0.
- Tambahkan variabel integer berupa array dengan nama 'peka' dari 0 hingga 7 dengan nilai awal adalah 500.
- Tambahkan variabel integer dengan nama 'Kp' dari 0 hingga 7 dengan nilai awal adalah 20.
- Tambahkan variabel integer dengan nama 'Kd' dari 0 hingga 7 dengan nilai awal adalah 5.
- Di dalam void setup tambahkan :
- Baca nilai eeprom yang terdapat di EEPROM dengan alamat 0 hingga 7 (dengan ketentuan alamat 0 adalah untuk data nilai tengah kalibrasi sensor 1, alamat 1 untuk nilai tengah kalibrasi sensor 2, dan seterusnya hingga sensor 8) dengan perintah EEPROM.read(0) hingga EEPROM.read(7), kemudian simpan didalam variabel peka[0] hingga peka[7] dengan mengkalikan dengan angka 4 (contoh : peka[0]=EEPROM.read[0]*4).
- Baca nilai eeprom yang terdapat di EEPROM dengan alamat 8 (ketentuan alamat 8 adalah data nilai Kp), kemudian simpan didalam variabel Kp.
- Baca nilai eeprom yang terdapat di EEPROM dengan alamat 9 (ketentuan alamat 9 adalah data nilai Kd), kemudian simpan didalam variabel Kp.
- Tampilkan data peka[ 0 ] hingga peka[7], Kp dan Kd yang berasal dari data EEPROM ke Serial Monitor.
- Di dalam void loop ditambahkan conditional sebagai berikut :
- Referensi posisi push button dapat dilihat pada
- Jika tombol kiri belakang ditekan dan setting bernilai 0 maka 'state' akan bernilai 1.
- Jika tombol kiri depan ditekan dan setting bernilai 0 maka 'state' akan bernilai 2 dan 'setting' akan bernilai 1.
- Jika tombol kiri belakang ditekan dan setting bernilai 1 maka 'state' akan bernilai 3.
- Jika tombol kiri depan ditekan dan setting bernilai 1 maka 'state' akan bernilai 4.
- Jika tombol kanan belakang ditekan dan setting bernilai 1 maka 'state' akan bernilai 5.
- Jika tombol kanan depan ditekan dan setting bernilai 1 maka 'state' akan bernilai 6.


Perubahan state tersebut akan mengaktifkan sub program berikut 
- Jika 'state' bernilai 1 maka proses auto calibration berlangsung.
- Jika 'state' bernilai 2 maka nilai terakhir variabel 'peka[0]' hingga 'peka[7]' disimpan ke dalam EEPROM dengan alamat 0 hingga 7 dan menjalankan perintah proses line follower dengan sistem kendali PID kasus P dan D pada praktikum sebelumnya dengan nilai peka[0] hingga peka[7], Kp dan Kd yang diperoleh dari data EEPROM dan atau berasal dari hasil autocallibration.
- Perintah EEPROM.write(alamat,value) dengan value yang dibagi dengan 4. Jelaskan mengapa value harus dikali dan dibagi 4! Contoh : EEPROM.write(0, peka[0]/4)
- Jika 'state' bernilai 3 maka LED kiri akan menyala dan setelah 1 detik LED kiri akan mati, mengurangi nilai Kp(contoh : Kp=Kp-1;), menyimpan nilai Kp kedalam EEPROM pada alamat 8.
- Jika 'state' bernilai 4 maka LED kiri akan menyala dan setelah 1 detik LED kiri akan mati, menambah nilai Kp(contoh : Kp=Kp+1;), menyimpan nilai Kp kedalam EEPROM pada alamat 8.
- Jika 'state' bernilai 5 maka LED kanan akan menyala dan setelah 1 detik LED kiri akan mati, mengurangi nilai Kd(contoh : Kd=Kd-1;), menyimpan nilai Kp kedalam EEPROM pada alamat 9
- Jika 'state' bernilai 6 maka LED kanan akan menyala dan setelah 1 detik LED kiri akan mati, menambah nilai Kd(contoh : Kd=Kd+1;), menyimpan nilai Kp kedalam EEPROM pada alamat 9

### 5.1. PERTANYAAN PRAKTIKUM

1. Variasikan nilai konstanta Kp dan Kd pada program kemudian ujicoba
pada lintasan dan amati hasil perubahan kedua konstanta tersebut pada robot
line follower! Apa yang menjadi kesimpulan dari hasil percobaan ini?
  Jawab: Variasi nilai konstanta Proportional (Kp) berpengaruh terhadap kecepatan respon robot dalam mengejar set point saat terjadi galat (error). Nilai Kp yang terlalu tinggi menyebabkan osilasi yang besar atau gerakan robot yang terlalu agresif, sedangkan nilai Kp yang terlalu rendah mengakibatkan respon robot menjadi lambat sehingga sulit mengikuti garis pada tikungan tajam. Penambahan konstanta Derivative (Kd) berfungsi sebagai peredam osilasi tersebut dengan memprediksi perubahan galat berdasarkan laju perubahannya terhadap waktu. Hasil percobaan menunjukkan bahwa kombinasi nilai Kp dan Kd yang tepat menghasilkan pergerakan robot yang lebih halus dan stabil. Penggunaan fitur penyimpanan pada EEPROM memungkinkan nilai konstanta hasil penalaan (tuning) tersimpan secara permanen sehingga robot dapat mempertahankan performa kendali meskipun catu daya diputus.

### 5.2. KODE PROGRAM

```cpp
#include <EEPROM.h>
const byte S[8] = {A4, A5, A6, A7, A0, A1, A2, A3};
const byte PIN_M_KANAN_MAJU = 9;
const byte PIN_M_KANAN_MUNDUR = 10;
const byte PIN_M_KIRI_MAJU = 6;
const byte PIN_M_KIRI_MUNDUR = 5;
const int PIN_LED_KIRI = 7;
const int PIN_LED_TENGAH = 13;
const int PIN_LED_KANAN = 8;
const int pin_btn_LB = 2;  // Kiri Belakang (D2)
const int pin_btn_LF = 3;  // Kiri Depan (D3)
const int pin_btn_RB = 12; // Kanan Belakang (D12)
const int pin_btn_RF = 11; // Kanan Depan (D11)
int sensor[8];
int NilaiTengahSensor[8];
int Kp, Kd;
int error = 0, lastError = 0;
int kecepatanSetPoint = 150;
bool isMoving = false;
bool bDetekPutih = false;
const int ADDR_KP = 100;
const int ADDR_KD = 102;
const int ADDR_SENS = 0; // Alamat 0-15 untuk 8 sensor (int = 2 bytes)

void setup() {
  Serial.begin(9600);

  // Inisialisasi Pin
  pinMode(PIN_M_KANAN_MAJU, OUTPUT);
  pinMode(PIN_M_KANAN_MUNDUR, OUTPUT);
  pinMode(PIN_M_KIRI_MAJU, OUTPUT);
  pinMode(PIN_M_KIRI_MUNDUR, OUTPUT);
  pinMode(PIN_LED_KIRI, OUTPUT);
  pinMode(PIN_LED_TENGAH, OUTPUT);
  pinMode(PIN_LED_KANAN, OUTPUT);

  pinMode(pin_btn_LB, INPUT_PULLUP);
  pinMode(pin_btn_LF, INPUT_PULLUP);
  pinMode(pin_btn_RB, INPUT_PULLUP);
  pinMode(pin_btn_RF, INPUT_PULLUP);

  stopMotors();

  // KASUS 2: Baca konfigurasi dari EEPROM saat startup
  loadSettings();
  Serial.println("Sistem Siap.");
}

void loop() {
  checkButtons();

  if (isMoving) {
    bDetekPutih = false;
    runLineFollower();
  }
}

// === KASUS 1: Kalibrasi & Simpan EEPROM ===
void doCalibration() {
  Serial.println("Proses Kalibrasi Otomatis...");
  int minVal[8], maxVal[8];
  for (int i = 0; i < 8; i++) {
    minVal[i] = 1023;
    maxVal[i] = 0;
  }

  unsigned long start = millis();
  while (millis() - start < 5000) { // Kalibrasi 5 detik
    for (int i = 0; i < 8; i++) {
      int val = analogRead(S[i]);
      if (val < minVal[i])
        minVal[i] = val;
      if (val > maxVal[i])
        maxVal[i] = val;
      NilaiTengahSensor[i] = (minVal[i] + maxVal[i]) / 2;
    }
    // LED Blinking
    digitalWrite(PIN_LED_KIRI, !digitalRead(PIN_LED_KIRI));
    digitalWrite(PIN_LED_KANAN, !digitalRead(PIN_LED_KANAN));
    delay(100);
  }

  // Simpan ke EEPROM
  for (int i = 0; i < 8; i++) {
    EEPROM.put(ADDR_SENS + (i * 2), NilaiTengahSensor[i]);
  }

  digitalWrite(PIN_LED_KIRI, LOW);
  digitalWrite(PIN_LED_KANAN, LOW);
  displaySettings();
}

// === KASUS 2, 3, 4: Logika Tombol & EEPROM ===
void checkButtons() {
  // Tombol Kiri Belakang (Kasus 1: Kalibrasi | Kasus 3: Kp--)
  if (digitalRead(pin_btn_LB) == LOW) {
    delay(200); // Debounce
    if (!isMoving) {
      doCalibration(); // State 1
    } else {
      Kp--; // State 3
      EEPROM.put(ADDR_KP, Kp);
      Serial.print("Kp berkurang: ");
      Serial.println(Kp);
    }
  }

  // Tombol Kiri Depan (Kasus 2: Start | Kasus 3: Kp++)
  if (digitalRead(pin_btn_LF) == LOW) {
    delay(200);
    if (!isMoving) {
      isMoving = true; // State 2
      Serial.println("Robot Jalan (Line Follower)");
    } else {
      Kp++; // State 4
      EEPROM.put(ADDR_KP, Kp);
      Serial.print("Kp bertambah: ");
      Serial.println(Kp);
    }
  }

  // Tombol Kanan Belakang (Kasus 4: Kd--)
  if (digitalRead(pin_btn_RB) == LOW && isMoving) {
    delay(200);
    Kd--; // State 5
    EEPROM.put(ADDR_KD, Kd);
    Serial.print("Kd berkurang: ");
    Serial.println(Kd);
  }

  // Tombol Kanan Depan (Kasus 4: Kd++) - Berdasarkan Instruksi Kiri Depan
  // (State 6) Catatan: Karena instruksi Kasus 4 menyebutkan "tombol kiri depan"
  // lagi untuk Kd++, saya asumsikan ini menggunakan tombol kanan depan agar
  // tidak bentrok dengan Kp++.
  if (digitalRead(pin_btn_RF) == LOW && isMoving) {
    delay(200);
    Kd++; // State 6
    EEPROM.put(ADDR_KD, Kd);
    Serial.print("Kd bertambah: ");
    Serial.println(Kd);
  }
}

void loadSettings() {
  for (int i = 0; i < 8; i++) {
    EEPROM.get(ADDR_SENS + (i * 2), NilaiTengahSensor[i]);
  }
  EEPROM.get(ADDR_KP, Kp);
  EEPROM.get(ADDR_KD, Kd);

  // Proteksi jika EEPROM kosong/nan
  if (Kp < 0 || Kp > 100)
    Kp = 15;
  if (Kd < 0 || Kd > 100)
    Kd = 5;
}

void displaySettings() {
  Serial.println("\n--- DATA EEPROM ---");
  for (int i = 0; i < 8; i++) {
    Serial.print("Peka[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(NilaiTengahSensor[i]);
  }
  Serial.print("Kp: ");
  Serial.println(Kp);
  Serial.print("Kd: ");
  Serial.println(Kd);
  Serial.println("-------------------\n");
}

void runLineFollower() {
  String pola = "";
  for (int i = 0; i < 8; i++) {
    if (analogRead(S[i]) > NilaiTengahSensor[i])
      pola += "1";
    else
      pola += "0";
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

  int rate = error - lastError;
  int moveControl = (Kp * error) + (Kd * rate);
  lastError = error;

  int pwmKiri = constrain(kecepatanSetPoint + moveControl, 0, 255);
  int pwmKanan = constrain(kecepatanSetPoint - moveControl, 0, 255);

  analogWrite(PIN_M_KIRI_MAJU, pwmKiri);
  analogWrite(PIN_M_KANAN_MAJU, pwmKanan);
  digitalWrite(PIN_M_KIRI_MUNDUR, LOW);
  digitalWrite(PIN_M_KANAN_MUNDUR, LOW);
}

void stopMotors() {
  analogWrite(PIN_M_KIRI_MAJU, 0);
  analogWrite(PIN_M_KANAN_MAJU, 0);
}
```

## 6. KESIMPULAN

Praktikum ini berhasil mengimplementasikan sistem kendali PD pada robot line follower dengan integrasi memori EEPROM dan antarmuka tombol tekan. Implementasi kontrol proporsional mampu mengoreksi posisi robot berdasarkan besaran galat, sementara kontrol derivatif berperan dalam meminimalkan osilasi sehingga pergerakan robot menjadi lebih linear. Penggunaan EEPROM terbukti efektif untuk menyimpan data kalibrasi sensor dan parameter Kp serta Kd secara non-volatile, yang memudahkan proses konfigurasi tanpa perlu melakukan pemrograman ulang. Selain itu, penggunaan tombol tekan sebagai alat input eksternal mempermudah proses penalaan parameter secara real-time di lapangan. Secara keseluruhan, sistem ini meningkatkan fleksibilitas dan stabilitas robot dalam mengikuti lintasan garis.

## 7. LAMPIRAN

Link dokumentasi video:
https://drive.google.com/drive/folders/1Lgzs5CNa1o80Mx8uDIgMI3fhwKW2n_Xc?usp=sharing

Dokumentasi:

![[mod-06-serial.jpg | 400]]

![[mod-06-kertas.jpg | 400]]
