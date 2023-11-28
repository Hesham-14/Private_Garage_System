/*
 * Servo_Motor.h
 *
 *  Created on: Oct 9, 2023
 *      Author: hesham mohamed
 */

#ifndef INC_GATE_H_
#define INC_GATE_H_

#include "STM32F103x6.h"
#include "STM32F103C6_GPIO_Driver.h"
#include "STM32F103C6_TIMER_Driver.h"

//Direction of motion
#define UP   1
#define DOWN 2

void Servo1_Entry_Gate_Init(void);
void Servo1_Entry_Gate(uint8_t Direction);

void Servo2_Exit_Gate_Init(void);
void Servo2_Exit_Gate(uint8_t Direction);

#endif /* INC_GATE_H_ */
