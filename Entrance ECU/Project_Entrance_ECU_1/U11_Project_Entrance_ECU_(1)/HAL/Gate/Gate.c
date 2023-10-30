
/*
 * Gate.c
 *
 * Created: 10/23/2023 12:08:41 AM
 *  Author: hesham mohamed
 */ 

/*****************************************
-----------     Includes     -------------
*****************************************/
#include "Gate.h"


/*****************************************
-----------   ISR Functions  -------------
*****************************************/
static void TIMER0_CompareMatch_PWM_CallBack(void)
{
	Clear_Bit(PORTB, PINB3);
	Toggle_Bit(PORTD, PIND6);
}

static void TIMER0_Overflow_PWM_CallBack(void)
{
	Toggle_Bit(PORTD, PIND7);
}

/**************************************
------------  Functions  --------------
***************************************/
void Gate_Init()
{
	//Configure PIND 1&2 as Output
	//MCAL_PIN_Direction(DIOD, DIO_PIN0, OUTPUT_PIN);
	Set_Bit(DDRD, PIND6);
	Set_Bit(DDRD, PIND7);
	
	//set timer configuration
	S_TIMER0_config_t TIMER0_PWM_cfg;
	TIMER0_PWM_cfg.mode = FastPWM;
	TIMER0_PWM_cfg.Compare_Match_Mode = INVERTING_SET;
	TIMER0_PWM_cfg.clk = PRESCALING_CLK64;
	TIMER0_PWM_cfg.ovfInterrupt = TOVFIE_ENABLE;
	TIMER0_PWM_cfg.ocmInterrupt = OCIE_ENABLE;
	TIMER0_CALLBACK_Overflow_INTERRUPT(TIMER0_Overflow_PWM_CallBack);
	TIMER0_CALLBACK_CompareMatch_INTERRUPT(TIMER0_CompareMatch_PWM_CallBack);
	MCAL_TIMER0_Init(&TIMER0_PWM_cfg);
	MCAL_PWM_DutyCycle_2(127);
	
	LCD_clear_screen();
	LCD_write_string("Gate READY ");
	_delay_ms(500);
	
	//init other HALs
	Buzzer_Init();
	LCD_clear_screen();
	LCD_write_string("Buzzer READY ");
	_delay_ms(500);
	
	Motion_Sensor_Init();
	LCD_clear_screen();
	LCD_write_string("Motion Sensor");
	LCD_GOTO(2,0);
	LCD_write_string("Ready");
	_delay_ms(500);
}
void Gate_Open(void)
{
	MCAL_PWM_DutyCycle_2(0);
}
void Gate_Close(void)
{
	MCAL_PWM_DutyCycle_2(127);
}
/*****************************************
------------  ISR Functions  -------------
*****************************************/
