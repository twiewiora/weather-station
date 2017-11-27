#include <ESP8266WiFi.h>
#include <Wire.h>
#include <OneWire.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <DallasTemperature.h>

#define wifi_ssid "Wi-Fi"
#define wifi_password "haslookon"

#define mqtt_server "test.mosquitto.org"

#define temperature_in_topic "js8837euf_temperature_in"
#define temperature_out_topic "js8837euf_temperature_out"
#define humidity_topic "js8837euf_humidity"

#define DHT_TYPE DHT11
#define DHT_PIN 2

#define DS18B20_PIN 5

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHT_TYPE);

OneWire oneWire(DS18B20_PIN);
DallasTemperature DS18B20(&oneWire);

void setup() {
  Serial.begin(115200);
  dht.begin();
  DS18B20.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  // Connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  float temp_in = dht.readTemperature();
  float hum = dht.readHumidity();
  
  if (isnan(temp_in) || isnan(hum)) {
    Serial.println("Error read sensor DHT11");
  } else {
    Serial.print("New temperature indoor:");
    Serial.println(String(temp_in).c_str());
    Serial.print("New humidity:");
    Serial.println(String(hum).c_str());
  }
    
  DS18B20.requestTemperatures(); 
  float temp_out = DS18B20.getTempCByIndex(0);
  if (temp_out == 80 || temp_out == (-127)) {
    Serial.println("Error read sensor DS18B20");
  } else {
    Serial.print("New temperature outdoor:");
    Serial.println(String(temp_out).c_str());
  }

  
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 
  
  if(client.publish(temperature_in_topic, String(temp_in).c_str())) {
    Serial.println("Sent indoor temperature");
  } else {
    Serial.println("Didn't send indoor temperature");
  }
  if(client.publish(temperature_out_topic, String(temp_out).c_str())) {
    Serial.println("Sent outdoor temperature");
  } else {
    Serial.println("Didn't send outdoor temperature");
  }
  if(client.publish(humidity_topic, String(hum).c_str())) {
    Serial.println("Sent humidity");
  } else {
    Serial.println("Didn't send humidity");
  }
  delay(5000);
}

