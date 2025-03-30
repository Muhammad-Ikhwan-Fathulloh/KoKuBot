#include <WiFi.h>
#include <MQTT.h>

// ---------------- WiFi & MQTT Config ----------------
const char ssid[] = "ssid";       // Ganti dengan WiFi SSID Anda
const char pass[] = "pass";       // Ganti dengan WiFi Password Anda

WiFiClient net;
MQTTClient client;

// MQTT Broker
const char mqttServer[] = "public.cloud.shiftr.io";  // Broker MQTT
const char mqttUser[] = "public";    // Username Shiftr.io
const char mqttPass[] = "public";    // Password Shiftr.io

unsigned long lastMillis = 0;

// ---------------- Pin Motor ----------------
#define IN1 13
#define IN2 12
#define ENA 14

#define IN3 27
#define IN4 26
#define ENB 25

#define IN5 33
#define IN6 32
#define ENA2 15

#define IN7 4
#define IN8 2
#define ENB2 0

// ---------------- Fungsi Kendali Motor ----------------
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);
}

void moveForward() { // Maju
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
}

void moveBackward() { // Mundur
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

void moveRight() { // Geser kanan
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

void moveLeft() { // Geser kiri
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
}

void turnRight() { // Putar kanan
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

void turnLeft() { // Putar kiri
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
}

// ---------------- MQTT Message Handler ----------------
void messageReceived(String &topic, String &payload) {
  Serial.println("Message from MQTT: " + topic + " - " + payload);

  if (payload == "forward") moveForward();
  else if (payload == "backward") moveBackward();
  else if (payload == "left") moveLeft();
  else if (payload == "right") moveRight();
  else if (payload == "turn_right") turnRight();
  else if (payload == "turn_left") turnLeft();
  else stopMotors();
}

void connect() {
  Serial.print("Checking WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nConnecting to MQTT...");
  while (!client.connect("ESP32_OMNIWHEEL", mqttUser, mqttPass)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected!");
  client.subscribe("robot/control");  // Subscribe ke topic "robot/control"
}

void setup() {
  Serial.begin(115200);

  // Start WiFi
  WiFi.begin(ssid, pass);
  client.begin(mqttServer, net);
  client.onMessage(messageReceived);

  connect();

  // Konfigurasi pin motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(ENA2, OUTPUT);
  
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  pinMode(ENB2, OUTPUT);
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  // Publish pesan ke topic MQTT setiap 5 detik
  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    client.publish("robot/status", "Running");
  }
}
