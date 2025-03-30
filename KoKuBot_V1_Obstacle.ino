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

// ---------------- Pin Ultrasonik ----------------
#define TRIG_PIN 5  // Pin Trigger
#define ECHO_PIN 18 // Pin Echo

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

// ---------------- Fungsi Sensor Ultrasonik ----------------
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // Konversi ke cm
  return distance;
}

void setup() {
  Serial.begin(115200);
  
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
  
  // Konfigurasi pin sensor ultrasonik
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  float distance = getDistance();
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  if (distance < 20) { // Jika ada rintangan dalam jarak 20 cm
    Serial.println("Rintangan terdeteksi! Mundur...");
    moveBackward();
    delay(1000);
    stopMotors();
    delay(500);
  } else {
    Serial.println("Jalan terus...");
    moveForward();
  }
  
  delay(100);
}
