#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 256

#include <TinyGPS++.h>
#include <TinyGsmClient.h> 
#include <ArduinoHttpClient.h> 

const char FIREBASE_HOST[]  = "tracking-anak-07-default-rtdb.firebaseio.com";
const String FIREBASE_AUTH  = "DWOEgSzSkcgyL7dN7pp6PtNxEGRaHZQ4TtFSKbZT";
const String FIREBASE_PATH  = "TrackingAnak";
const int SSL_PORT          = 443;

char apn[]  = "internet"; 
char user[] = "";
char pass[] = "";

#define rxPin 16
#define txPin 17
HardwareSerial sim800(1);
TinyGsm modem(sim800);

#define RXD2 19
#define TXD2 18
HardwareSerial neogps(2);
TinyGPSPlus gps;

TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT);

unsigned long previousMillis = 0;
const unsigned long interval = 30000; 

// Data GPS global
String latitude = "0.0";
String longitude = "0.0";
String bpm = "98"; 

//**************************************************************************************************
void setup() {
  Serial.begin(9600);
  Serial.println("ESP32 serial initialize");
  
  sim800.begin(9600, SERIAL_8N1, rxPin, txPin);
  Serial.println("SIM800L serial initialize");

  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("NeoGPS serial initialize");

  delay(3000);
  
  Serial.println("Initializing modem...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
  
  http_client.setHttpResponseTimeout(90 * 1000);
}

void loop() {
  // Pastikan GPRS tersambung
  if (!modem.isGprsConnected()) {
    Serial.print(F("Connecting to "));
    Serial.print(apn);
    if (!modem.gprsConnect(apn, user, pass)) {
      Serial.println(" fail");
      delay(1000);
      return;
    }
    Serial.println(" OK");
  }

  // Baca data GPS terus-menerus di background
  bacaGPS();

  // Kirim ke Firebase setiap 30 detik
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    kirimDataFirebase();
  }
}

void bacaGPS() {
  while (neogps.available()) {
    gps.encode(neogps.read());
  }

  if (gps.location.isValid()) {
    latitude = String(gps.location.lat(), 6);
    longitude = String(gps.location.lng(), 6);
  }

  Serial.print("Latitude= ");
  Serial.print(latitude);
  Serial.print(" Longitude= ");
  Serial.println(longitude);
}

void kirimDataFirebase() {
  if (!http_client.connected()) {
    http_client.connect(FIREBASE_HOST, SSL_PORT);
  }

  String gpsData = "{";
  gpsData += "\"latitude\":" + latitude + ",";
  gpsData += "\"longitude\":" + longitude + ",";
  gpsData += "\"bpm\":" + bpm;
  gpsData += "}";

  PostToFirebase("PATCH", FIREBASE_PATH, gpsData, &http_client);
}

void PostToFirebase(const char* method, const String & path , const String & data, HttpClient* http) {
  String url = "/";
  url += path + ".json";
  url += "?auth=" + FIREBASE_AUTH;

  Serial.print("POST: ");
  Serial.println(url);
  Serial.print("Data: ");
  Serial.println(data);

  String contentType = "application/json";
  http->put(url, contentType, data);

  int statusCode = http->responseStatusCode();
  String response = http->responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  if (!http->connected()) {
    Serial.println("HTTP POST disconnected");
    http->stop();
  }
}