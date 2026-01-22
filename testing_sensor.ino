// ==================================================
// ESP32-S3 | 12-Channel Visible Light Sensor Test
// ==================================================

// ---------- Select pins ----------
#define SEL_A 5
#define SEL_B 6

// ---------- Analog input pins ----------
#define IN_A 1
#define IN_B 2
#define IN_C 3
#define IN_D 4

#define SENSOR_COUNT 12

int sensorADC[SENSOR_COUNT];

// --------------------------------------------------
// Read all 12 sensors using the internal multiplexer
// --------------------------------------------------
void readSensors() {

  // -------- Select = 00 --------
  digitalWrite(SEL_A, LOW);
  digitalWrite(SEL_B, LOW);
  delayMicroseconds(5);

  sensorADC[6] = analogRead(IN_A);
  sensorADC[4] = analogRead(IN_C);
  sensorADC[1] = analogRead(IN_D);

  // -------- Select = 01 --------
  digitalWrite(SEL_A, LOW);
  digitalWrite(SEL_B, HIGH);
  delayMicroseconds(5);

  sensorADC[7]  = analogRead(IN_A);
  sensorADC[11] = analogRead(IN_B);
  sensorADC[2]  = analogRead(IN_C);
  sensorADC[0]  = analogRead(IN_D);

  // -------- Select = 10 --------
  digitalWrite(SEL_A, HIGH);
  digitalWrite(SEL_B, LOW);
  delayMicroseconds(5);

  sensorADC[9]  = analogRead(IN_A);
  sensorADC[10] = analogRead(IN_B);
  sensorADC[3]  = analogRead(IN_C);

  // -------- Select = 11 --------
  digitalWrite(SEL_A, HIGH);
  digitalWrite(SEL_B, HIGH);
  delayMicroseconds(5);

  sensorADC[8] = analogRead(IN_A);
  sensorADC[5] = analogRead(IN_C);
}

// --------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(SEL_A, OUTPUT);
  pinMode(SEL_B, OUTPUT);

  // ESP32 ADC configuration
  analogReadResolution(12);           // 0–4095
  analogSetAttenuation(ADC_11db);     // up to ~3.3V

  Serial.println();
  Serial.println("ESP32-S3 | 12-Channel Sensor Test");
  Serial.println("Index: 0 1 2 3 4 5 6 7 8 9 10 11");
  Serial.println("--------------------------------");
}

// --------------------------------------------------
void loop() {
  readSensors();

  for (int i = 0; i < SENSOR_COUNT; i++) {
    Serial.print(sensorADC[i]);
    Serial.print('\t');
  }
  Serial.println();

  delay(200);
}
