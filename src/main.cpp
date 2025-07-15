#include <HardwareSerial.h>

HardwareSerial sim800(2); // Gunakan Serial2
const int SIM_RX = 16; // sesuaikan pin kamu
const int SIM_TX = 17; // sesuaikan pin kamu

String bpm = "85";
String lat = "-2.917";
String lon = "104.719";

void sendCommand(String cmd, int delayMs = 1000) {
  sim800.println(cmd);
  delay(delayMs);
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}

void setup() {
  Serial.begin(115200);
  sim800.begin(115200, SERIAL_8N1, SIM_RX, SIM_TX);

  delay(3000);
  Serial.println("Init...");

  sendCommand("AT");
  sendCommand("AT+CPIN?");
  sendCommand("AT+CSQ");
  sendCommand("AT+CREG?");
  sendCommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  sendCommand("AT+SAPBR=3,1,\"APN\",\"internet\""); // APN by.u/telkomsel
  sendCommand("AT+SAPBR=1,1");
  sendCommand("AT+SAPBR=2,1");

  // Kirim HTTP GET
  sendCommand("AT+HTTPINIT");
  sendCommand("AT+HTTPPARA=\"CID\",1");

  String url = "http://child-safety-iot-system-with-touch-based-alert-production.up.railway.app/send?bpm=" + bpm + "&lat=" + lat + "&lon=" + lon;
  sendCommand("AT+HTTPPARA=\"URL\",\"" + url + "\"");

  sendCommand("AT+HTTPACTION=0");
  delay(5000); // tunggu request
  sendCommand("AT+HTTPREAD");
  sendCommand("AT+HTTPTERM");

  sendCommand("AT+SAPBR=0,1"); // tutup GPRS

  Serial.println("Done.");
}

void loop() {
  // Kosong, kirim hanya sekali
}
