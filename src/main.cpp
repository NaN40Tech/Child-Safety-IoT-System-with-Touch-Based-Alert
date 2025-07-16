// ESP32 + SIM800L EVB V2
// Kirim data ke Telegram via Replit menggunakan SIM800L HTTP connector

#include <Arduino.h>
#include <SIM800L.h>  // Library: ostaquet/SIM800L HTTP connector

// Konfigurasi pin SIM800L
#define SIM800_TX 17
#define SIM800_RX 16
#define SIM800_RST 5     // RST disambungkan ke D5 (GPIO5)

// Konfigurasi jaringan dan endpoint
const char APN[] = "internet";  // APN untuk Telkomsel/By.U
const char* HOST = "https://ee1e468f-e4af-41fa-a6c3-ed757d70d861-00-36vp7ciz3ewao.pike.replit.dev:3000";
const char* PATH = "/iot";     // endpoint di server

SIM800L* sim800l;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("=== ESP32 + SIM800L EVB (dengan RST di D5) ===");

  // Inisialisasi komunikasi UART SIM800L
  HardwareSerial* serialSim800 = new HardwareSerial(2);
  serialSim800->begin(115200, SERIAL_8N1, SIM800_RX, SIM800_TX);

  // Inisialisasi SIM800L library
  sim800l = new SIM800L((Stream*)serialSim800, SIM800_RST, 200, 512, &Serial);

  // Tunggu hingga module siap
  while (!sim800l->isReady()) {
    Serial.println("⏳ Menunggu module siap...");
    delay(1000);
  }

  // Cek sinyal
  while (sim800l->getSignal() <= 0) {
    Serial.println("📶 Menunggu sinyal...");
    delay(1000);
  }

  // Registrasi jaringan
  while (sim800l->getRegistrationStatus() != REGISTERED_HOME && sim800l->getRegistrationStatus() != REGISTERED_ROAMING) {
    Serial.println("📡 Registrasi jaringan...");
    delay(1000);
  }

  // Setup GPRS
  bool success = sim800l->setupGPRS(APN);
  while (!success) {
    Serial.println("❌ Gagal setup GPRS. Coba ulang...");
    delay(2000);
    success = sim800l->setupGPRS(APN);
  }
  Serial.println("✅ GPRS siap.");

  // Connect GPRS
  bool connected = sim800l->connectGPRS();
  while (!connected) {
    Serial.println("🔁 Koneksi ke GPRS...");
    delay(1000);
    connected = sim800l->connectGPRS();
  }

  String ip = sim800l->getIP();
  Serial.println("✅ IP GPRS: " + ip);

  // Kirim HTTP GET ke server
  String bpm = "88";
  String lat = "-6.20";
  String lon = "106.80";

  String fullUrl = String(HOST) + PATH + "?bpm=" + bpm + "&lat=" + lat + "&lon=" + lon;
  Serial.println("🌐 Kirim HTTP GET...");
  Serial.println("Full URL: " + fullUrl);

  int status = sim800l->doGet(fullUrl.c_str(), 10000);
  if (status == 200) {
    Serial.println("✅ Data terkirim. Response:");
    Serial.println(sim800l->getDataReceived());
  } else {
    Serial.print("❌ Gagal. HTTP code: ");
    Serial.println(status);
  }

  sim800l->disconnectGPRS();
}

void loop() {
  // Kosong (hanya kirim sekali di setup)
}