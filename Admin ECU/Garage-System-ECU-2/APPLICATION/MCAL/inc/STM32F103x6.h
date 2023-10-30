/*
 * STM32F103x6.h
 *
 *  Created on: May 2, 2023
 *      Author: hesham mohamed
 */

#ifndef INC_STM32F103X6_H_
#define INC_STM32F103X6_H_

//-----------------------------
//Includes
//-----------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define u8			uint8_t
#define u16			uint16_t
#define u32			uint32_t


//-----------------------------
//Base addresses for Memories
//-----------------------------
#define FLASH_MEMORY_BASE						0x08000000UL
#define SYSTEM_MEMORY_BASE						0x1FFFF000UL
#define SRAM_BASE								0x20000000UL
#define Peripherals_BASE						0x40000000UL
#define Cortex_M3_Internal_Peripherals_BASE		0xE0000000UL


//------------------------------------
//Base addresses for AHB Peripherals
//------------------------------------
#define RCC_BASE							(Peripherals_BASE + 0x00021000UL)



//----------------------------------------------------
//Base addresses for Cortex-M3 Internal Peripherals
//-----------------------------------------------------
//NVIC
#define NVIC_BASE							(Cortex_M3_Internal_Peripherals_BASE + 0x0000E100UL)

//--------------------------------------
//Base addresses for APB2 Peripherals
//--------------------------------------
//GPIO
//A,B fully included in LQFP48 package
#define GPIOA_BASE							(Peripherals_BASE + 0x00010800UL)
#define GPIOB_BASE							(Peripherals_BASE + 0x00010C00UL)
//C,D partial included in LQFP48 package
#define GPIOC_BASE							(Peripherals_BASE + 0x00011000UL)
#define GPIOD_BASE							(Peripherals_BASE + 0x00011400UL)
//E not included in LQFP48 package
#define GPIOE_BASE							(Peripherals_BASE + 0x00011800UL)
#define EXTI_BASE							(Peripherals_BASE + 0x00010400UL)
#define USART1_BASE							(Peripherals_BASE + 0x00013800UL)
#define AFIO_BASE							(Peripherals_BASE + 0x00010000UL)
#define SPI1_BASE							(Peripherals_BASE + 0x00013000UL)


//--------------------------------------
//Base addresses for APB1 Peripherals
//--------------------------------------
#define USART2_BASE							(Peripherals_BASE + 0x00004400UL)
#define USART3_BASE							(Peripherals_BASE + 0x00004800UL)
#define SPI2_BASE							(Peripherals_BASE + 0x00003800UL)


//>>>>>>>>>>>>>>>>>>>>>>>>>
//Peripheral register:
//>>>>>>>>>>>>>>>>>>>>>>>>>

//Peripheral register: NVIC
//because NVIC registers located separaterly, we are gonna to define every register by it self
#define NVIC_ISER0							*(volatile u32 *)(NVIC_BASE + 0x000)
#define NVIC_ISER1							*(volatile u32 *)(NVIC_BASE + 0x004)
#define NVIC_ISER2							*(volatile u32 *)(NVIC_BASE + 0x008)
#define NVIC_ICER0							*(volatile u32 *)(NVIC_BASE + 0x080)
#define NVIC_ICER1							*(volatile u32 *)(NVIC_BASE + 0x084)
#define NVIC_ICER2							*(volatile u32 *)(NVIC_BASE + 0x088)

//Peripheral register: RCC
typedef struct
{
	volatile u32 CR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 APB2RSTR;
	volatile u32 APB1RSTR;
	volatile u32 AHNENR;
	volatile u32 APB2ENR;
	volatile u32 APB1ENR;
	volatile u32 BDCR;
	volatile u32 CSR;
} RCC_t;

//Peripheral register: GPIO
typedef struct
{
	volatile u32 CRL;
	volatile u32 CRH;
	volatile u32 IDR;
	volatile u32 ODR;
	volatile u32 BSRR;
	volatile u32 BRR;
	volatile u32 LCKR;
} GPIO_t;

