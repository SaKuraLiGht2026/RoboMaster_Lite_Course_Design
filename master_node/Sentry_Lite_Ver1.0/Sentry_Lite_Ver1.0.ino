#include <PS2X_lib.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arduino_FreeRTOS.h>
#include "Robot_Config.hpp"
#include "chassis.h"
#include "gimbal.h"
#include "fire.h"
#include <Arduino.h>

#define Head_Frame 0xAA
#define Tail_Frame 0x55

#define PKT_LEN    7

uint8_t win[PKT_LEN];     

int8_t  IsFire = 0;
int16_t yaw_offset = 0;
int16_t pitch_offset = 0;

float   cm = 0.0f;
float   cm_last = 0.0f;

bool    pktReady = false;

bool    IsWall   = false;

void master_node(void *pvParameters);

Adafruit_PWMServoDriver chassis_motor = Adafruit_PWMServoDriver(0x60);

PS2X                    ps2x;

Servo                   yaw_servo;
Servo                   pitch_servo;

bool                    IsAttack = false;

int                     yaw_angle   = 0;
int                     pitch_angle = 0;

int                     chassis_speed_x = 0;
int                     chassis_speed_y = 0;
int                     chassis_omega   = 0;

void feedByte(uint8_t b) {
  for (uint8_t i = 0; i < PKT_LEN - 1; i++) win[i] = win[i + 1];
  win[PKT_LEN - 1] = b;

  if (win[0] == Head_Frame && win[PKT_LEN - 1] == Tail_Frame) {
    IsFire = win[1];
    yaw_offset = win[2] | (win[3] << 8);
    pitch_offset = win[4] | (win[5] << 8);
    pktReady = true;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);              

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AMMO_BOOSTER, OUTPUT);

  yaw_servo.attach(5);
  pitch_servo.attach(6);
  
  chassis_motor.begin();
  chassis_motor.setPWMFreq(50);

  xTaskCreate(
    chassis_ctrl_node,          // 任务函数
    "chassis_ctrl_node",        // 任务名称
    128,                        // 堆栈大小
    NULL,                       // 参数
    1,                          // 优先级
    NULL                        // 任务句柄
  );

  xTaskCreate(
    gimbal_ctrl_node,           // 任务函数
    "gimbal_ctrl_node",         // 任务名称
    128,                        // 堆栈大小
    NULL,                       // 参数
    1,                          // 优先级
    NULL                        // 任务句柄
  );

  xTaskCreate(
    radar_node,                 // 任务函数
    "radar_node",               // 任务名称
    128,                        // 堆栈大小
    NULL,                       // 参数
    1,                          // 优先级
    NULL                        // 任务句柄
  );

  xTaskCreate(
    master_node,                // 任务函数
    "master_node",              // 任务名称
    128,                        // 堆栈大小
    NULL,                       // 参数
    1,                          // 优先级
    NULL                        // 任务句柄
  );
  
  // 启动调度器
  vTaskStartScheduler();
}

void master_node(void *pvParameters){
  uint64_t uart_tick    = 0;
  bool     uart_timeout = false;

  for(;;){
    uart_tick++;
    if(Serial.available() > 0){
      feedByte(Serial.read()); 
      uart_tick = 0;
      uart_timeout = false;
    }

    if(uart_tick > 30){
      uart_tick = 30;
      uart_timeout = true;
    }

    if(uart_timeout){
      IsFire = 0;
      yaw_offset = 0;
      pitch_offset = 0;
    }

    if(IsFire != 0){
      IsAttack = true;
    }else{
      IsAttack = false;
    }
    
    vTaskDelay(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
