#include "pins_arduino.h"
#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include "Robot_Config.hpp"

const int TrigPin = 7;
const int EchoPin = 8;

extern bool  IsWall;

extern float cm;
extern float cm_last;

void radar_node(void *pvParameters){
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  for(;;){
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);

    cm = pulseIn(EchoPin, HIGH) / 58.0;
    cm = (int(cm * 100.0)) / 100.0;

    cm = 0.1 * cm + 0.9 * cm_last;

    if(cm < 36) {
      digitalWrite(LED_BUILTIN, LOW);
      IsWall = true;
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
      IsWall = false;
    }

    cm_last = cm;

    vTaskDelay(1);
  }
}
