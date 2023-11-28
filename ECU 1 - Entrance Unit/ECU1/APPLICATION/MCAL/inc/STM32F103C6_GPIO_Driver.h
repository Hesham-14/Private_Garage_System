/*
 * STM32F103C6_GPIO_Driver.h
 *
 *  Created on: May 2, 2023
 *      Author: hesham mohamed
 */

#ifndef INC_STM32F103C6_GPIO_DRIVER_H_
#define INC_STM32F103C6_GPIO_DRIVER_H_

//===============================================
//Includes
//===============================================
#include "STM32F103x6.h"

//===============================================
//User type definitions (structures), what i need to know from the user
//===============================================
typedef struct
{
	uint16_t GPIO_PinNumber		;		// Specifies the GPIO pins to be configured
										// This prameter can be a value of @ref GPIO_PINS_define

	uint8_t GPIO_Mode			;		// Specifies the operation mode for the selected pins
										// This prameter can be a value of @ref GPIO_MODE_define

	uint8_t GPIO_Output_Speed	;		// Specifies the speed for the selected output pins
										// This prameter can be a value of @ref GPIO_SPEED_define
}GPIO_PinConfig_t;


//===============================================
// Macros Configuration References
//===============================================
//	@ref PIO_PINS_define
#define GPIO_PIN_0					((uint16_t)0x0001)
#define GPIO_PIN_1					((uint16_t)0x0002)
#define GPIO_PIN_2					((uint16_t)0x0004)
#define GPIO_PIN_3					((uint16_t)0x0008)
#define GPIO_PIN_4					((uint16_t)0x0010)
#define GPIO_PIN_5					((uint16_t)0x0020)
#define GPIO_PIN_6					((uint16_t)0x0040)
#define GPIO_PIN_7					((uint16_t)0x0080)
#define GPIO_PIN_8					((uint16_t)0x0100)
#define GPIO_PIN_9					((uint16_t)0x0200)
#define GPIO_PIN_10					((uint16_t)0x0400)
#define GPIO_PIN_11					((uint16_t)0x0800)
#define GPIO_PIN_12					((uint16_t)0x1000)
#define GPIO_PIN_13					((uint16_t)0x2000)
#define GPIO_PIN_14					((uint16_t)0x4000)
#define GPIO_PIN_15					((uint16_t)0x8000)
#define GPIO_PIN_ALL				((uint16_t)0xFFFF)	/*All pins selected*/

/*	@ref GPIO_SPEED_define
 * 		1: Output mode, max speed 10 MHz.
 * 		2: Output mode, max speed 2 MHz.
 * 		3: Output mode, max speed 50 MHz
*/
#define GPIO_SPEED_10M				0x00000001u
#define GPIO_SPEED_2M				0x00000002u
#define GPIO_SPEED_50M				0x00000003u


/*	@ref GPIO_MODE_define
 * 		0: Analog mode
 *		1: Input floating (resert state)
 *		2: Input pull-up
 *		3: Input pull-down
 * 		4: General purpose output push-pull
 * 		5: General purpose output open-drain
 * 		6: Alternate function output push-pull
 * 		7: Alternate function output open-drain
 * 		8: Alternate function input
*/
#define GPIO_MODE_ANALOG			0x00000000u
#define GPIO_MODE_INPUT_FLO			0x00000001u
#define GPIO_MODE_INPUT_PU			0x00000002u
#define GPIO_MODE_INPUT_PD			0x00000003u
#define GPIO_MODE_OUTPUT_PP			0x00000004u
#define GPIO_MODE_OUTPUT_OD			0x00000005u
#define GPIO_MODE_OUTPUT_AF_PP		0x00000006u
#define GPIO_MODE_OUTPUT_AF_OD		0x00000007u
#define GPIO_MODE_AF_INPUT			0x00000001u

//@ref GPIO_PIN_state
#define GPIO_PIN_SET 				1
#define GPIO_PIN_RESET 				0

//@ref GPIO_LOCK_state
#define GPIO_PIN_LOCK_DONE 			1
#define GPIO_PIN_LOCK_ERROR			0

//===============================================
// APIs Supported by "MCAL GPIO DRIVER"
//===============================================
void MCAL_GPIO_Init(GPIO_t *GPIOx, GPIO_PinConfig_t *PinConfig);
void MCAL_GPIO_ReInit(GPIO_t *GPIOx);

//Read APIs
uint8_t MCAL_GPIO_ReadPin(GPIO_t *GPIOx, uint16_t PinNumber);
uint32_t MCAL_GPIO_ReadPORT(GPIO_t *GPIOx);

//Write APIs
void MCAL_GPIO_WritePin(GPIO_t *GPIOx, uint16_t *PinNumber, uint8_t value);
void MCAL_GPIO_WritePort(GPIO_t *GPIOx, uint32_t value);
void MCAL_GPIO_TogglePin(GPIO_t *GPIOx, uint16_t PinNumber);

//other APIs
uint8_t MCAL_GPIO_LockPin(GPIO_t *GPIOx, uint16_t *PinNumber);
void MCAL_GPIO_EXTI_IRQConfig();
void MCAL_GPIO_EXTI_IRQHandling();



#endif /* INC_STM32F103C6_GPIO_DRIVER_H_ */
