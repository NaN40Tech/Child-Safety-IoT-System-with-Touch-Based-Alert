# Child Safety IoT System with Touch-Based Alert

Proyek ini merupakan sistem keamanan anak berbasis **Internet of Things (IoT)** yang memungkinkan anak untuk mengirim sinyal darurat melalui sentuhan. Dengan menekan sensor sentuh sebanyak **tiga kali berturut-turut**, sistem akan secara otomatis mengirimkan **lokasi GPS dan detak jantung (BPM)** ke **Telegram**, serta mengaktifkan **buzzer** sebagai tanda lokal bahwa sinyal telah dikirim.

---

## 🔧 Komponen yang Digunakan

- **ESP32** – Mikrokontroler utama
- **GPS Module** – Untuk pelacakan lokasi anak secara real-time
- **SIM800L EVB** – Modul GSM untuk mengirim data ke Telegram
- **MAX30102** – Sensor detak jantung (BPM)
- **Sensor Sentuh** – Sebagai tombol darurat
- **Buzzer** – Penanda sinyal darurat berhasil dikirim
- **Provider** – By.U / Telkomsel

---

## ⚙️ Cara Kerja

1. Anak menekan sensor sentuh sebanyak **3 kali terus menerus**.
2. ESP32 membaca:
   - **Detak jantung** dari sensor MAX30102.
   - **Lokasi GPS** dari modul GPS.
3. Data dikirim ke **Telegram** menggunakan modul GSM SIM800L.
4. **Buzzer menyala** untuk menandakan bahwa sinyal telah berhasil dikirim.

---

## 🚀 Instalasi & Setup

### 1. Software & Library yang Dibutuhkan (Arduino IDE)
- `TinyGSM`
- `MAX3010x`
- `SoftwareSerial`
- `Adafruit_GPS` *(jika pakai GPS TTL)*
- `ArduinoJson`
- `WiFi.h` *(bawaan ESP32)*

### 2. Hardware Setup
- Hubungkan sensor-sensor ke ESP32 sesuai dengan pin yang ditentukan dalam kode.
- Gunakan power supply eksternal 5V untuk SIM800L agar stabil.
- Pastikan kartu SIM (By.U / Telkomsel) memiliki pulsa atau kuota internet aktif.

### 3. Konfigurasi
Edit file `config.h` atau bagian konfigurasi di sketch:
```cpp
#define BOT_TOKEN "TOKEN_BOT_TELEGRAM_ANDA"
#define CHAT_ID "CHAT_ID_ORANG_TUA"
#define APN "internet" // untuk Telkomsel / By.U