//Peripheral register: EXTI
typedef struct
{
	volatile u32 IMR;
	volatile u32 EMR;
	volatile u32 RTSR;
	volatile u32 FTSR;
	volatile u32 SWIER;
	volatile u32 PR;
} EXTI_t;

//Peripheral register: AFIO
typedef struct
{
	volatile u32 EVCR;
	volatile u32 MAPR;
	volatile u32 EXTICR[4];
	volatile u32 RESERVED0;
	volatile u32 MAPR2;
} AFIO_t;

//Peripheral register: USART
typedef struct
{
	volatile u32 SR;
	volatile u32 DR;
	volatile u32 BRR;
	volatile u32 CR1;
	volatile u32 CR2;
	volatile u32 CR3;
	volatile u32 GTPR;
} USART_t;

//Peripheral register: SPI
typedef struct
{
	volatile u32 CR1;
	volatile u32 CR2;
	volatile u32 SR;
	volatile u32 DR;
	volatile u32 CRCPR;
	volatile u32 RXCRCR;
	volatile u32 TXCRCR;
	volatile u32 I2SCFGR;
	volatile u32 I2SPR;
} SPI_t;

//>>>>>>>>>>>>>>>>>>>>>>>>>
//Peripheral Instants:
//>>>>>>>>>>>>>>>>>>>>>>>>>
#define GPIOA				((GPIO_t *)GPIOA_BASE)
#define GPIOB				((GPIO_t *)GPIOB_BASE)
#define GPIOC				((GPIO_t *)GPIOC_BASE)
#define GPIOD				((GPIO_t *)GPIOD_BASE)
#define GPIOE				((GPIO_t *)GPIOE_BASE)

#define RCC					((RCC_t *)RCC_BASE)

#define EXTI				((EXTI_t *)EXTI_BASE)
#define AFIO				((AFIO_t *)AFIO_BASE)

#define USART1				((USART_t *)USART1_BASE)
#define USART2				((USART_t *)USART2_BASE)
#define USART3				((USART_t *)USART3_BASE)

#define SPI1				((SPI_t *)SPI1_BASE)
#define SPI2				((SPI_t *)SPI2_BASE)


//>>>>>>>>>>>>>>>>>>>>>>>>>
//IVT:
//>>>>>>>>>>>>>>>>>>>>>>>>>
//EXTI
#define EXTI0_IRQ			6
#define EXTI1_IRQ			7
#define EXTI2_IRQ			8
#define EXTI3_IRQ			9
#define EXTI4_IRQ			10
#define EXTI5_IRQ			23
#define EXTI6_IRQ			23
#define EXTI7_IRQ			23
#define EXTI8_IRQ			23
#define EXTI9_IRQ			23
#define EXTI10_IRQ			40
#define EXTI11_IRQ			40
#define EXTI12_IRQ			40
#define EXTI13_IRQ			40
#define EXTI14_IRQ			40
#define EXTI15_IRQ			40

#define USART1_IRQ			37
#define USART2_IRQ			38
#define USART3_IRQ			39

#define SPI1_IRQ			35
#define SPI2_IRQ			36


//>>>>>>>>>>>>>>>>>>>>>>>>>
//clock Enable Macros:
//>>>>>>>>>>>>>>>>>>>>>>>>>
#define RCC_AFIO_CLK_EN()			(RCC->APB2ENR |= 1 <<0)
#define RCC_GPIOA_CLK_EN()			(RCC->APB2ENR |= 1 <<2)
#define RCC_GPIOB_CLK_EN()			(RCC->APB2ENR |= 1 <<3)
#define RCC_GPIOC_CLK_EN()			(RCC->APB2ENR |= 1 <<4)
#define RCC_GPIOD_CLK_EN()			(RCC->APB2ENR |= 1 <<5)
#define RCC_GPIOE_CLK_EN()			(RCC->APB2ENR |= 1 <<6)

#define RCC_USART1_CLK_EN()			(RCC->APB2ENR |= 1 <<14)
#define RCC_USART2_CLK_EN()			(RCC->APB1ENR |= 1 <<17)
#define RCC_USART3_CLK_EN()			(RCC->APB1ENR |= 1 <<18)

