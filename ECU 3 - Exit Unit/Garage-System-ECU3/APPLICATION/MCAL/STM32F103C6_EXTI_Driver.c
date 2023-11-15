/*
 * STM32F103C6_EXTI_Driver.c
 *
 *  Created on: May 6, 2023
 *      Author: hesham mohamed
 */

#include "STM32F103C6_EXTI_Driver.h"
#include "STM32F103C6_GPIO_Driver.h"

/*
 * =========================================================================
 * 								Generic Macros
 * =========================================================================
 */
#define AFIO_EXTI_PORT_mapping(x) 	( 	(x==GPIOA)?0:\
										(x==GPIOB)?1:\
										(x==GPIOC)?2:\
										(x==GPIOD)?3:0	)



/*
 * =========================================================================
 * 								Global Variables
 * =========================================================================
 */

void (* GP_IRQ_CALLBACK[15])(void);


/*
 * =========================================================================
 * 								Generic Functions
 * =========================================================================
 */

void Enable_NVIC (uint16_t IRQ)
{
	switch (IRQ)
	{
	case 0:
		NVIC_EXT0_IRQ6_Enable();
		break;
	case 1:
		NVIC_EXT1_IRQ7_Enable();
		break ;
	case 2:
		NVIC_EXT2_IRQ8_Enable();
		break ;
	case 3:
		NVIC_EXT3_IRQ9_Enable();
		break ;
	case 4:
		NVIC_EXT4_IRQ10_Enable();
		break ;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_EXT9_5_IRQ23_Enable();
		break ;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_EXT15_10_IRQ40_Enable();
		break ;

	}
}

void Disable_NVIC (uint16_t IRQ)
{
	switch (IRQ)
		{
		case 0:
			NVIC_EXT0_IRQ6_Disable() ;
			break;
		case 1:
			NVIC_EXT1_IRQ7_Disable() ;
			break ;
		case 2:
			NVIC_EXT2_IRQ8_Disable() ;
			break ;
		case 3:
			NVIC_EXT3_IRQ9_Disable() ;
			break ;
		case 4:
			NVIC_EXT4_IRQ10_Disable() ;
			break ;
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			NVIC_EXT9_5_IRQ23_Disable();
			break ;

		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			NVIC_EXT15_10_IRQ40_Disable();
			break ;

		}

}


void UPDATE_EXTI(EXTI_PinConfig_t *EXTI_Config)
{
	//1. Configure GPIO to be AF input -> Floating input
	GPIO_PinConfig_t EXTI_pin;
	EXTI_pin.GPIO_PinNumber = EXTI_Config->EXTI_PIN.GPIO_Pin;
	EXTI_pin.GPIO_Mode = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(EXTI_Config->EXTI_PIN.GPIO_Port, &EXTI_pin);

	//===============================================================
	//2. configure AFIO_EXTICR to route betwen EXTI line with port A, B, C, D
	//konw which EXTICR
	uint8_t AFIO_EXTICR_index = (EXTI_Config->EXTI_PIN.EXTI_Line_Number / 4);
	//know position in the register bits
	uint8_t AFIO_EXTICR_position = ((EXTI_Config->EXTI_PIN.EXTI_Line_Number % 4)*4);
	//clear the 4 bits for the XTI
	AFIO->EXTICR[AFIO_EXTICR_index] &= ~(0xF << AFIO_EXTICR_position);
	//store port value
	AFIO->EXTICR[AFIO_EXTICR_index] |= ((AFIO_EXTI_PORT_mapping(EXTI_Config->EXTI_PIN.GPIO_Port) & 0x0F) << AFIO_EXTICR_position);

	//===============================================================
	//3. setting trigger state
	//clear trigger registers
	EXTI->RTSR &= ~(1 << EXTI_Config->EXTI_PIN.EXTI_Line_Number);
	EXTI->FTSR &= ~(1 << EXTI_Config->EXTI_PIN.EXTI_Line_Number);
	//check state
	if(EXTI_Config->Trigger_Case == EXTI_TRIGGER_RISING)
	{
		EXTI->RTSR |= (1 << EXTI_Config->EXTI_PIN.EXTI_Line_Number);
	}
	else if (EXTI_Config->Trigger_Case == EXTI_TRIGGER_FALLING)
	{
		EXTI->FTSR |= (1 << EXTI_Config->EXTI_PIN.EXTI_Line_Number);
	}
	else if (EXTI_Config->Trigger_Case == EXTI_TRIGGER_RisingAndFalling)
	{
		EXTI->RTSR |= (1 << EXTI_Config->EXTI_PIN.EXTI_Line_Number);
		EXTI->FTSR |= (1 << EXTI_Config->EXTI_PIN.EXTI_Line_Number);
	}

	//===============================================================
	//4. Update IRQ Handling Callback array
	GP_IRQ_CALLBACK[EXTI_Config->EXTI_PIN.EXTI_Line_Number] = EXTI_Config->P_IRQ_CallBack;

	//===============================================================
	//5. Enable/Disable IRQ EXTI (IMR) & NVIC
	if(EXTI_Config->IRQ_EN == EXTI_IRQ_ENABLE)
	{
		//EXTI IMR
		EXTI->IMR |= (1<< EXTI_Config->EXTI_PIN.EXTI_Line_Number );

		//enable NVIC
		Enable_NVIC (EXTI_Config->EXTI_PIN.EXTI_Line_Number ) ;
	}
	else
	{
		//EXTI IMR
		EXTI->IMR &= ~(1<< EXTI_Config->EXTI_PIN.IVT_IRQ_Number);

		//disable NVIC
		Disable_NVIC (EXTI_Config->EXTI_PIN.IVT_IRQ_Number);
	}
}

