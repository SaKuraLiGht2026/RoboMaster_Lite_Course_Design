#include <stdint.h>
#include <Adafruit_PWMServoDriver.h>
#include "HardwareSerial.h"
#include <Arduino_FreeRTOS.h>
#include "chassis.h"
#include <Arduino.h>
#include "Robot_Config.hpp"
#include "drv_motor.h"

extern bool                    IsAttack;
extern bool                    IsWall;

extern int8_t                  IsFire;

extern int                     chassis_speed_x;
extern int                     chassis_speed_y;
extern int                     chassis_omega;

void chassis_ctrl_node(void *pvParameters){
  uint64_t action_tick = 0;
  uint8_t  action_flag = 2;

  chassis_speed_x = 0;
  chassis_speed_y = 0;
  chassis_omega   = 0;
  
  chassis_motor_init();

  for (;;) {
    action_tick++;
  
    if(action_flag == 2){
      chassis_speed_x = 2000;
      chassis_speed_y = 0;
      chassis_omega   = 0;

      if(action_tick > 100){
        if(IsWall){
          action_flag += 1;
          action_tick = 0;
        }
      }
    }else if (action_flag == 3){
      chassis_speed_x = 0;
      chassis_speed_y = 0;
      chassis_omega   = -2000;

      if(action_tick > 26){
        action_flag += 1;
        action_tick = 0;
      }
    }else if (action_flag == 4){
      chassis_speed_x = 1800;
      chassis_speed_y = 0;
      chassis_omega   = 0;

      if(action_tick > 110){
        action_flag += 1;
        action_tick = 0;
      }
    }else if(action_flag == 5){
      chassis_speed_x = 0;
      chassis_speed_y = 0;
      chassis_omega   = -2000;

      if(action_tick > 23){
        action_flag += 1;
        action_tick = 0;
      }
    }else if(action_flag == 6){
      chassis_speed_x = 1800;
      chassis_speed_y = 0;
      chassis_omega   = 0;

      if(action_tick > 100){
        action_flag += 1;
        action_tick = 0;
      }
    }else if(action_flag == 7){
      chassis_speed_x = 0;
      chassis_speed_y = 0;
      chassis_omega   = 0;

      IsFire          = 1;
    }
    else{
      chassis_speed_x = 0;
      chassis_speed_y = 0;
      chassis_omega   = 0;
    }
    
    

    if(IsAttack){
      chassis_calc(0, 0, 0);
    }else{      
      chassis_calc(SPEED_MAPPING(chassis_speed_x), 
                   SPEED_MAPPING(chassis_speed_y), 
                   SPEED_MAPPING(chassis_omega));
    }

    motor_driver();
    
    vTaskDelay(1);
  }
}
