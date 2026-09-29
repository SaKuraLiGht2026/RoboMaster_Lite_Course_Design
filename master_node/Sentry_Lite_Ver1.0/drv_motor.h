#ifndef DRV_MOTOR_H
#define DRV_MOTOR_H

#include <stdint.h>

#define set_speed_pwm(x) (x >= 0 ? x : -x)

typedef struct{
  int8_t  id;
  int16_t speed;
}Drv_motor_Info_t;

void chassis_motor_init();

void set_motor_speed(int8_t id, int16_t speed);

void motor_driver();

void chassis_calc(int vel_x, int vel_y, int omega);

#endif