#include <Arduino.h>

// ===== L298N pins (change if needed) =====
// Motor A
#define ENA 11   // PWM pin
#define IN1 12
#define IN2 13

// Motor B
#define ENB 14   // PWM pin
#define IN3 15
#define IN4 16

// ===== PWM config =====
const uint32_t PWM_FREQ = 20000; // 20kHz
const uint8_t  PWM_RES  = 8;     // duty: 0..255

static inline void pwmWritePin(uint8_t pin, uint32_t duty) {
  ledcWrite(pin, duty); // ESP32 core 3.x: write by pin
}

// speed: -255..255
void motorA(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {                // forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    pwmWritePin(ENA, speed);
  } else if (speed < 0) {         // reverse
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    pwmWritePin(ENA, -speed);
  } else {                        // stop (coast)
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    pwmWritePin(ENA, 0);
  }
}

void motorB(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {                // forward
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    pwmWritePin(ENB, speed);
  } else if (speed < 0) {         // reverse
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    pwmWritePin(ENB, -speed);
  } else {                        // stop (coast)
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    pwmWritePin(ENB, 0);
  }
}

void setMotors(int left, int right) {
  motorA(left);   // left motor on OUT1/OUT2
  motorB(right);  // right motor on OUT3/OUT4
}

void stopMotors(int ms = 300) {
  setMotors(0, 0);
  delay(ms);
}

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Attach PWM to ENA/ENB (ESP32 core 3.x)
  if (!ledcAttach(ENA, PWM_FREQ, PWM_RES)) Serial.println("LEDC attach failed for ENA");
  if (!ledcAttach(ENB, PWM_FREQ, PWM_RES)) Serial.println("LEDC attach failed for ENB");

  stopMotors(200);
  Serial.println("L298N + N20 motor test start");
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
  for (int s = 0; s <= 255; s += 1) {
    setMotors(s, s);
    delay(15);
  }

  Serial.println("Ramp Down");
  for (int s = 255; s >= 0; s -= 1) {
    setMotors(s, s);
    delay(15);
  }

  stopMotors(800);
}

//you can change the ramp up speed and delays
