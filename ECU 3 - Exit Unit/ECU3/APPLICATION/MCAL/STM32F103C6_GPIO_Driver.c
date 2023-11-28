/*
 * STM32F103C6_GPIO_Driver.c
 *
 *  Created on: May 2, 2023
 *      Author: hesham mohamed
 */
#include "STM32F103C6_GPIO_Driver.h"


uint8_t Get_CRLH_Position(uint16_t PinNumber)
{
	switch(PinNumber)
	{
	case GPIO_PIN_0:
		return 0;
		break;
	case GPIO_PIN_1:
		return 4;
		break;
	case GPIO_PIN_2:
		return 8;
		break;
	case GPIO_PIN_3:
		return 12;
		break;
	case GPIO_PIN_4:
		return 16;
		break;
	case GPIO_PIN_5:
		return 20;
		break;
	case GPIO_PIN_6:
		return 24;
		break;
	case GPIO_PIN_7:
		return 28;
		break;
	case GPIO_PIN_8:
		return 0;
		break;
	case GPIO_PIN_9:
		return 4;
		break;
	case GPIO_PIN_10:
		return 8;
		break;
	case GPIO_PIN_11:
		return 12;
		break;
	case GPIO_PIN_12:
		return 16;
		break;
	case GPIO_PIN_13:
		return 20;
		break;
	case GPIO_PIN_14:
		return 24;
		break;
	case GPIO_PIN_15:
		return 28;
		break;
	}
	return 0;
}


/**================================================================
 * @Fn			-MCAL_GPIO_Init
 * @brief 		-Initializes the GPIOx PINy according to the specified parameters in the PinConfig
 * @param [in]	-GPIOx: where x can be (A..E dependeing on device used) to select the GPIO peripheral
 * @param [in] 	-PinConfig: a pointer to a GPIO_PinConfig_t structure that contains
 * 				 the configuration information about the specified GPIO pin.
 * @retval 		-none
 * Note			-STM32F103C6 MCU has GPIO A,B,C,D,E Modules,
 * 				 but LQFP48 package has only GPIO A, B, and (part of C,D) exported as extenal PINs from thr MCU
 */
void MCAL_GPIO_Init(GPIO_t *GPIOx, GPIO_PinConfig_t *PinConfig)
{
	//check CRL\H
	volatile uint32_t *ConfigRegister = NULL;
	uint8_t PIN_Config = 0;
	ConfigRegister = (PinConfig->GPIO_PinNumber < GPIO_PIN_8)? &GPIOx->CRL: &GPIOx->CRH;

	//0. clear CNFy[1:0] MODEy[1:0]
	*ConfigRegister &= ~(0xF << Get_CRLH_Position(PinConfig->GPIO_PinNumber));

	//1. if pin is output
	if((PinConfig->GPIO_Mode >= GPIO_MODE_OUTPUT_PP) && (PinConfig->GPIO_Mode <= GPIO_MODE_OUTPUT_AF_OD))
	{
		//set CNF from output options, then set MODE (speed)
		PIN_Config = (((PinConfig->GPIO_Mode - 0x4UL) << 2) | (PinConfig->GPIO_Output_Speed & 0x0F));
	}
	//2. if pin is input
	else
	{
		//if input pull-down, reset ODR
		if(PinConfig->GPIO_Mode == GPIO_MODE_INPUT_PD)
		{
			GPIOx->ODR &= ~(PinConfig->GPIO_PinNumber);
		}
		//if input pull-up, set ODR
		else if(PinConfig->GPIO_Mode == GPIO_MODE_INPUT_PU)
		{
			GPIOx->ODR |= (PinConfig->GPIO_PinNumber);
		}

		//set CNF from input options, then set MODe (00)
		PIN_Config = ((PinConfig->GPIO_Mode << 2) & 0x0F );
	}
	//3. update register with the Pin_Config information
	*ConfigRegister |= (PIN_Config << Get_CRLH_Position(PinConfig->GPIO_PinNumber));
}


/**================================================================
 * @Fn			-MCAL_GPIO_ReInit
 * @brief 		-Reset the GPIOx PINs
 * @param [in]	-GPIOx: where x can be (A..E dependeing on device used) to select the GPIO peripheral
 * @retval 		-none
 * Note			-none
 */
void MCAL_GPIO_ReInit(GPIO_t *GPIOx)
{
	//I can reset every pin individually using bit reset register GPIOx_BRR

	//or reset the hole GPIOx using RCC_APB2RSTR
	if(GPIOx == AFIO)
	{
		RCC->APB2RSTR |= (1<<0);
		RCC->APB2RSTR &= ~(1<<0);
	}
	else if (GPIOx == GPIOA || GPIOx == GPIOB || GPIOx == GPIOC || GPIOx == GPIOD || GPIOx == GPIOE)
	{
		//compare GPIOx position to GPIOA position, then move to its reset bit
		RCC->APB2RSTR |= (1<<(2 + ((GPIOA - GPIOx)>>10)));
		RCC->APB2RSTR &= ~(1<<(2 + ((GPIOA - GPIOx)>>10)));
	}
}


