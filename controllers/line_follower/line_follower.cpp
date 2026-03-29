#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>

#include <iostream>

using namespace webots;
using namespace std;

#define TIME_STEP 32

int main() {

  Robot *robot = new Robot();

  double max_speed = 3.28;
  double base_speed = 2.8;
  
  cout << "hello v2\n";

  // ================= MOTOR =================
  Motor *left_motor = robot->getMotor("left wheel motor");
  Motor *right_motor = robot->getMotor("right wheel motor");

  left_motor->setPosition(INFINITY);
  right_motor->setPosition(INFINITY);

  left_motor->setVelocity(0);
  right_motor->setVelocity(0);

  // ================= SENSOR =================
  const char *ir_names[8] = {"IR7","IR6","IR5","IR4","IR3","IR2","IR1","IR0"};
  DistanceSensor *ir[8];

  for(int i=0;i<8;i++){
    ir[i] = robot->getDistanceSensor(ir_names[i]);
    ir[i]->enable(TIME_STEP);
  }

  // posisi sensor kiri → kanan
  int weight[8] = {-7,-5,-3,-1,1,3,5,7};

  // ================= PID =================
  double Kp = 0.8;
  double Ki = 0.0;
  double Kd = 2.0;

  double error = 0;
  double last_error = 0;
  double integral = 0;

  while(robot->step(TIME_STEP) != -1){

    double numerator = 0;
    double denominator = 0;

    double sensor_raw[8];
    int line[8];

    // ================= BACA SENSOR =================
    for(int i=0;i<8;i++){

      sensor_raw[i] = ir[i]->getValue();

      // threshold (dari data kamu: putih≈90 hitam≈450)
      if(sensor_raw[i] > 200)
        line[i] = 1; // hitam = garis
      else
        line[i] = 0;

      numerator += line[i] * weight[i];
      denominator += line[i];
    }

    // posisi garis
    if(denominator != 0)
      error = numerator / denominator;

    // ================= PID =================
    integral += error;
    double derivative = error - last_error;

    double correction =
      (Kp * error) +
      (Ki * integral) +
      (Kd * derivative);

    last_error = error;

    // arah motor diperbaiki supaya mengikuti garis
    double left_speed  = base_speed + correction;
    double right_speed = base_speed - correction;

    // limit motor
    if(left_speed > max_speed) left_speed = max_speed;
    if(left_speed < -max_speed) left_speed = -max_speed;

    if(right_speed > max_speed) right_speed = max_speed;
    if(right_speed < -max_speed) right_speed = -max_speed;

    left_motor->setVelocity(left_speed);
    right_motor->setVelocity(right_speed);

    // ================= DEBUG =================
    cout << "IR: ";
    for(int i=0;i<8;i++)
      cout << sensor_raw[i] << " ";

    cout << "| LINE: ";
    for(int i=0;i<8;i++)
      cout << line[i] << " ";

    cout << "| L:" << left_speed
         << " R:" << right_speed
         << " Error:" << error << endl;
  }

  delete robot;
  return 0;
}