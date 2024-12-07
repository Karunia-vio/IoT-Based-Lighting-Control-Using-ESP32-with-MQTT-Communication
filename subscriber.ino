#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int NUM_LAMPS = 3;
const int LED_PIN[NUM_LAMPS] = {18,2,4};
int lampBrightness[NUM_LAMPS] = {0};

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload, length);
  
  int lamp = doc["lamp"];
  int brightness = doc["brightness"];

  if (lamp >= 1 && lamp <= NUM_LAMPS) {
    lampBrightness[lamp - 1] = brightness;
    analogWrite(getPinFromLamp(lamp), brightness * 2.55);
  }
}
int getPinFromLamp(int lamp) {
  
  if (lamp >= 1 && lamp <= NUM_LAMPS) {
    return LED_PIN[lamp - 1];
  } else {
    return -1; 
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Subscriber")) {
      Serial.println("connected");
      client.subscribe("lamp_control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
