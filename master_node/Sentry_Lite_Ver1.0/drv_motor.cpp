#include "drv_motor.h"
#include <Adafruit_PWMServoDriver.h>

extern Adafruit_PWMServoDriver chassis_motor;

Drv_motor_Info_t Chassis_Motor[4];

void chassis_motor_init() {
  Chassis_Motor[0].id    = 1;
  Chassis_Motor[0].speed = 0;

  Chassis_Motor[1].id    = 2;
  Chassis_Motor[1].speed = 0;

  Chassis_Motor[2].id    = 3;
  Chassis_Motor[2].speed = 0;

  Chassis_Motor[3].id    = 4;
  Chassis_Motor[3].speed = 0;

  chassis_motor.writeMicroseconds( 3, 0);
  chassis_motor.writeMicroseconds( 4, 0);
  chassis_motor.writeMicroseconds( 5, 0);
  chassis_motor.writeMicroseconds( 6, 0);
  chassis_motor.writeMicroseconds( 9, 0);
  chassis_motor.writeMicroseconds(10, 0);
  chassis_motor.writeMicroseconds(11, 0);
  chassis_motor.writeMicroseconds(12, 0);
}

void set_motor_speed(int8_t id, int16_t speed){
  if(id == 1){
    if(fabs(speed) >= 1500 && fabs(speed) < 4096){
      Chassis_Motor[0].speed = speed;
    }else{
      return;
    }
  }else if(id == 2){
    if(fabs(speed) >= 1500 && fabs(speed) < 4096){
      Chassis_Motor[1].speed = speed;
    }else{
      return;
    }
  }else if(id == 3){
    if(fabs(speed) >= 1500 && fabs(speed) < 4096){
      Chassis_Motor[2].speed = speed;
    }else{
      return;
    }
  }else if(id == 4){
    if(fabs(speed) >= 1500 && fabs(speed) < 4096){
      Chassis_Motor[3].speed = speed;
    }else{
      return;
    }
  }else{
    return;
  }
}

void motor_driver(){
  if(Chassis_Motor[0].speed > 0){
    chassis_motor.writeMicroseconds( 3, set_speed_pwm(Chassis_Motor[0].speed));//1500~2000
    chassis_motor.writeMicroseconds( 4, 0);
  }else{
    chassis_motor.writeMicroseconds( 3, 0);//1500~2000
    chassis_motor.writeMicroseconds( 4, set_speed_pwm(Chassis_Motor[0].speed));
  }
  if(Chassis_Motor[1].speed > 0){
    chassis_motor.writeMicroseconds( 5, set_speed_pwm(Chassis_Motor[1].speed));//1500~2000
    chassis_motor.writeMicroseconds( 6, 0);
  }else{
    chassis_motor.writeMicroseconds( 5, 0);//1500~2000
    chassis_motor.writeMicroseconds( 6, set_speed_pwm(Chassis_Motor[1].speed));
  }
  if(Chassis_Motor[2].speed > 0){
    chassis_motor.writeMicroseconds( 9, set_speed_pwm(Chassis_Motor[2].speed));//1500~2000
    chassis_motor.writeMicroseconds(10, 0);
  }else{
    chassis_motor.writeMicroseconds( 9, 0);//1500~2000
    chassis_motor.writeMicroseconds(10, set_speed_pwm(Chassis_Motor[2].speed));
  }
  if(Chassis_Motor[3].speed > 0){
    chassis_motor.writeMicroseconds(11, set_speed_pwm(Chassis_Motor[3].speed));//1500~2000
    chassis_motor.writeMicroseconds(12, 0);
  }else{
    chassis_motor.writeMicroseconds(11, 0);//1500~2000
    chassis_motor.writeMicroseconds(12, set_speed_pwm(Chassis_Motor[3].speed));
  }
}

void chassis_calc(int vel_x, int vel_y, int omega){
  Chassis_Motor[0].speed = vel_x + vel_y + omega;
  Chassis_Motor[1].speed = vel_x - vel_y + omega;
  Chassis_Motor[2].speed = vel_x - vel_y - omega;
  Chassis_Motor[3].speed = vel_x + vel_y - omega;

  set_motor_speed(1, Chassis_Motor[0].speed);
  set_motor_speed(2, Chassis_Motor[1].speed);
  set_motor_speed(3, Chassis_Motor[2].speed);
  set_motor_speed(4, Chassis_Motor[3].speed);
}
