#include <Arduino.h>

// ===== TB6612FNG pins (change if needed) =====
#define STBY_PIN 10

// Motor A (Left)
#define AIN1 12
#define AIN2 13
#define PWMA 11  // PWM pin

// Motor B (Right)
#define BIN1 15
#define BIN2 16
#define PWMB 14  // PWM pin

// ===== PWM config =====
const uint32_t PWM_FREQ = 20000; // 20kHz (quiet)
const uint8_t  PWM_RES  = 8;     // 8-bit => duty 0..255

static inline void pwmWritePin(uint8_t pin, uint32_t duty) {
  // duty must match resolution: for 8-bit, 0..255
  ledcWrite(pin, duty);
}

void motorWrite(int in1, int in2, int pwmPin, int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    pwmWritePin(pwmPin, (uint32_t)speed);
  } else if (speed < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    pwmWritePin(pwmPin, (uint32_t)(-speed));
  } else {
    // Coast
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    pwmWritePin(pwmPin, 0);
  }
}

void setMotors(int left, int right) {
  digitalWrite(STBY_PIN, HIGH);
  motorWrite(AIN1, AIN2, PWMA, left);
  motorWrite(BIN1, BIN2, PWMB, right);
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

  // NEW ESP32 core 3.x LEDC API:
  // ledcAttach(pin, freq, resolution)
  if (!ledcAttach(PWMA, PWM_FREQ, PWM_RES)) Serial.println("LEDC attach failed for PWMA");
  if (!ledcAttach(PWMB, PWM_FREQ, PWM_RES)) Serial.println("LEDC attach failed for PWMB");

  stopMotors(200);
  Serial.println("TB6612FNG + N20 motor test start");
}

void loop() {
  Serial.println("Forward");
  setMotors(180, 180);
  delay(2000);
  stopMotors();

  Serial.println("Reverse");
  setMotors(-180, -180);
  delay(2000);
  stopMotors();

  Serial.println("Spin Left");
  setMotors(-180, 180);
  delay(2000);
  stopMotors();

  Serial.println("Spin Right");
  setMotors(180, -180);
  delay(2000);
  stopMotors();

  Serial.println("Ramp Up");
  for (int s = 0; s <= 255; s += 15) {
    setMotors(s, s);
    delay(150);
  }

  Serial.println("Ramp Down");
  for (int s = 255; s >= 0; s -= 15) {
    setMotors(s, s);
    delay(150);
  }

  stopMotors(800);
}
