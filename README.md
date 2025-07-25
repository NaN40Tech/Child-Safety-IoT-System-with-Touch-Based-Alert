# 👶📍 Child Safety IoT System with Touch-Based Alert

Sistem ini adalah **IoT berbasis ESP32** untuk menjaga keamanan anak. Dengan hanya **menyentuh sensor sebanyak 3 kali**, anak dapat mengirimkan sinyal darurat berupa:

* 📍 **Lokasi GPS**
* ❤️ **Detak jantung (BPM)**
* 📲 Terkirim otomatis ke **Telegram orang tua**

Sinyal dikirim via **SIM800L** ke **Firebase Realtime Database**, lalu dipantau oleh **Node-RED**, dan akhirnya diteruskan ke Telegram.

---

## 🧹 Komponen Hardware

| Komponen          | Fungsi                         |
| ----------------- | ------------------------------ |
| **ESP32**         | Mikrokontroler utama           |
| **SIM800L**       | Modul GSM untuk upload data    |
| **GPS NEO-6MV2**  | Pelacakan lokasi               |
| **MAX30102**      | Sensor detak jantung           |
| **Sensor Sentuh** | Trigger sinyal darurat         |
| **Buzzer**        | Penanda sinyal darurat dikirim |
| **Provider**      | Kartu SIM By.U / Telkomsel     |

---

## 🔌 Wiring Pin ESP32

### Sensor Sentuh

```
VCC  → 3.3V  
GND  → GND  
SIG  → GPIO 34  
```

### Buzzer

```
VCC  → GPIO 4  
GND  → GND  
```

### MAX30102

```
VCC  → 3.3V  
GND  → GND  
SDA  → GPIO 21  
SCL  → GPIO 22  
```

### GPS NEO-6MV2

```
VCC  → 3.3V  
RX   → GPIO 19  
TX   → GPIO 18  
GND  → GND  
```

### SIM800L

```
VCC  → 4.2V (langsung dari Baterai 18650)  
RX   → GPIO 16  
```

---

## ↺ Alur Kerja Sistem

1. Anak menekan **sensor sentuh sebanyak 3 kali**.
2. ESP32:
   * Membaca **BPM** dari MAX30102
   * Mengambil **koordinat lokasi** dari GPS
   * Mengirim data ke **Firebase** via SIM800L
3. **Node-RED** yang di-host di **Railway** membaca data dari Firebase
4. Node-RED mengirim **notifikasi Telegram otomatis** ke orang tua
5. **Buzzer menyala** di sisi anak sebagai notifikasi lokal

---

## ⚙️ Instalasi (Arduino IDE)

### 📦 Library yang Dibutuhkan

* `TinyGSM` → komunikasi GSM
* `MAX3010x` → detak jantung
* `SoftwareSerial`
* `ArduinoJson`
* `Adafruit_GPS`

> 💡 **Tips:** Gunakan Board Manager ESP32 terbaru di Arduino IDE

---

## ⚙️ Contoh Format Data ke Firebase

```json
{
  "bpm": 102,
  "latitude": -6.200000,
  "longitude": 106.816666,
  "timestamp": "2025-07-25T17:00:00Z"
}
```

---

## 🧐 Node-RED Workflow

1. Node Firebase membaca path `TrackingAnak`
2. Saat data baru masuk:

   * Ambil BPM + koordinat
   * Format pesan Telegram
   * Kirim via Node `telegram sender`

### 🔐 Environment di Railway

Masukkan environment variables di **Railway ➔ Variables**:

```env
TELEGRAM_TOKEN=xxxxxxxx:xxxxxxxxxxxxxxxxxxxx
CHAT_ID=1234567890
FIREBASE_URL=https://your-project-id.firebaseio.com/TrackingAnak.json
```

---

## 🚀 Hosting Node-RED di Railway (Tanpa CMD!)

### 📁 Struktur Folder

```
📁 my-node-red/
🔼️ server.js
🔼️ package.json
🔼️ .env (JANGAN DI PUSH)
🔼️ flows.json
🔼️ flows_cred.json
```

### 📦 Contoh package.json

```json
{
  "name": "child-safety-iot-nodered",
  "version": "1.0.0",
  "main": "server.js",
  "scripts": {
    "start": "node server.js"
  },
  "dependencies": {
    "node-red": "^3.1.0",
    "node-red-node-telegrambot": "^1.7.0",
    "node-red-contrib-firebase": "^0.1.4"
  }
}
```

### ▶️ Deploy Steps:

1. Buat akun di [Railway](https://railway.app)
2. Hubungkan ke GitHub repo-mu
3. Railway akan auto-build & deploy
4. Akses Node-RED via link public Railway

---

## 📬 Contoh Pesan Telegram

```
🚨 PERINGATAN DARURAT!
❤️ BPM: 102
📍 Lokasi: https://maps.google.com/?q=-6.200000,106.816666
```
