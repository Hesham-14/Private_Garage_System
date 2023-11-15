/*
 * STM32F103C6_USART_Driver.h
 *
 *  Created on: May 20, 2023
 *      Author: hesham mohamed
 */

#ifndef STM32F103C6_USART_DRIVER_H_
#define STM32F103C6_USART_DRIVER_H_

#include "STM32F103x6.h"
#include "STM32F103C6_GPIO_Driver.h"


//macros
#define HSI_RC_CLK		(uint32_t)8000000
#define HSE_RC_CLK		(uint32_t)16000000
#define PLL_RC_CLK		(uint32_t)72000000

//APIs
uint32_t MCAL_RCC_GetSYS_CLKFreq(void);
uint32_t MCAL_RCC_GetHCLKFreq(void);
uint32_t MCAL_RCC_GetPCLK1Freq(void);
uint32_t MCAL_RCC_GetPCLK2Freq(void);

#endif /* STM32F103C6_USART_DRIVER_H_ */