/**================================================================
 * @Fn			-MCAL_GPIO_ReadPin
 * @brief 		-Reads specified pin
 * @param [in]	-GPIOx: where x can be (A..E dependeing on device used) to select the GPIO peripheral
 * @param [in] 	-PinNumber: Set pin number according @ref GPIO_PINS_define
 * @retval 		-Pin value (two values based on @ref GPIO_PIN_state)
 * Note			-none
 */
uint8_t MCAL_GPIO_ReadPin(GPIO_t *GPIOx, uint16_t PinNumber)
{
	if((GPIOx->IDR & (uint32_t)PinNumber) != (uint32_t)GPIO_PIN_RESET)
		return GPIO_PIN_SET;
	else
		return GPIO_PIN_RESET;
}

/**================================================================
 * @Fn			-MCAL_GPIO_ReadPORT
 * @brief 		-Reads port value
 * @param [in]	-GPIOx: where x can be (A..E dependeing on device used) to select the GPIO peripheral
 * @retval 		-Port value
 * Note			-none
 */
uint32_t MCAL_GPIO_ReadPORT(GPIO_t *GPIOx)
{
	return ((uint32_t)GPIOx->IDR);
}


/**================================================================
 * @Fn			-MCAL_GPIO_WritePin
 * @brief 		-Writes on specified pin
 * @param [in]	-GPIOx: Where x can be (A..E dependeing on device used) to select the GPIO peripheral
 * @param [in] 	-PinNumber: Set pin number according @ref GPIO_PINS_define
 * @param [in] 	-Value: Pin value
 * @retval 		-none
 * Note			-none
 */
void MCAL_GPIO_WritePin(GPIO_t *GPIOx, uint16_t *PinNumber, uint8_t value)
{
	//set pin by 1
	if(value != GPIO_PIN_RESET)
		GPIOx->BSRR = (uint32_t)PinNumber;
	else
		//reset bit
		GPIOx->BRR = (uint32_t)PinNumber;
}


/**================================================================
 * @Fn			-MCAL_GPIO_WritePort
 * @brief 		-Writes on specified port
 * @param [in]	-GPIOx: Where x can be (A..E dependeing on device used) to select the GPIO peripheral
 * @param [in] 	-Value: port value
 * @retval 		-none
 * Note			-none
 */
void MCAL_GPIO_WritePort(GPIO_t *GPIOx, uint32_t value)
{
	//set pin by any value
	if(value != GPIO_PIN_RESET)
		GPIOx->BSRR = value;
	else
		//reset bit
		GPIOx->BRR = value;

	//or write directly on port
	//GPIOx->ODR = value;
}

/**================================================================
 * @Fn			-MCAL_GPIO_TogglePin
 * @brief 		-Toggles specified GPIO pin
 * @param [in]	-GPIOx: Where x can be (A..E dependeing on device used) to select the GPIO peripheral
 * @param [in] 	-PinNumber: Set pin number according @ref GPIO_PINS_define
 * @retval 		-none
 * Note			-none
 */
void MCAL_GPIO_TogglePin(GPIO_t *GPIOx, uint16_t PinNumber)
{
	GPIOx->ODR ^= (uint32_t) PinNumber;
}

/**================================================================
 * @Fn			-MCAL_GPIO_LockPin
 * @brief 		-Locks specified GPIO pin
 * @param [in]	-GPIOx: Where x can be (A..E dependeing on device used) to select the GPIO peripheral
 * @param [in] 	-PinNumber: Set pin number according @ref GPIO_PINS_define
 * @retval 		-DONE if pin is locked or ERROR if pin not locked (two states based on @ref GPIO_PIN_LOCK_state)
 * Note			-none
 */
uint8_t MCAL_GPIO_LockPin(GPIO_t *GPIOx, uint16_t *PinNumber)
{
	//1. set bit 16 in temp
	uint32_t temp = (1<<16);
	//2. add pin to temp
	temp |= (uint32_t)PinNumber;

	//3. LOCK key writing sequence:
	//	Write 1
	GPIOx->LCKR = temp;
	//	Write 0
	GPIOx->LCKR = (uint32_t)PinNumber;
	//	Write 1
	GPIOx->LCKR = temp;
	//	Read 0
	temp = GPIOx->LCKR;
	//	Read 1 (this read is optional but confirms that the lock is active)
	if( (uint32_t)(temp &(1<<16)) )
		return GPIO_PIN_LOCK_DONE;
	else
		return GPIO_PIN_LOCK_ERROR;
}




/**================================================================
 * @Fn			-MCAL_GPIO_EXTI_IRQConfig
 * @brief 		-
 * @param [in]	-
 * @retval 		-none
 * Note			-none
 */
void MCAL_GPIO_EXTI_IRQConfig()
{

}

/**================================================================
 * @Fn			-
 * @brief 		-
 * @param [in]	-
 * @param [in] 	-
 * @retval 		-none
 * Note			-
 */
void MCAL_GPIO_EXTI_IRQHandling()
{

}









