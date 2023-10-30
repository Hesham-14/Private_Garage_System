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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Macros.h"
#include "Standard_Types.h"

/*****************************************
---------   Pre-build Configure   --------
*****************************************/
#define Motion_Sensor_Pin					(PC5)
#define Motion_Sensor_DDR					(DDRC)
#define Motion_Sensor_Port					(PINC)

/************************
--------  APIs  --------
*************************/

void Motion_Sensor_Init(void);
u8 Motion_Sensor_Get_State(void);

#endif
