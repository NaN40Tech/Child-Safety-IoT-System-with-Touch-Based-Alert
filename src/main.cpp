#include <Arduino.h>
#include <HardwareSerial.h>

// Konfigurasi pin SIM800L EVB (gunakan UART2)
HardwareSerial sim800(2);
const int SIM_RX = 16;
const int SIM_TX = 17;

// Data awal (simulasi)
String bpm = "75";
String lat = "-6.200000";
String lon = "106.816666";

// Deklarasi fungsi sebelum digunakan
void sendHTTPRequest();
void sendATCommand(String command, int delayMs = 1000);

void setup() {
  Serial.begin(115200);
  sim800.begin(115200, SERIAL_8N1, SIM_RX, SIM_TX);
  
  delay(3000);
  Serial.println("=== ESP32 + SIM800L EVB Test ===");
  Serial.println("Testing Telegram message via HTTP...");

  // Inisialisasi koneksi SIM800L
  sendATCommand("AT", 2000);
  sendATCommand("AT+CSQ", 1000);     // Cek sinyal
  sendATCommand("AT+CPIN?", 1000);   // Cek status SIM
  sendATCommand("AT+CREG?", 2000);   // Cek status jaringan

  // Setup GPRS
  Serial.println("Setting up GPRS...");
  sendATCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", 1000);
  sendATCommand("AT+SAPBR=3,1,\"APN\",\"internet\"", 1000); // APN By.U/Telkomsel
  sendATCommand("AT+SAPBR=1,1", 3000);  // Aktifkan GPRS
  sendATCommand("AT+SAPBR=2,1", 2000);  // Tampilkan status GPRS

  // Kirim data ke server Railway
  sendHTTPRequest();

  // Matikan GPRS setelah kirim
  sendATCommand("AT+SAPBR=0,1", 1000);

  Serial.println("=== Test Complete ===");
}

void loop() {
  delay(30000); // Kirim ulang setiap 30 detik

  Serial.println("Sending another test message...");

  // Simulasikan data acak
  bpm = String(random(65, 100));
  lat = String(-6.2 + random(-50, 50) * 0.0001);   // Sekitar Jakarta
  lon = String(106.8 + random(-50, 50) * 0.0001);  // Sekitar Jakarta

  // Aktifkan GPRS ulang
  sendATCommand("AT+SAPBR=1,1", 2000);

  // Kirim data
  sendHTTPRequest();

  // Matikan GPRS
  sendATCommand("AT+SAPBR=0,1", 1000);
}

void sendHTTPRequest() {
  sendATCommand("AT+HTTPINIT", 2000);
  sendATCommand("AT+HTTPPARA=\"CID\",1", 1000);

  // ✅ GUNAKAN HTTP, BUKAN HTTPS!
  String url = "http://child-safety-iot-system-with-touch-based-alert-production.up.railway.app/iot?bpm=" + bpm + "&lat=" + lat + "&lon=" + lon;

  Serial.print("URL: ");
  Serial.println(url);

  sendATCommand("AT+HTTPPARA=\"URL\",\"" + url + "\"", 1000);
  sendATCommand("AT+HTTPACTION=0", 10000);  // 0 = GET request
  sendATCommand("AT+HTTPREAD", 3000);
  sendATCommand("AT+HTTPTERM", 1000);
}

void sendATCommand(String command, int delayMs) {
  Serial.print("Sending: ");
  Serial.println(command);

  sim800.println(command);
  delay(delayMs);

  while (sim800.available()) {
    Serial.write(sim800.read());
  }
  Serial.println();
}
