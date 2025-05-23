#define BLYNK_TEMPLATE_ID "TMPL6NPW8MOWX"
#define BLYNK_TEMPLATE_NAME "monitoring"
#define BLYNK_AUTH_TOKEN "R94JIAMz6MyVYpcrPIqlIgCUGU9sP15q"

// Mengimpor pustaka yang diperlukan
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Kredensial WiFi Anda
char ssid[] = "Faiq Abdillah";
char pass[] = "Bungapersik";

// Pin analog sensor kelembaban tanah
const int sensorPin = A0;

// Variabel untuk menyimpan pembacaan sensor dan persentase kelembaban tanah
int sensorValue = 0;
int soilMoisturePercent = 0;

// Rentang kelembaban tanah optimal dalam persentase
const int optimalMin = 50;
const int optimalMax = 70;

// Pin Virtual Blynk untuk mengirim kelembaban tanah
#define VPIN_SOIL_MOISTURE V1

BlynkTimer timer;

void setup()
{
  // Konsol debug
  Serial.begin(9600);
  delay(100);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Mengatur timer untuk mengirim data kelembaban tanah setiap 5 detik
  timer.setInterval(5000L, sendSoilMoisture);
}

void sendSoilMoisture()
{
  sensorValue = analogRead(sensorPin);

  // Memetakan nilai analog sensor ke persentase
  soilMoisturePercent = map(sensorValue, 1023, 301, 0, 100);

  // Mengatur nilai agar berada dalam rentang 0 - 100%
  soilMoisturePercent = constrain(soilMoisturePercent, 0, 100);

  Serial.print("Nilai Sensor: ");
  Serial.print(sensorValue);
  Serial.print("  | Kelembaban Tanah: ");
  Serial.print(soilMoisturePercent);
  Serial.println("%");

  // Mengirim data kelembaban tanah ke Aplikasi Blynk
  Blynk.virtualWrite(VPIN_SOIL_MOISTURE, soilMoisturePercent);

  // Kirim notifikasi jika kelembaban tanah di luar rentang optimal
  if (soilMoisturePercent < optimalMin || soilMoisturePercent > optimalMax) {
    Blynk.logEvent("peringatan_kelembaban", String("Kelembaban tanah tidak optimal: ") + soilMoisturePercent + "%");
  }
}

void loop()
{
  Blynk.run();
  timer.run();
}
