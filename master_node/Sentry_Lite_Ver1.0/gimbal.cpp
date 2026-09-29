#include <Arduino_FreeRTOS.h>
#include "pins_arduino.h"
#include <Arduino.h>
#include "gimbal.h"
#include "Robot_Config.hpp"
#include "HardwareSerial.h"
#include <Servo.h>

extern Servo            yaw_servo;
extern Servo            pitch_servo;

extern bool             IsAttack;
extern int8_t           IsFire;

extern int16_t          yaw_offset;
extern int16_t          pitch_offset;

extern int              yaw_angle;
extern int              pitch_angle;

void gimbal_ctrl_node(void *pvParameters){
  bool IsYawRising    = false;
  bool IsPitchRising  = false;
  for(;;){
    if(IsAttack == false){
      if(yaw_angle >  YAW_WORKRANGE / 2) IsYawRising = false;
      if(yaw_angle < -YAW_WORKRANGE / 2) IsYawRising = true;

      if(IsYawRising) yaw_angle++;
      else            yaw_angle--;

      if(pitch_angle >  PITCH_WORKRANGE / 2) IsPitchRising = false;
      if(pitch_angle < -PITCH_WORKRANGE / 2) IsPitchRising = true;

      if(IsPitchRising) pitch_angle++;
      else              pitch_angle--;
      
    }else{
      yaw_angle   += yaw_offset;
      pitch_angle += pitch_offset;

      if(yaw_angle >  YAW_WORKRANGE / 2) yaw_angle =  YAW_WORKRANGE / 2;
      if(yaw_angle < -YAW_WORKRANGE / 2) yaw_angle = -YAW_WORKRANGE / 2;

      if(pitch_angle >  PITCH_WORKRANGE / 2) pitch_angle =  PITCH_WORKRANGE / 2;
      if(pitch_angle < -PITCH_WORKRANGE / 2) pitch_angle = -PITCH_WORKRANGE / 2;
    }

    if(IsFire != 0){
      digitalWrite(AMMO_BOOSTER, HIGH);
    }else{
      digitalWrite(AMMO_BOOSTER, LOW);
    }

    yaw_servo.write(yaw_angle + YAW_ANGLE_INIT);
    pitch_servo.write(pitch_angle + PITCH_ANGLE_INIT);

    vTaskDelay(1);
  }
}