#define RCC_SPI1_CLK_EN()			(RCC->APB2ENR |= 1 <<12)
#define RCC_SPI2_CLK_EN()			(RCC->APB1ENR |= 1 <<14)

//>>>>>>>>>>>>>>>>>>>>>>>>>
//clock Reset Macros:
//>>>>>>>>>>>>>>>>>>>>>>>>>
#define RCC_USART1_CLK_Reset()		(RCC->APB2RSTR |= 1 <<14)
#define RCC_USART2_CLK_Reset()		(RCC->APB1RSTR |= 1 <<17)
#define RCC_USART3_CLK_Reset()		(RCC->APB1RSTR |= 1 <<18)

#define RCC_SPI1_CLK_Reset()		(RCC->APB2RSTR |= 1 <<12)
#define RCC_SPI2_CLK_Reset()		(RCC->APB1RSTR |= 1 <<14)

//>>>>>>>>>>>>>>>>>>>>>>>>>
//NVIC IRQ Enable/Disable Macros:
//>>>>>>>>>>>>>>>>>>>>>>>>>
#define NVIC_EXT0_IRQ6_Enable()					(NVIC_ISER0 |= 1<<6)
#define NVIC_EXT1_IRQ7_Enable()					(NVIC_ISER0 |= 1<<7)
#define NVIC_EXT2_IRQ8_Enable()					(NVIC_ISER0 |= 1<<8)
#define NVIC_EXT3_IRQ9_Enable()					(NVIC_ISER0 |= 1<<9)
#define NVIC_EXT4_IRQ10_Enable()				(NVIC_ISER0 |= 1<<10)
#define NVIC_EXT9_5_IRQ23_Enable()				(NVIC_ISER0 |= 1<<23)
#define NVIC_EXT15_10_IRQ40_Enable()			(NVIC_ISER1 |= 1<<8)	//40-32 = 8
#define NVIC_SPI1_IRQ35_Enable()				(NVIC_ISER1 |= 1<< (SPI1_IRQ - 32))
#define NVIC_SPI2_IRQ36_Enable()				(NVIC_ISER1 |= 1<< (SPI1_IRQ - 32))
#define NVIC_USART1_IRQ37_Enable()				(NVIC_ISER1 |= 1<< (USART1_IRQ - 32))
#define NVIC_USART2_IRQ38_Enable()				(NVIC_ISER1 |= 1<< (USART2_IRQ - 32))
#define NVIC_USART3_IRQ39_Enable()				(NVIC_ISER1 |= 1<< (USART3_IRQ - 32))



#define NVIC_EXT0_IRQ6_Disable()				(NVIC_ICER0 |= 1<<6)
#define NVIC_EXT1_IRQ7_Disable()				(NVIC_ICER0 |= 1<<7)
#define NVIC_EXT2_IRQ8_Disable()				(NVIC_ICER0 |= 1<<8)
#define NVIC_EXT3_IRQ9_Disable()				(NVIC_ICER0 |= 1<<9)
#define NVIC_EXT4_IRQ10_Disable()				(NVIC_ICER0 |= 1<<10)
#define NVIC_EXT9_5_IRQ23_Disable()				(NVIC_ICER0 |= 1<<23)
#define NVIC_EXT15_10_IRQ40_Disable()			(NVIC_ICER1 |= 1<<8)
#define NVIC_SPI1_IRQ35_Disable()				(NVIC_ICER1 |= 1<< (SPI1_IRQ - 32))
#define NVIC_SPI2_IRQ36_Disable()				(NVIC_ICER1 |= 1<< (SPI1_IRQ - 32))
#define NVIC_USART1_IRQ37_Disable()				(NVIC_ICER1 |= 1<< (USART1_IRQ - 32))
#define NVIC_USART2_IRQ38_Disable()				(NVIC_ICER1 |= 1<< (USART2_IRQ - 32))
#define NVIC_USART3_IRQ39_Disable()				(NVIC_ICER1 |= 1<< (USART3_IRQ - 32))


//>>>>>>>>>>>>>>>>>>>>>>>>>
//Generic Macros:
//>>>>>>>>>>>>>>>>>>>>>>>>>





#endif /* INC_STM32F103X6_H_ */
