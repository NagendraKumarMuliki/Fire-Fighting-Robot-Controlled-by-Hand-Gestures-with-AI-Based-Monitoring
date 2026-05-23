#include <Servo.h>

// -------- FLAME SENSORS --------
#define LEFT_A    A0
#define RIGHT_A   A1

// -------- SERVOS --------
#define PAN_SERVO_PIN   9
#define TILT1_SERVO_PIN 10
#define TILT2_SERVO_PIN 8

// -------- MOTOR DRIVER --------
#define LEFT_DIR  13
#define LEFT_PWM  11
#define RIGHT_DIR 12
#define RIGHT_PWM 3

// -------- PUMP RELAY --------
#define PUMP_RELAY 2
#define PUMP_ON  LOW
#define PUMP_OFF HIGH

// -------- ULTRASONIC SENSOR --------
#define TRIG_PIN 5
#define ECHO_PIN 4

long duration;

// -------- STAIR CONTROL --------
bool avoidingStairs = false;
unsigned long avoidStartTime = 0;
unsigned long lastStairTime = 0;

const unsigned long BACK_TIME = 300;
const unsigned long STAIR_COOLDOWN = 1000;

// -------- MOTOR POWER --------
int LEFT_POWER  = 160;
int RIGHT_POWER = 160;

// -------- OBJECTS --------
Servo pan, tilt1, tilt2;

// -------- VARIABLES --------
int pos = 90;
int direction = 1;

int flameThreshold = 900;
int deadZone = 40;

String cmd = "S";
unsigned long lastCmdTime = 0;
unsigned long timeout = 1000;

// -------- PUMP VARIABLES --------
bool pumpActive = false;
unsigned long pumpStartTime = 0;
const unsigned long SPRAY_TIME = 2000;

// ======================================================
// MOTOR FUNCTIONS
// ======================================================
void stopMotors() {
  analogWrite(LEFT_PWM, 0);
  analogWrite(RIGHT_PWM, 0);
}

void forward() {
  digitalWrite(LEFT_DIR, LOW);
  digitalWrite(RIGHT_DIR, LOW);
  analogWrite(LEFT_PWM, LEFT_POWER);
  analogWrite(RIGHT_PWM, RIGHT_POWER);
}

void backward() {
  digitalWrite(LEFT_DIR, HIGH);
  digitalWrite(RIGHT_DIR, HIGH);
  analogWrite(LEFT_PWM, LEFT_POWER);
  analogWrite(RIGHT_PWM, RIGHT_POWER);
}

void leftTurn() {
  digitalWrite(LEFT_DIR, HIGH);
  digitalWrite(RIGHT_DIR, LOW);
  analogWrite(LEFT_PWM, LEFT_POWER);
  analogWrite(RIGHT_PWM, RIGHT_POWER);
}

void rightTurn() {
  digitalWrite(LEFT_DIR, LOW);
  digitalWrite(RIGHT_DIR, HIGH);
  analogWrite(LEFT_PWM, LEFT_POWER);
  analogWrite(RIGHT_PWM, RIGHT_POWER);
}

// ======================================================
// PUMP
// ======================================================
void startPump() {
  digitalWrite(PUMP_RELAY, PUMP_ON);
  pumpActive = true;
  pumpStartTime = millis();
}

// ======================================================
// ULTRASONIC
// ======================================================
int readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 20000);

  int d = duration * 0.034 / 2;

  if (d == 0 || d > 100) return -1;
  return d;
}

int getDistance() {
  int d1 = readDistance();
  delay(20);
  int d2 = readDistance();

  if (d1 == -1 && d2 == -1) return -1;
  if (d1 == -1) return d2;
  if (d2 == -1) return d1;

  return (d1 + d2) / 2;
}

// ======================================================
// SETUP
// ======================================================
void setup() {
  Serial.begin(9600);

  pan.attach(PAN_SERVO_PIN);
  tilt1.attach(TILT1_SERVO_PIN);
  tilt2.attach(TILT2_SERVO_PIN);

  pan.write(90);
  tilt1.write(80);
  tilt2.write(80);

  pinMode(LEFT_DIR, OUTPUT);
  pinMode(RIGHT_DIR, OUTPUT);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(RIGHT_PWM, OUTPUT);

  pinMode(PUMP_RELAY, OUTPUT);
  digitalWrite(PUMP_RELAY, PUMP_OFF);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  stopMotors();
}

// ======================================================
// LOOP
// ======================================================
void loop() {

  // -------- SERIAL INPUT --------
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\n');
    cmd.trim();
    lastCmdTime = millis();
  }

  if (millis() - lastCmdTime > timeout) {
    cmd = "S";
  }

  // ======================================================
  // STAIR DETECTION
  // ======================================================
  int dist = getDistance();

  bool stairDetected = (dist > 20 && dist < 80);

  if (stairDetected && millis() - lastStairTime > STAIR_COOLDOWN) {
    avoidingStairs = true;
    avoidStartTime = millis();
    lastStairTime = millis();
  }

  if (avoidingStairs) {
    if (millis() - avoidStartTime < BACK_TIME) {
      backward();
    } else {
      stopMotors();
      avoidingStairs = false;
    }
  }
  else {
    if (cmd == "F") forward();
    else if (cmd == "B") backward();
    else if (cmd == "L") leftTurn();
    else if (cmd == "R") rightTurn();
    else stopMotors();
  }

  // ======================================================
  // PUMP CONTROL
  // ======================================================
  if (cmd == "P" && !pumpActive) startPump();
  else if (cmd == "AUTO_FIRE" && !pumpActive) startPump();
  else if (cmd == "p") {
    digitalWrite(PUMP_RELAY, PUMP_OFF);
    pumpActive = false;
  }

  if (pumpActive && millis() - pumpStartTime >= SPRAY_TIME) {
    digitalWrite(PUMP_RELAY, PUMP_OFF);
    pumpActive = false;
  }

  // ======================================================
  // FIRE TRACKING (Tilt moves ONLY when flame detected)
  // ======================================================
  int leftVal  = analogRead(LEFT_A);
  int rightVal = analogRead(RIGHT_A);

  bool leftFire  = leftVal < flameThreshold;
  bool rightFire = rightVal < flameThreshold;

  // --- NO FIRE: ONLY PAN SWEEPS ---
  if (!leftFire && !rightFire) {

    pos += direction;

    if (pos >= 160) direction = -1;
    if (pos <= 20)  direction = 1;

    pan.write(pos);

    // Tilt stays fixed
    tilt1.write(80);
    tilt2.write(80);
  }

  // --- FIRE CENTERED ---
  else if (abs(leftVal - rightVal) < deadZone) {

    int t1 = constrain(pos, 40, 140);
    int t2 = constrain(pos, 40, 120);

    pan.write(pos);
    tilt1.write(t1);
    tilt2.write(t2);
  }

  // --- FIRE LEFT OR RIGHT ---
  else {

    if (leftVal < rightVal) pos -= 2;
    else pos += 2;

    pos = constrain(pos, 20, 160);

    int t1 = constrain(pos, 40, 140);
    int t2 = constrain(pos, 40, 120);

    pan.write(pos);
    tilt1.write(t1);
    tilt2.write(t2);
  }

  delay(20);
}