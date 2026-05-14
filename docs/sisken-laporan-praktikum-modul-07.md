Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<center> <h1>
LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 07: Sistem Kendali PID Kasus P, I, dan D dengan EEPROM
</h1> </center>

---

## 1. JUDUL PRAKTIKUM
Sistem Kendali PID Kasus P, I, dan D dengan EEPROM

## 2. MAKSUD DAN TUJUAN

Maksud dan tujuan dari praktikum ini adalah :
1. Mahasiswa dapat memahami fungsi dan cara kerja PID pada motor DC
2. Mahasiswa dapat membuat program berbasis timer untuk melakukan algoritma PID.
3. Mahasiswa dapat menggunakan peripheral berupa push button untuk menambah konstanta Kp, Ki, dan Kd.

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

Jika dengan kasus P dan D telah mengendalikan pergerakan robot dengancukup smooth, maka penambahan Integratif menjadi opsional. Komponen Integratif (I) digunakan untuk mengakumulasi error dan mengetahui durasi error. Dengan menjumlahkan error disetiap pembacaan process value (PV) akan memberikan akumulasi offset yang harus diperbaiki sebelumnya. Saat robot bergerak menjauhi garis, maka nilai error akan bertambah. Semakin lama tidak mendapatkan set point (SP), maka semakin besar nilai I. Dengan mendapatkan nilai Ki yang tepat, imbas dari Integratif bisa dikurangi. Nilai akumulasi error didapat dari: error + last_error. Proses perhitungan integral secara intuitif melibatkan komponen waktu, sehingga implementasinya dibutuhkan suatu fungsi timer pada mikrokontroler. Dalam Arduino untuk menghitung waktu atau fungsi timer digunakan fungsi millis() menghasilkan jumlah milidetik semenjak program berjalan. Fungsi ini memiliki tipe data Unsigned Long. Jumlah milidetik yang dihasilkan ini harus dicatat dalam setiap perulangan program. 

Nilai konstanta perhitungan PID di-tuning secara trial and error dan proses ini dilakukan dengan metode eksperimental. Nilai proporsional, derivative, dan integratif pada algoritma PID diujicoba hingga ditemukan hasil sistem yang stabil. Adapun cara yang dilakukan untuk men-tuning PID pada robot line follower adalah sebagai berikut:
dicatat dalam setiap perulangan program.

1. Langkah awal gunakan kontrol proporsional terlebih dahulu, abaikan konstanta integratif dan derivatifnya dengan memberikan nilai nol pada integratif dan derivatif.
2. Tambahkan terus konstanta proporsional maksimum hingga keadaan stabil namun robotmasih berosilasi.
3. Untuk meredam osilasi, tambahkan konstanta derivatif dengan membagi dua nilai proporsional, amati keadaan sistem robot hingga stabil dan lebih responsif.
4. Jika sistem robot telah stabil, kontrol integratif dapat menjadi opsional, dalam artian jika ingin mencoba-coba tambahkan kontrol integratif tersebut, namun pemberian nilai
integratif yang tidak tepat dapat membuat sistem robot menjadi tidak stabil.
5. Nilai set point kecepatan dan nilai batas bawah/atas memberikan patokan kecepatan robot.
6. Nilai time sampling (waktu cuplik) juga mempengaruhi perhitungan PID, tentunnya saat penggunaan kontrol integratif dan derivatif.
7. Periksa kembali perfoma sistem hingga mendapatkan hasil yang memuaskan.

## 5. KEGIATAN PRAKTIKUM

5.1. Memodifikasi program pada praktikum sebelumnya dengan variable dan fungsi berikut ini:

- Tambahkan variabel integer dengan nama 'state' dengan nilai awal adalah 0.
- Tambahkan variabel integer dengan nama 'setting' dengan nilai awal adalah 0.
- Tambahkan variabel integer berupa array dengan nama 'peka' dari 0 hingga 7 dengan nilai awal adalah 500.
- Tambahkan variabel integer errSum
- Tambahkan variabel unsigned long SampleTime dengan nilai awal 1000. Angka 1000 menunjukkan nilai 1 detik.
- Tambahkan variabel now, lastTime, dan timeChange dengan tipe data unsigned long.
- Tambahkan variabel integer dengan nama 'Kp' dengan nilai awal adalah 20.
- Tambahkan variabel integer dengan nama 'Ki' dengan nilai awal adalah 20.
- Tambahkan variabel integer dengan nama 'Kd' dengan nilai awal adalah 5.

