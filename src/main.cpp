#include <Arduino.h>
#include "SIM800L.h"

#define SIM800_RST_PIN 5  // Bisa diabaikan jika tidak digunakan

SIM800L* sim800l;
HardwareSerial sim800Serial(2); // UART2 di ESP32: RX=16, TX=17

// APN Telkomsel atau By.U
const char APN[] = "internet";

// GANTI INI DENGAN LINK HTTP DARI REPLIT KAMU (HARUS PAKAI http://, BUKAN https://)
const char URL[] = "http://ee1e468f-e4af-41fa-a6c3-ed757d70d861-00-36vp7ciz3ewao.pike.replit.dev/iot?bpm=80&lat=-6.2&lon=106.8";

void setupModule();

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== ESP32 + SIM800L ===");

  sim800Serial.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17

  sim800l = new SIM800L((Stream *)&sim800Serial, SIM800_RST_PIN, 200, 512, &Serial);
  setupModule();
}

void loop() {
  Serial.println("🔁 Connecting GPRS...");

  bool connected = false;
  for (uint8_t i = 0; i < 5 && !connected; i++) {
    delay(1000);
    connected = sim800l->connectGPRS();
  }

  if (!connected) {
    Serial.println("❌ Gagal GPRS. Reset module...");
    sim800l->reset();
    setupModule();
    return;
  }

  Serial.println("✅ GPRS Connected. IP: " + sim800l->getIP());

  Serial.println("🌐 Kirim HTTP GET...");
  uint16_t rc = sim800l->doGet(URL, 10000);
  if (rc == 200) {
    Serial.println("✅ Sukses kirim!");
    Serial.println(sim800l->getDataReceived());
  } else {
    Serial.print("❌ Gagal kirim. Error: ");
    Serial.println(rc);
  }

  sim800l->disconnectGPRS();
  delay(60000);  // kirim tiap 1 menit
}

void setupModule() {
  while (!sim800l->isReady()) {
    Serial.println("Menunggu module siap...");
    delay(1000);
  }

  while (sim800l->getSignal() <= 0) {
    Serial.println("📶 Menunggu sinyal...");
    delay(1000);
  }

  while (sim800l->getRegistrationStatus() != REGISTERED_HOME &&
         sim800l->getRegistrationStatus() != REGISTERED_ROAMING) {
    Serial.println("📡 Registrasi jaringan...");
    delay(1000);
  }

  bool gprs = sim800l->setupGPRS(APN);
  while (!gprs) {
    Serial.println("❌ Gagal setup GPRS, coba lagi...");
    delay(2000);
    gprs = sim800l->setupGPRS(APN);
  }

  Serial.println("✅ GPRS Siap!");
}
