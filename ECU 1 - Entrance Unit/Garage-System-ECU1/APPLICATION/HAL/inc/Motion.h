/*
 * Motion.h
 *
 * Created: 10/23/2023 10:19:59 PM
 *  Author: hesham mohamed
 */ 

#ifndef MOTION_H_
#define MOTION_H_

/*****************************************
----------   Include Files    ------------
*****************************************/
#include "Standard_Types.h"
#include "STM32F103C6_GPIO_Driver.h"
#include "STM32F103x6.h"

/*****************************************
---------   Pre-build Configure   --------
*****************************************/
#define Motion_Sensor_Pin			(GPIO_PIN_11)
#define Motion_Sensor_Port			(GPIOB)

/************************
--------  APIs  --------
*************************/

void Motion_Sensor_Init(void);
u8 Motion_Sensor_Get_State(void);

#endif
