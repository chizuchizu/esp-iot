#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_SHT31.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* udpServerIp = "10.12.24.186";
const int udpPort = 12345;

WiFiUDP udp;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(115200);
  
  if (!sht31.begin(0x44)) {
    Serial.println("SHT31 sensor not found!");
    while (1) delay(1);
  }
  Serial.println("SHT31 sensor initialized!");

  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();

  // Error check
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.printf("Temperature: %.2f °C, Humidity: %.2f %%\n", temperature, humidity);

    udp.beginPacket(udpServerIp, udpPort);
    udp.printf("%.2f,%.2f", temperature, humidity);
    udp.endPacket();
  } else {
    Serial.println("Failed to read from SHT31 sensor!");
  }

  delay(5000);
}