- Di dalam void setup tambahkan:
    - Baca nilai eeprom yang terdapat di EEPROM dengan alamat 0 hingga 7 (dengan ketentuan alamat 0 adalah untuk data nilai tengah kalibrasi sensor 1, alamat 1 untuk nilai tengah kalibrasi sensor 2, dan seterusnya hingga sensor 8) dengan perintah EEPROM.read(0) hingga EEPROM.read(7), kemudian simpan didalam variabel peka[0] hingga peka[7] dengan mengkalikandengan angka 4 (contoh : peka[0]=EEPROM.read[0]*4).
    - Baca nilai eeprom yang terdapat di EEPROM dengan alamat 8 (ketentuan alamat 8 adalah data nilai Kp), kemudian simpandidalam variabel Kp.
    - Baca nilai eeprom yang terdapat di EEPROM dengan alamat 9 (ketentuan alamat 9 adalah data nilai Kd), kemudian simpandidalam variabel Kp.
    - Tampilkan data peka[0] hingga peka[7], Kp dan Kd yang berasal dari data EEPROM ke Serial Monitor.

- Di dalam void loop ditambahkan conditional sebagai berikut :
    - Jika tombol kiri belakang ditekan dan setting bernilai 0 maka 'state' akan bernilai 1.
    - Jika tombol kiri depan ditekan dan setting bernilai 0 maka 'state' akan bernilai 2 dan 'setting' akan bernilai 1.
    - Jika tombol kiri belakang ditekan dan setting bernilai 1 maka 'state' akan bernilai 3.
    - Jika tombol kiri depan ditekan dan setting bernilai 1 maka 'state' akan bernilai 4.
    - Jika tombol kanan belakang ditekan dan setting bernilai 1 maka 'state' akan bernilai 5.
    - Jika tombol kanan depan ditekan dan setting bernilai 1 maka 'state' akan bernilai 6.
    - Jika tombol kanan dan kiri depan ditekan secara bersamaan dan setting bernilai 1 maka 'state' akan bernilai 7.
    - Jika tombol kanan dan kiri belakang ditekan secara bersamaan dan setting bernilai 1 maka 'state' akan bernilai 8.

- Perubahan state tersebut akan mengaktifkan sub program berikut :
    - Jika 'state' bernilai 1 maka proses auto calibration berlangsung.
    - Jika 'state' bernilai 2 maka nilai terakhir variabel 'peka[0]' hingga 'peka[7]' disimpan ke dalam EEPROM dengan alamat 0 hingga 7 dan menjalankan perintah proses line follower dengan sistem kendali PID dengan nilai peka[0] hingga peka[7], Kp dan Kd yang diperoleh dari data EEPROM dan atau berasal dari hasil autocallibration.
    - Perintah EEPROM.write(alamat,value) dengan value yang dibagi dengan 4. Jelaskan mengapa value harus dikali dan dibagi 4! 

        Contoh : EEPROM.write(0, peka[0]/4) ‐ Jika 'state' bernilai 3 maka LED kiri akan menyala dan setelah 500 milidetik LED kiri akan mati, mengurangi nilai Kp(contoh : Kp=Kp-1;), menyimpan nilai Kp kedalam EEPROM pada alamat 8.

    - Jika 'state' bernilai 4 maka LED kiri akan menyala dan setelah 500 milidetik LED kiri akan mati, menambah nilai Kp(contoh : Kp=Kp+1;), menyimpan nilai Kp kedalam EEPROM pada alamat 8.
    - Jika 'state' bernilai 5 maka LED kanan akan menyala dan setelah 500 milidetik LED kiri akan mati, mengurangi nilai Kd(contoh : Kd=Kd-1;), menyimpan nilai Kd kedalam EEPROM pada alamat 9
    - Jika 'state' bernilai 6 maka LED kanan akan menyala dan setelah 500 milidetik LED kiri akan mati, menambah nilai Kd(contoh : Kd=Kd+1;), menyimpan nilai Kd kedalam EEPROM pada alamat 10
    - Jika 'state' bernilai 7 maka LED kiri dan kanan akan menyala dan setelah 500 milidetik LED kiri dan kanan akan mati, menambah nilai Ki(contoh : Ki=Ki+1;), menyimpan nilai Ki kedalam EEPROM pada alamat 11.
    - Jika 'state' bernilai 8 maka LED kiri dan kanan akan menyala dan setelah 500 milidetik LED kiri dan kanan akan mati, mengurangi nilai Ki(contoh : Ki=Ki-1;), menyimpan nilai Ki kedalam EEPROM pada alamat 12.

5.2. Gunakan fungsi millis() untuk menghitung jumlah milidetik semenjak program berjalan. Output dari fungsi ini memiliki tipe data Unsigned Long. Simpan output fungsi ini dalam variable now. Hitung selisih antara variabel waktu now dengan lastTime dalam variable timeChange.

5.3. Apabila nilai timeChange lebih besar sama dengan nilai SampleTime, maka lakukan proses perhitungan algoritma PID adalah sebagai berikut:

- Simpan nilai error saat ini dengan variabel error dan nilai error sebelumnya dengan variable lastError.
- Simpan nilai selisih antara lastError dengan error dalam variabel rate.
- Simpan nilai penjumlahan antara hasil perkalian error dan timeChange dengan errSum dalam variable errSum. Hak ini menunjukkan bahwa nilai variabel errSum selalu di-update setiap saat.
- Simpan nilai moveControl =( Kp x Error)+(Ki x errSum)+(Kd x rate).
- Simpan nilai kecepatanMotorKanan = kecepatanSetPoint dikurang moveControl
- Simpan nilai kecepatanMotorKiri = kecepatanSetPoint ditambah moveControl
- Kecepatan Motor Kiri dengan nilai analog sebesar kecepatanMotorKiri
- Kecepatan Motor Kanan dengan nilai analog sebesar kecepatanMotorKanan


