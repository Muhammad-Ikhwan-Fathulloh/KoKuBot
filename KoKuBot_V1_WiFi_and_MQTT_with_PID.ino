#include <WiFi.h>
#include <MQTT.h>

// ---------------- WiFi & MQTT Config ----------------
const char ssid[] = "ssid";       // Ganti dengan WiFi SSID Anda
const char pass[] = "pass";       // Ganti dengan WiFi Password Anda

WiFiClient net;
MQTTClient client;

const char mqttServer[] = "public.cloud.shiftr.io";
const char mqttUser[] = "public";
const char mqttPass[] = "public";

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

// ---------------- PID Variables ----------------
float kp = 1.0, ki = 0.5, kd = 0.2;
float error = 0, previousError = 0, integral = 0, derivative = 0;
int targetSpeed = 150;  // target kecepatan PWM
int currentSpeed = 0;   // kecepatan aktual (dummy)

// ---------------- Fungsi PID ----------------
int calculatePID(int target, int current) {
  error = target - current;
  integral += error;
  derivative = error - previousError;
  previousError = error;

  int output = kp * error + ki * integral + kd * derivative;
  return constrain(output, 0, 255);
}

void setMotorPWM(int pwm) {
  analogWrite(ENA, pwm);
  analogWrite(ENB, pwm);
  analogWrite(ENA2, pwm);
  analogWrite(ENB2, pwm);
}

// ---------------- Fungsi Kendali Motor ----------------
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); digitalWrite(IN8, LOW);
  setMotorPWM(0);
}

void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
}

void moveRight() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
}

void moveLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(IN5, HIGH); digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW); digitalWrite(IN8, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW); digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH); digitalWrite(IN8, LOW);
}

// ---------------- MQTT Handler ----------------
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
  client.subscribe("robot/control");
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin(mqttServer, net);
  client.onMessage(messageReceived);
  connect();

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN5, OUTPUT); pinMode(IN6, OUTPUT); pinMode(ENA2, OUTPUT);
  pinMode(IN7, OUTPUT); pinMode(IN8, OUTPUT); pinMode(ENB2, OUTPUT);
}

// ---------------- Loop ----------------
void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  // Simulasi kecepatan aktual (nanti ganti dengan data encoder)
  currentSpeed = random(120, 180); 

  // Hitung PID dan kontrol PWM motor
  int pwmOutput = calculatePID(targetSpeed, currentSpeed);
  setMotorPWM(pwmOutput);

  if (millis() - lastMillis > 5000) {
    lastMillis = millis();
    String status = "PID: PWM = " + String(pwmOutput) + ", Speed = " + String(currentSpeed);
    client.publish("robot/status", status);
    Serial.println(status);
  }
}
