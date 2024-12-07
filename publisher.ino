#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int NUM_LAMPS = 3;
const int BUTTON_PIN[NUM_LAMPS] = {18,2,4};
int buttonState[NUM_LAMPS] = {HIGH};
int lastButtonState[NUM_LAMPS] = {HIGH};
int buttonPressCounter[NUM_LAMPS] = {0};

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

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  for (int i = 0; i < NUM_LAMPS; i++) {
    int reading = digitalRead(BUTTON_PIN[i]);
    if (reading != lastButtonState[i]) {

      if (reading == LOW) {
        buttonPressCounter[i]++; 

        int brightness = 0;
        if (buttonPressCounter[i] == 1) {
          brightness = 50;
        } else if (buttonPressCounter[i] == 2) {
          brightness = 100;
        } else if (buttonPressCounter[i] == 3) {
          brightness = 0;
          buttonPressCounter[i] = 0; 
        }

        publishMessage(i + 1, brightness);
      }
    }

    lastButtonState[i] = reading;
  }
}

void publishMessage(int lamp, int brightness) {
  StaticJsonDocument<200> doc;
  doc["lamp"] = lamp;
  doc["brightness"] = brightness;

  char buffer[512];
  serializeJson(doc, buffer);
  client.publish("lamp_control", buffer);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32_Publisher")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