### 5.1. PERTANYAAN PRAKTIKUM

1. Jelaskan mengapa value harus dikali dan dibagi 4!
    
    Jawab:
Nilai ADC yang dibaca dari sensor berkisar antara 0 sampai 1023 (10 bit), sedangkan memori EEPROM pada Arduino Nano hanya mampu menyimpan data 1 byte atau 8 bit dengan rentang nilai 0 sampai 255. Oleh karena itu, nilai ADC perlu dibagi 4 terlebih dahulu sebelum disimpan ke dalam EEPROM agar masuk ke dalam rentang 0 sampai 255. Kemudian ketika data tersebut dibaca kembali dari EEPROM, nilai tersebut harus dikali 4 untuk mengembalikannya ke rentang nilai ADC semula (0-1023). Proses ini merupakan teknik kompresi data sederhana agar nilai ADC yang memiliki resolusi 10 bit dapat disimpan pada memori yang hanya berkapasitas 8 bit.

2. Apa yang menjadi kesimpulan dari hasil percobaan ini?
    
    Jawab:
Berdasarkan hasil percobaan, dapat disimpulkan bahwa sistem kendali PID berhasil diimplementasikan pada robot *line follower* dengan menggunakan mikrokontroler Arduino Nano. Penggunaan fungsi `millis()` sebagai *timer* memungkinkan perhitungan nilai integral dan derivatif secara akurat berdasarkan selisih waktu antar iterasi. Penambahan konstanta integral (Ki) memberikan kemampuan bagi robot untuk mengakumulasi *error* dari waktu ke waktu sehingga robot dapat kembali ke garis meskipun telah bergeser dalam waktu yang cukup lama. Parameter PID (Kp, Ki, Kd) dapat disesuaikan secara langsung melalui tombol pada robot tanpa harus memprogram ulang, dan nilai-nilai tersebut disimpan ke dalam EEPROM agar tetap tersimpan meskipun robot dimatikan. Proses kalibrasi sensor juga menghasilkan nilai tengah (*threshold*) yang disimpan ke dalam EEPROM sehingga robot dapat langsung beroperasi tanpa kalibrasi ulang setiap kali dinyalakan.

### 5.2. KODE PROGRAM

```cpp
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
```

## 6. KESIMPULAN

Berdasarkan hasil praktikum yang telah dilakukan, dapat diambil beberapa kesimpulan sebagai berikut.

1. Sistem kendali PID berhasil diimplementasikan pada robot *line follower* dengan menggunakan mikrokontroler Arduino Nano. Komponen proporsional (Kp) berfungsi memberikan koreksi berdasarkan besar *error* saat ini, komponen integral (Ki) mengakumulasi *error* dari waktu ke waktu untuk menghilangkan *offset*, dan komponen derivatif (Kd) memberikan koreksi berdasarkan laju perubahan *error* untuk meredam osilasi.

2. Penggunaan fungsi `millis()` sebagai *timer* memungkinkan perhitungan nilai integral dan derivatif secara akurat karena selisih waktu antar iterasi (*timeChange*) dihitung secara dinamis. Hal ini penting karena komponen integral dan derivatif sangat bergantung pada faktor waktu dalam perhitungannya.

3. Parameter PID (Kp, Ki, Kd) dapat disesuaikan secara langsung melalui kombinasi tombol pada robot tanpa harus memprogram ulang. Nilai-nilai tersebut disimpan ke dalam memori EEPROM sehingga tetap tersimpan meskipun robot dimatikan. Proses penyimpanan nilai ADC ke EEPROM memerlukan pembagian dengan 4 terlebih dahulu karena rentang nilai ADC (0-1023) tidak muat dalam memori 1 byte (0-255).

4. Proses kalibrasi sensor adaptif menghasilkan nilai tengah (*threshold*) untuk masing-masing sensor yang kemudian disimpan ke dalam EEPROM. Dengan penyimpanan ini, robot dapat langsung beroperasi tanpa perlu melakukan kalibrasi ulang setiap kali dinyalakan.

5. *State machine* dengan 9 *state* berhasil mengelola berbagai mode operasi robot, mulai dari *idle*, kalibrasi, *line following*, hingga penyesuaian parameter PID. Perpindahan antar *state* dipicu oleh tekanan tombol yang dikonfigurasi sesuai kebutuhan praktikum.

## 7. LAMPIRAN

Link dokumentasi video:
https://drive.google.com/drive/folders/1Lgzs5CNa1o80Mx8uDIgMI3fhwKW2n_Xc?usp=sharing

Dokumentasi:

