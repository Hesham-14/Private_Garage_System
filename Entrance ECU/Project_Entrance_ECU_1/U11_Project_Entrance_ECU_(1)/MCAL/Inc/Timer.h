/*
 * Timer.h
 *
 * Created: 7/16/2023 7:16:27 PM
 *  Author: MONS
 */ 


#ifndef TIMER_H_
#define TIMER_H_


/******************************************************************************
*                               INCLUDES			                          *
*******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Macros.h"
#include "Standard_Types.h"

/******************************************************************************
*									Macros	                				  *
*******************************************************************************/
#define DIOB_PORT   PORTD
#define DIOB_DDR    DDRD
#define DIOB_PIN_3    PD3

/******************************************************************************
*									ENUMS	                				  *
*******************************************************************************/
typedef enum
{
	Normal,
	CTC = 0x08,
	PWM = 0x40,
	FastPWM = 0x48
}TIMER0_MODE_t;


typedef enum{
	NORMAL_PORT_OP = ~(1<<4)|~(1<<5),
	TOGGLE_OC0_CM = (1<<4),
	NONINVERTING_CLEAR= (1<<5),
	INVERTING_SET = (1<<4)|(1<<5),
}Compare_Mode_t;

typedef enum
{
	OCIE_DISABLE = 0x00,
	OCIE_ENABLE = 0x02
}OCMIE0_t;

typedef enum
{
	TOVFIE_DISABLE = 0x00,
	TOVFIE_ENABLE = 0x01
}TOVFIE0_t;


typedef enum
{
	NO_CLK_SRC,
	NO_PRESCALING,
	PRESCALING_CLK8,
	PRESCALING_CLK64,
	PRESCALING_CLK256,
	PRESCALING_CLK1024,
	EXT_CLK_FALLING,
	EXT_CLK_RISING,
}TIMER0_CLOCK_t;


/******************************************************************************
*									STRUCT	                				  *
*******************************************************************************/
typedef struct
{
	TIMER0_MODE_t		mode;
	Compare_Mode_t		Compare_Match_Mode;
	OCMIE0_t			ocmInterrupt;
	TOVFIE0_t			ovfInterrupt;
	TIMER0_CLOCK_t		clk;
}S_TIMER0_config_t;

/******************************************************************************
*									APIS				                      *
*******************************************************************************/
E_STATUS_t MCAL_TIMER0_Init(S_TIMER0_config_t* timer0_cfg);
E_STATUS_t MCAL_TIMER0_Stop();

E_STATUS_t MCAL_TIMER0_GetCompareValue(uint8_t* pu8_TicksNumber);
E_STATUS_t MCAL_TIMER0_SetCompareValue(uint8_t u8_TicksNumber);

E_STATUS_t MCAL_TIMER0_GetCounterValue(uint8_t* pu8_TicksNumber);
E_STATUS_t MCAL_TIMER0_SetCounterValue(uint8_t u8_TicksNumber);

E_STATUS_t MCAL_TIMER0_GetOverflowValue(uint8_t* pu8_TicksNumber);
E_STATUS_t MCAL_TIMER0_SetOverflowValue(uint8_t u8_TicksNumber);

E_STATUS_t MCAL_PWM_DutyCycle(uint8_t Duty_Cycle);

void TIMER0_CALLBACK_Overflow_INTERRUPT(PTR_VoidFuncVoid_t callback);
void TIMER0_CALLBACK_CompareMatch_INTERRUPT(PTR_VoidFuncVoid_t callback);




#endif /* TIMER_H_ */