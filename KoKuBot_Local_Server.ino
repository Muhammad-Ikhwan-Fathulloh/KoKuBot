#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiManager.h>
#include <WebSocketsServer.h>
#include <Ticker.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

// SSID dan Password
const char* ssid = "ESP32";
const char* password = "1sampai100";

// Pin Motor
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

AsyncWebServer server(80);
WebSocketsServer websockets(81);
Ticker timer;

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

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

void moveRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

void moveLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        websockets.sendTXT(num, "Connected from server");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)(payload));
      if (message == "forward") moveForward();
      else if (message == "backward") moveBackward();
      else if (message == "right") moveRight();
      else if (message == "left") moveLeft();
      else if (message == "turnRight") turnRight();
      else if (message == "turnLeft") turnLeft();
      else if (message == "stop") stopMotors();
      break;
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/html", "Page Not Found");
}

void setup() {
  Serial.begin(115200);
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

  WiFiManager wifiManager;
  wifiManager.autoConnect("Kokubot", "password");
  Serial.println("Connected...");

  if (MDNS.begin("kokubot")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <title>Kokubot Control</title>
        <style>
          body { text-align: center; font-family: Arial, sans-serif; }
          button { padding: 10px; font-size: 16px; margin: 5px; }
        </style>
        <script>
          var socket;
          window.onload = function() {
            socket = new WebSocket("ws://" + window.location.hostname + ":81");
          }
          function sendCommand(cmd) {
            socket.send(cmd);
          }
        </script>
      </head>
      <body>
        <h1>Kokubot Control</h1>
        <button onclick="sendCommand('forward')">Maju</button>
        <button onclick="sendCommand('backward')">Mundur</button>
        <button onclick="sendCommand('right')">Geser Kanan</button>
        <button onclick="sendCommand('left')">Geser Kiri</button>
        <button onclick="sendCommand('turnRight')">Putar Kanan</button>
        <button onclick="sendCommand('turnLeft')">Putar Kiri</button>
        <button onclick="sendCommand('stop')">Berhenti</button>
      </body>
      </html>
    )rawliteral");
  });

  server.onNotFound(notFound);
  AsyncElegantOTA.begin(&server);
  server.begin();
  websockets.begin();
  websockets.onEvent(webSocketEvent);
}

void loop() {
  websockets.loop();
}
