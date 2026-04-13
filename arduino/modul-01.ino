// =====================================================
// PROGRAM LINE FOLLOWER 8 SENSOR - MODUL 1 PRAKTIKUM
// REV: 06 APRIL 2026
// NOTE: Simple belok menggunakan 2 sensor
// =====================================================

// LED Pin
const int pin_led_left = 7;
const int pin_led_middle = 13;
const int pin_led_right = 8;

// PWM output for Motor
const int pin_pwm_right_motor_forward = 9;
const int pin_pwm_left_motor_forward = 6;

// Sensor ANALOG
const int pin_sensor_sayap_kanan_s1 = A4;
const int pin_sensor_sayap_kanan_s2 = A5;
const int pin_sensor_sayap_kanan_s3 = A6;
const int pin_sensor_sayap_kanan_s4 = A7;
const int pin_sensor_sayap_kiri_s5 = A0;
const int pin_sensor_sayap_kiri_s6 = A1;
const int pin_sensor_sayap_kiri_s7 = A2;
const int pin_sensor_sayap_kiri_s8 = A3;

const int threshold = 500;
const int MOTOR_SPEED = 255;
const int TURN_SPEED = 125;

void setMotors(int leftSpeed, int rightSpeed);

void setup() {
  Serial.begin(9600);
  pinMode(pin_led_left, OUTPUT);
  pinMode(pin_led_middle, OUTPUT);
  pinMode(pin_led_right, OUTPUT);
  pinMode(pin_pwm_right_motor_forward, OUTPUT);
  pinMode(pin_pwm_left_motor_forward, OUTPUT);
  setMotors(0, 0);

  Serial.println("=== LINE FOLLOWER WITH 2 SENSORS (S4 & S5) ===");
  Serial.println("System Ready: Place sensors over black line.");
  Serial.println("S4 (Right): A7, S5 (Left): A0");
  Serial.println("Threshold: " + String(threshold));
  Serial.println("==============================================");
}

// ================= MAIN LOOP =================
void loop() {
  // Membaca 8 analog sensors: S1 S2 S3 S4 (right) and S5 S6 S7 S8 (left)
  int s1 = analogRead(pin_sensor_sayap_kanan_s1);
  int s2 = analogRead(pin_sensor_sayap_kanan_s2);
  int s3 = analogRead(pin_sensor_sayap_kanan_s3);
  int s4 = analogRead(pin_sensor_sayap_kanan_s4);

  int s5 = analogRead(pin_sensor_sayap_kiri_s5);
  int s6 = analogRead(pin_sensor_sayap_kiri_s6);
  int s7 = analogRead(pin_sensor_sayap_kiri_s7);
  int s8 = analogRead(pin_sensor_sayap_kiri_s8);

  // Determine if sensors detect black line (above threshold)
  bool s1_black = s1 > threshold;
  bool s2_black = s2 > threshold;
  bool s3_black = s3 > threshold;
  bool s4_black = s4 > threshold;

  bool s5_black = s5 > threshold;
  bool s6_black = s6 > threshold;
  bool s7_black = s7 > threshold;
  bool s8_black = s8 > threshold;

  // Indikator LED
  digitalWrite(pin_led_left, (s6_black || s5_black) ? HIGH : LOW);
  digitalWrite(pin_led_right, (s3_black || s4_black) ? HIGH : LOW);
  digitalWrite(pin_led_middle, (s4_black && s5_black) ? HIGH : LOW);

  // Motor control logic based on sensor readings
  if (s4_black && s5_black) {
    // Kedua sensor S4 dan S5 detek hitam: Jalan maju
    setMotors(MOTOR_SPEED, MOTOR_SPEED);
    Serial.println("STRAIGHT  - Both sensors on line");
  } else if (s4_black && !s5_black) {
    // Jika sensor kanan (S4) detek hitam - Belok Kiri
    setMotors(0, TURN_SPEED); // Right motor forward, left motor stop
    Serial.println("TURN LEFT - Right sensor on line");
  } else if (!s4_black && s5_black) {
    // Jika sensor kiri (S5) detek hitam - Belok Kanan
    setMotors(TURN_SPEED, 0); // Left motor forward, right motor stop
    Serial.println("TURN RIGHT - Left sensor on line");
  } else {
    // Jika sensors S4 dan S5 detek putih: stop
    setMotors(0, 0);
    Serial.println("STOP      - Line lost");
  }

  Serial.print("S4 (Right): ");
  Serial.print(s4);
  Serial.print(" | S5 (Left): ");
  Serial.print(s5);
  Serial.print(" | S4_black: ");
  Serial.print(s4_black ? "YES" : "NO");
  Serial.print(" | S5_black: ");
  Serial.println(s5_black ? "YES" : "NO");

  delay(10);
}

void setMotors(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);
  analogWrite(pin_pwm_left_motor_forward, leftSpeed);
  analogWrite(pin_pwm_right_motor_forward, rightSpeed);
}