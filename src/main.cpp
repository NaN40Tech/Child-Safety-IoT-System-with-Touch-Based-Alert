#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial sim800(1); // UART1 untuk SIM800L

#define MODEM_RX 16
#define MODEM_TX 17

// URL dan IP Webhook (ganti jika perlu)
const String webhookPath = "/0bcc8843-1ca4-458b-baf7-585d5405f6a8";  // dari webhook.site
const String webhookHost = "webhook.site";
const String webhookIP   = "178.63.67.153";  // IPv4 yang cocok untuk SIM800L

const String message = "Halo dari ESP32 + SIM800L";

// Fungsi kirim AT Command
void sendAT(String cmd, int wait = 1000) {
  sim800.println(cmd);
  delay(wait);
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}

void setup() {
  Serial.begin(115200);
  sim800.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  Serial.println("Inisialisasi SIM800L...");

  // Langkah awal koneksi GPRS
  sendAT("AT");
  sendAT("AT+CSQ");  // Cek sinyal
  sendAT("AT+CGATT?"); // Cek status attach
  sendAT("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  sendAT("AT+SAPBR=3,1,\"APN\",\"internet\""); // APN default Telkomsel/By.U
  sendAT("AT+SAPBR=1,1", 3000); // Mulai koneksi GPRS
  sendAT("AT+SAPBR=2,1");       // Cek IP

  Serial.println("Mengirim pesan ke webhook...");

  // Siapkan dan kirim HTTP GET ke webhook
  sendAT("AT+HTTPTERM"); // Terminate sesi sebelumnya (jika ada)
  sendAT("AT+HTTPINIT");
  sendAT("AT+HTTPPARA=\"CID\",1");

  // Buat URL lengkap (gunakan IP, bukan domain langsung)
  String fullUrl = "http://" + webhookIP + webhookPath + "?msg=" + message;

  sendAT("AT+HTTPPARA=\"URL\",\"" + fullUrl + "\"");
  sendAT("AT+HTTPPARA=\"HOST\",\"" + webhookHost + "\""); // Penting agar HOST dikenali oleh server
  sendAT("AT+HTTPPARA=\"UA\",\"ESP32SIM800\""); // Opsional user-agent
  sendAT("AT+HTTPACTION=0", 8000); // GET request
  sendAT("AT+HTTPREAD", 2000);     // Baca respons
}

void loop() {
  // Tidak melakukan apa-apa di loop
}
