#include <HardwareSerial.h>

HardwareSerial sim800(2); // RX=16, TX=17

// Deklarasi fungsi di atas
void sendCommand(String cmd);

void setup() {
  Serial.begin(115200);
  sim800.begin(115200, SERIAL_8N1, 16, 17);

  Serial.println("Inisialisasi SIM800L...");
  delay(1000);

  sendCommand("AT");
  sendCommand("AT+CPIN?");
  sendCommand("AT+CSQ");
  sendCommand("AT+CREG?");
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  sendCommand("AT+SAPBR=3,1,\"APN\",\"internet\"");
  sendCommand("AT+SAPBR=1,1");
  delay(3000);

  sendCommand("AT+HTTPTERM");
  sendCommand("AT+HTTPINIT");
  sendCommand("AT+HTTPPARA=\"CID\",1");

  // Ganti sesuai URL ngrok kamu
  String url = "http://5b7109ce888c.ngrok-free.app/send?bpm=85&lat=-2.917&lon=104.719";
  sendCommand("AT+HTTPPARA=\"URL\",\"" + url + "\"");

  sendCommand("AT+HTTPACTION=0");
  delay(5000); // tunggu HTTP response

  sendCommand("AT+HTTPREAD");
  sendCommand("AT+HTTPTERM");
  sendCommand("AT+SAPBR=0,1");

  Serial.println("Selesai mengirim pesan.");
}

void loop() {
  // tidak ada loop
}

// Fungsi dipindah ke bawah
void sendCommand(String cmd) {
  sim800.println(cmd);
  delay(1000);

  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}
