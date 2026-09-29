#ifndef CHASSIS_H
#define CHASSIS_H

#define SPEED_MAPPING(x) (x > 0 ? x + 1500 : (x < 0 ? x - 1500 : 0))

void chassis_ctrl_node(void *pvParameters);

#endif