#include <Arduino.h>

// ===== TB6612FNG pins (change if needed) =====
#define STBY_PIN 10

// Motor A (Left)
#define AIN1 12
#define AIN2 13
#define PWMA 11  // PWM

// Motor B (Right)
#define BIN1 15
#define BIN2 16
#define PWMB 14  // PWM

// ===== PWM config =====
const int PWM_FREQ = 20000;     // 20 kHz (quiet)
const int PWM_RES  = 8;         // 8-bit: 0..255
const int CH_A = 0;
const int CH_B = 1;

void motorWrite(int in1, int in2, int pwmChannel, int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    ledcWrite(pwmChannel, speed);
  } else if (speed < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    ledcWrite(pwmChannel, -speed);
  } else {
    // Coast (free run). For brake, set both HIGH.
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    ledcWrite(pwmChannel, 0);
  }
}

void setMotors(int left, int right) {
  digitalWrite(STBY_PIN, HIGH); // enable driver
  motorWrite(AIN1, AIN2, CH_A, left);
  motorWrite(BIN1, BIN2, CH_B, right);
}

void stopMotors(int ms = 300) {
  setMotors(0, 0);
  delay(ms);
}

void setup() {
  Serial.begin(115200);

  pinMode(STBY_PIN, OUTPUT);

  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);

  // PWM setup
  ledcSetup(CH_A, PWM_FREQ, PWM_RES);
  ledcSetup(CH_B, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWMA, CH_A);
  ledcAttachPin(PWMB, CH_B);

  stopMotors(200);
  Serial.println("TB6612FNG + N20 motor test start");
}

void loop() {
  // 1) Forward
  Serial.println("Forward");
  setMotors(180, 180);
  delay(2000);
  stopMotors();

  // 2) Reverse
  Serial.println("Reverse");
  setMotors(-180, -180);
  delay(2000);
  stopMotors();

  // 3) Spin left (left motor reverse, right motor forward)
  Serial.println("Spin Left");
  setMotors(-180, 180);
  delay(2000);
  stopMotors();

  // 4) Spin right
  Serial.println("Spin Right");
  setMotors(180, -180);
  delay(2000);
  stopMotors();

  // 5) Ramp up/down forward
  Serial.println("Ramp Up");
  for (int s = 0; s <= 255; s += 15) {
    setMotors(s, s);
    delay(200);
  }
  Serial.println("Ramp Down");
  for (int s = 255; s >= 0; s -= 15) {
    setMotors(s, s);
    delay(200);
  }
  stopMotors(800);
}