//==========================================================================
/*
 * =========================================================================
 * 								APIs Functions Definitions
 * =========================================================================
 */

/**================================================================
 * @Fn			-MCAL_EXTI_GPIO_DeInit
 * @brief 		-rese EXTI Registers and NVIC corresponding IRQ Mask
 * @retval 		-none
 * Note			-none
 */
void MCAL_EXTI_GPIO_DeInit(void)
{
	//1. reset EXTI registers
	EXTI->IMR = 0x00000000;
	EXTI->EMR = 0x00000000;
	EXTI->RTSR = 0x00000000;
	EXTI->FTSR = 0x00000000;
	EXTI->SWIER = 0x00000000;
	EXTI->PR = 0xFFFFFFFF;

	//2. Disable EXTI IRQ from NVIC
	NVIC_EXT0_IRQ6_Disable();
	NVIC_EXT1_IRQ7_Disable();
	NVIC_EXT2_IRQ8_Disable();
	NVIC_EXT3_IRQ9_Disable();
	NVIC_EXT4_IRQ10_Disable();
	NVIC_EXT9_5_IRQ23_Disable();
	NVIC_EXT15_10_IRQ40_Disable();

}

/**================================================================
 * @Fn			-MCAL_EXTI_GPIO_Init
 * @brief 		-Initializes the EXTI from specific GPIO pin and specify the trigger and the callback
 * @param [in]	-EXTI_Config: set by @ref EXTI_PINS_define, TRIGGER_CASE_define, EXTI_IRQ_define
 * @retval 		-none
 * Note			-STM32F103C6 MCU has GPIO A,B,C,D,E Modules,
 * 				 but LQFP48 package has only GPIO A, B, and (part of C,D) exported as extenal PINs from thr MCU
 * 				 -Also mandatory to enable RCC clock for AFIO and the corresponding GPIO
 */
void MCAL_EXTI_GPIO_Init(EXTI_PinConfig_t *EXTI_Config)
{
	UPDATE_EXTI(EXTI_Config);
}



void MCAL_EXTI_GPIO_Update(EXTI_PinConfig_t *EXTI_Config)
{
	UPDATE_EXTI(EXTI_Config);
}


/*
 * =========================================================================
 * 								IRQ Functions
 * =========================================================================
 */
void EXTI0_IRQHandler()
{
	//clear pinding register by writting 1 on it
	EXTI->PR |= (1 << 0);

	//call CALLBACK
	GP_IRQ_CALLBACK[0]();
}
void EXTI1_IRQHandler()
{
	//clear pinding register by writting 1 on it
	EXTI->PR |= (1 << 1);

	//call CALLBACK
	GP_IRQ_CALLBACK[1]();
}
void EXTI2_IRQHandler()
{
	//clear pinding register by writting 1 on it
	EXTI->PR |= (1 << 2);

	//call CALLBACK
	GP_IRQ_CALLBACK[2]();
}
void EXTI3_IRQHandler()
{
	//clear pinding register by writting 1 on it
	EXTI->PR |= (1 << 3);

	//call CALLBACK
	GP_IRQ_CALLBACK[3]();
}
void EXTI4_IRQHandler()
{
	//clear pinding register by writting 1 on it
	EXTI->PR |= (1 << 4);

	//call CALLBACK
	GP_IRQ_CALLBACK[4]();
}
void EXTI9_5_IRQHandler()
{
	//check which EXTI
	if( EXTI->PR & 1<<5 ) { EXTI->PR |= (1 << 5); GP_IRQ_CALLBACK[5](); }
	if( EXTI->PR & 1<<6 ) { EXTI->PR |= (1 << 6); GP_IRQ_CALLBACK[6](); }
	if( EXTI->PR & 1<<7 ) { EXTI->PR |= (1 << 7); GP_IRQ_CALLBACK[7](); }
	if( EXTI->PR & 1<<8 ) { EXTI->PR |= (1 << 8); GP_IRQ_CALLBACK[8](); }
	if( EXTI->PR & 1<<9 ) { EXTI->PR |= (1 << 9); GP_IRQ_CALLBACK[9](); }

}
void EXTI15_10_IRQHandler()
{
	//check which EXTI
	if( EXTI->PR & 1<<10 ) { EXTI->PR |= (1 << 10); GP_IRQ_CALLBACK[10](); }
	if( EXTI->PR & 1<<11 ) { EXTI->PR |= (1 << 11); GP_IRQ_CALLBACK[11](); }
	if( EXTI->PR & 1<<12 ) { EXTI->PR |= (1 << 12); GP_IRQ_CALLBACK[12](); }
	if( EXTI->PR & 1<<13 ) { EXTI->PR |= (1 << 13); GP_IRQ_CALLBACK[13](); }
	if( EXTI->PR & 1<<14 ) { EXTI->PR |= (1 << 14); GP_IRQ_CALLBACK[14](); }
	if( EXTI->PR & 1<<15 ) { EXTI->PR |= (1 << 15); GP_IRQ_CALLBACK[15](); }

}
