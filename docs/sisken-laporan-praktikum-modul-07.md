Nama: Ibrahim Fauzi Rahman
NIM: 607022400009
Kelas: D3TK-48-01

---

<center> <h1>
LAPORAN PRAKTIKUM SISTEM KENDALI <br>
MODUL 07: Kasus P dan D dengan EEPROM dan Push Button
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

1. Variasikan 

### 5.2. KODE PROGRAM

```cpp

```

## 6. KESIMPULAN


## 7. LAMPIRAN

Link dokumentasi video:
https://drive.google.com/drive/folders/1Lgzs5CNa1o80Mx8uDIgMI3fhwKW2n_Xc?usp=sharing

Dokumentasi:

