/*
 * Timer.c
 *
 * Created: 7/16/2023 7:16:37 PM
 *  Author: MONS
 */ 

#include "Timer.h"

/******************************************************************************
*                               GLOBAL & EXTERN VARIABLES                     *
*******************************************************************************/
PTR_VoidFuncVoid_t CALLBACK_OVF;
PTR_VoidFuncVoid_t CALLBACK_CM;
uint8_t u8_TOVNumber;
S_TIMER0_config_t* TIMER0_Configurations;



/******************************************************************************
*								 APIS IMPLEMENTATION			              *
*******************************************************************************/
E_STATUS_t MCAL_TIMER0_Init(S_TIMER0_config_t* timer0_cfg)
{
	// First, assume having no ACK
	E_STATUS_t u8_Retval = E_NOK;
	if(timer0_cfg == NULL_PTR)
	{
		u8_Retval = E_NOK;
	}
	else
	{
		uint8_t Temp_TCCR0;
		//1. Save timer configuration
		TIMER0_Configurations = timer0_cfg;
		
		//2. Set timer mode 
		if ((timer0_cfg->mode == Normal) || (timer0_cfg->mode == CTC) || (timer0_cfg->mode == PWM) || (timer0_cfg->mode == FastPWM))
		{
			Temp_TCCR0 |= TIMER0_Configurations->mode;
			u8_Retval = E_OK;
		}
		else
		{
			u8_Retval = E_NOK;
		}
		
		//3. Set Compare Output Mode
		if(timer0_cfg->Compare_Match_Mode != NORMAL_PORT_OP)		//OC0 Connected
		{
			if(timer0_cfg->Compare_Match_Mode != TOGGLE_OC0_CM || timer0_cfg->mode == Normal)
			{
				/* Configure OC0 (PINB3) as Output */
				Set_Bit(DDRB, PINB3);
			}	//else OC0 is Disconnected
			//Clear or Set or Toggle OC0 on Compare match
			
			Temp_TCCR0 |= timer0_cfg->Compare_Match_Mode;
		}

		
		//4. Set timer CLK
		if ((timer0_cfg->clk >= NO_CLK_SRC) && (timer0_cfg->clk <= EXT_CLK_RISING))
		{
			Temp_TCCR0 |= TIMER0_Configurations->clk;
			u8_Retval = E_OK;
		}
		else
		{
			u8_Retval = E_NOK;
		}
		
		
		//4. Set OCM interrupt
		if ((timer0_cfg->ocmInterrupt == OCIE_ENABLE) || (timer0_cfg->ocmInterrupt == OCIE_DISABLE))
		{
			Temp_TCCR0 |= TIMER0_Configurations->ocmInterrupt;
			u8_Retval = E_OK;
		}
		else
		{
			u8_Retval = E_NOK;
		}
		
		//5. Set Overflow interrupt
		if ((timer0_cfg->ovfInterrupt == TOVFIE_ENABLE) || (timer0_cfg->ovfInterrupt == TOVFIE_DISABLE))
		{
			Temp_TCCR0 |= TIMER0_Configurations->ovfInterrupt;
			u8_Retval = E_OK;
		}
		else
		{
			u8_Retval = E_NOK;
		}
		
		
		// 6. Set TCNT0 = zero
		TCNT0 = 0;
		
		/* Store TCCR0 */
		TCCR0 = Temp_TCCR0;
		return u8_Retval;
	}
	
}
E_STATUS_t MCAL_TIMER0_Stop()
{
	E_STATUS_t u8_Retval = E_NOK;
	TIMSK |= NO_CLK_SRC;
	return u8_Retval;
}

E_STATUS_t MCAL_TIMER0_GetCompareValue(uint8_t* pu8_TicksNumber)
{
	E_STATUS_t u8_Retval = E_NOK;
	*pu8_TicksNumber = OCR0;
	return u8_Retval;
}

E_STATUS_t MCAL_TIMER0_SetCompareValue(uint8_t u8_TicksNumber)
{
	E_STATUS_t u8_Retval = E_NOK;
	OCR0 = u8_TicksNumber;
	return u8_Retval;
}

E_STATUS_t MCAL_TIMER0_GetCounterValue(uint8_t* pu8_TicksNumber)
{
	E_STATUS_t u8_Retval = E_NOK;
	*pu8_TicksNumber = TCNT0;
	return u8_Retval;
}

E_STATUS_t MCAL_TIMER0_SetCounterValue(uint8_t u8_TicksNumber)
{
	E_STATUS_t u8_Retval = E_NOK;
	TCNT0 = u8_TicksNumber;
	return u8_Retval;
}

E_STATUS_t MCAL_TIMER0_GetOverflowValue(uint8_t* pu8_TicksNumber)
{
	E_STATUS_t u8_Retval = E_NOK;
	*pu8_TicksNumber = u8_TOVNumber;
	return u8_Retval;
}

E_STATUS_t MCAL_TIMER0_SetOverflowValue(uint8_t u8_TicksNumber)
{
	E_STATUS_t u8_Retval = E_OK;
	u8_TOVNumber = u8_TicksNumber;
	return u8_Retval;
}


E_STATUS_t MCAL_PWM_DutyCycle(uint8_t Duty_Cycle){
	E_STATUS_t u8_Retval = E_OK;
	
	////The N variable represents the prescale factor (1, 8, 64, 256, or 1024)
	//uint8_t prescale_factor[5] = {1, 8, 64, 256, 1024};
	//uint8_t N = prescale_factor[TIMER0->TCCR0.bits.CS0n-1];
	//uint8_t Fpwm = 8000000 /(N*510);
	if( 0< Duty_Cycle && Duty_Cycle <= 100){
		if(TIMER0_Configurations->mode !=  Normal ||TIMER0_Configurations->mode != CTC)
		{
			if(TIMER0_Configurations->mode == PWM )
			{
				OCR0 = (uint8_t)(255ul-((255*Duty_Cycle)/100ul)/2ul);
			}
			else if(TIMER0_Configurations->mode == FastPWM)
			{
				if(TIMER0_Configurations->Compare_Match_Mode == NONINVERTING_CLEAR)
				OCR0 = (uint8_t)((255ul*Duty_Cycle)/100ul);
				
				else if(TIMER0_Configurations->Compare_Match_Mode == INVERTING_SET)
				MCAL_TIMER0_SetCompareValue((uint8_t)(255ul -((255ul*Duty_Cycle)/100ul)));
			}
		}
	}
	return u8_Retval;
}


E_STATUS_t MCAL_PWM_DutyCycle_2(uint8_t Duty_Cycle)
{
	u8 Local_u8Variable=((Duty_Cycle*255ul)/100ul);
	OCR0=Local_u8Variable;
}

/************************************************************************
*								 ISR Functions							*
*************************************************************************/

void TIMER0_CALLBACK_Overflow_INTERRUPT(PTR_VoidFuncVoid_t callback)
{
	CALLBACK_CM = callback;
}
void TIMER0_CALLBACK_CompareMatch_INTERRUPT(PTR_VoidFuncVoid_t callback)
{
	CALLBACK_OVF = callback;
}

ISR(TIMER0_COMP_vect)
{
	CALLBACK_CM();
}


ISR(TIMER0_OVF_vect)
{
	u8_TOVNumber++;
	CALLBACK_OVF();
}