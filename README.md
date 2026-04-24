# Praktikum Sistem Kendali - Robot Line Follower

Repositori ini berisi kode sumber, laporan, dan simulasi untuk praktikum Sistem Kendali (Sisken) yang fokus pada pengembangan dan pengendalian robot line follower. Praktikum ini terdiri dari 5 modul yang secara bertahap mengajarkan konsep sistem kendali dari dasar hingga implementasi PID.

## 📋 Daftar Modul Praktikum

| Modul | Judul | Deskripsi | Teknologi |
|-------|-------|-----------|-----------|
| 1 | Pengenalan Sistem Kendali On/Off dan Sensor pada Robot Line Follower | Pengenalan dasar sensor dan sistem kendali on/off untuk robot line follower | Arduino, Sensor Analog |
| 2 | Sistem Kendali PWM (Pulse Width Modulation) | Implementasi PWM untuk pengendalian kecepatan motor | Arduino, PWM |
| 3 | Sistem Kendali PWM dengan Error Detection Memory | Pengembangan sistem kendali PWM dengan penyimpanan kondisi error | Arduino, State Memory |
| 4 | Sistem Kendali PID Kasus P | Implementasi kontroler PID dengan komponen proporsional | Arduino, PID Control |
| 5 | Sistem Kendali PID Kasus P dan D | Implementasi kontroler PID dengan komponen proporsional dan derivatif | Arduino, PID Control |

## 📁 Struktur Direktori

```
prak-sisken-webots/
├── arduino/                    # Kode sumber Arduino untuk setiap modul
│   ├── modul-01.ino           # Modul 1: Sistem Kendali On/Off
│   ├── modul-02.ino           # Modul 2: Sistem Kendali PWM
│   ├── modul-03.ino           # Modul 3: PWM dengan Error Detection Memory
│   ├── modul-04.ino           # Modul 4: PID Kasus P
│   └── modul-05.ino           # Modul 5: PID Kasus P dan D
├── docs/                      # Laporan praktikum dalam format markdown
│   ├── sisken-laporan-praktikum-modul-01.md
│   ├── sisken-laporan-praktikum-modul-02.md
│   ├── sisken-laporan-praktikum-modul-03.md
│   ├── sisken-laporan-praktikum-modul-04.md
│   └── sisken-laporan-praktikum-modul-05.md
├── controllers/               # Kontroller untuk simulasi Webots
│   └── line_follower/
│       ├── line_follower.cpp  # Kontroller C++ untuk robot line follower
│       └── README.md
├── worlds/                    # File dunia simulasi Webots
│   ├── main.wbt              # Dunia utama dengan arena line follower
│   └── .main.wbproj          # Proyek Webots
└── README.md                 # File ini
```

## 🚀 Prasyarat dan Instalasi

### 1. Perangkat Keras (Opsional untuk Simulasi)
- Robot Kit Line Follower
- Arduino Nano
- Baterai LiPo 2-Cell 1300 mAh
- Sensor line follower 8 channel

### 2. Perangkat Lunak
- **Arduino IDE** (versi 1.8.x atau lebih baru)
  - Download dari [arduino.cc](https://www.arduino.cc/en/software)
  - Instal driver untuk Arduino Nano jika diperlukan

- **Webots** (versi R2021b atau lebih baru)
  - Download dari [cyberbotics.com](https://cyberbotics.com/)
  - Versi gratis untuk pendidikan tersedia

- **Proteus** (untuk simulasi rangkaian, opsional)
  - Digunakan untuk simulasi rangkaian elektronik

## 💻 Panduan Penggunaan

### Menjalankan Kode Arduino

1. **Buka Arduino IDE**
2. **Pilih Board**: Tools → Board → Arduino Nano
3. **Pilih Port**: Tools → Port → (port Arduino Anda)
4. **Buka file** dari direktori `arduino/` (misalnya `modul-01.ino`)
5. **Upload** kode ke Arduino

### Menjalankan Simulasi Webots

1. **Buka Webots**
2. **File → Open World** → pilih `worlds/main.wbt`
3. **Build kontroller** (jika diperlukan):
   - Klik kanan pada robot E-puck
   - Pilih "Edit Controller"
   - Build kontroller dari `controllers/line_follower/`
4. **Jalankan simulasi** dengan tombol play

### Struktur Kode Arduino

Setiap modul memiliki struktur yang serupa dengan peningkatan kompleksitas:

```cpp
// Contoh struktur dasar (Modul 1)
const int pin_sensor_kiri = A0;
const int pin_sensor_kanan = A7;
const int threshold = 500;

void setup() {
  // Inisialisasi pin dan serial monitor
}

void loop() {
  // Baca sensor
  // Logika kendali on/off
  // Kendali motor
}
```

## 🔧 Spesifikasi Teknis

### Sensor
- 8 sensor analog (4 kiri, 4 kanan)
- Tipe: Photodioda dengan LED inframerah
- Range pembacaan: 0-1023 (10-bit ADC)
- Threshold: 500 (dapat disesuaikan)

### Motor
- 2 motor DC dengan gearbox
- Kendali PWM untuk kecepatan
- Pin PWM: 6 (kiri) dan 9 (kanan)

### Logika Kendali
- **Modul 1**: On/Off control berdasarkan 2 sensor utama
- **Modul 2**: PWM control dengan variasi kecepatan
- **Modul 3**: PWM dengan memory error state
- **Modul 4**: PID control (komponen P)
- **Modul 5**: PID control (komponen P dan D)

## 📚 Teori Dasar

### Sistem Kendali On/Off
Sistem kendali paling sederhana dimana output hanya memiliki dua keadaan: ON atau OFF. Digunakan pada modul 1 untuk pengenalan dasar.

### Pulse Width Modulation (PWM)
Teknik modulasi lebar pulsa untuk mengontrol daya rata-rata yang dikirim ke beban. Pada modul 2 dan 3, PWM digunakan untuk mengontrol kecepatan motor.

### Proportional-Integral-Derivative (PID)
Algoritma kendali umpan balik yang menghitung error sebagai perbedaan antara setpoint dan variabel proses. Pada modul 4 dan 5, PID diimplementasikan untuk kontrol yang lebih presisi.


## 📄 Lisensi

Proyek ini dibuat untuk tujuan pendidikan sebagai bagian dari praktikum Sistem Kendali. Kode dapat digunakan dan dimodifikasi dengan atribusi yang sesuai.

## 🔗 Referensi

1. Arduino Documentation - https://www.arduino.cc/reference/en/
2. Webots Documentation - https://cyberbotics.com/doc/guide/
3. PID Controller Theory - https://en.wikipedia.org/wiki/PID_controller
4. Line Follower Robot Design - Berbagai sumber praktikum robotika

---

*Terakhir diperbarui: April 2026*
