
/*
 * Buzzer.h
 *
 * Created: 10/23/2023 9:43:34 PM
 *  Author: hesham mohamed
 */ 

#ifndef BUZZER_H_
#define BUZZER_H_

/*****************************************
----------   Include Files    ------------
*****************************************/
#include "Standard_Types.h"
#include "STM32F103C6_GPIO_Driver.h"
#include "STM32F103x6.h"


/*****************************************
---------   Pre-build Configure   --------
*****************************************/
#define Buzzer_Pin					(GPIO_PIN_10)
#define Buzzer_Port					(GPIOB)


/************************
--------  APIs  --------
*************************/

void Buzzer_Init(void);

void Buzzer_Start(void);

#endif
