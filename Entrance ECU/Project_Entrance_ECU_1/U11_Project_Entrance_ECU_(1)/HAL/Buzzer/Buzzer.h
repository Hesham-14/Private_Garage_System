
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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Macros.h"
#include "Standard_Types.h"


/*****************************************
---------   Pre-build Configure   --------
*****************************************/
#define Buzzer_Pin					(PINC0)
#define Buzzer_Port					(PORTC)
#define Buzzer_DDR					(DDRC)



/************************
--------  APIs  --------
*************************/

void Buzzer_Init(void);

void Buzzer_Start(void);

#endif
