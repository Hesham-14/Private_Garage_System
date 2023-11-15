/*
 * STM32F103C6_UART_Driver.h
 *
 *  Created on: Oct 15, 2023
 *      Author: hesham mohamed
 */

#ifndef INC_STM32F103C6_SPI_DRIVER_H_
#define INC_STM32F103C6_SPI_DRIVER_H_

/*****************************************
-------------   Includes   -------------
*****************************************/
#include "STM32F103x6.h"
#include "STM32F103C6_GPIO_Driver.h"


/******************************************************
----------------   Global Variables   ----------------
******************************************************/
struct S_IRQ_SRC{
	u8 TXE:1;
	u8 RXNE:1;
	u8 ERRI:1;
	u8 Reserved:5;
};

enum SPI_Polling_mechanism
{
	PollingEnable,
	PollingDisable
};


/******************************************************************
---------   User Type Definition || Prebuild Configure   ---------
*******************************************************************/
typedef struct
{
	u16 Mode;					// Specifies the Master/Slave mode
								// This prameter can be a value of @ref SPI_MODE_define

	u16 Comm_Mode;				// Specifies the Communication mode
								// @ref SPI_COMM_MODE_define

	u16 Frame_Format;			// Specifies  which one (MSB or LSB) is transfered first
								// @ref SPI_FRAME_define

	u16 Data_Size;				// Specifies size of data (8-16) bits
								// @ref SPI_DATA_define

	u16 NSS;					// Specifies the NSS signal is managed by hardware (NSS pin) or software using SSI bit enable
								// @ref SPI_NSS_define

	u16 Polarity;				// Specifies the ideal state for SPI (high or low)
								// @ref SPI_POLARITY_define

	u16 Phase;					// Specifies which edge is the MSB capture strobe
								// @ref SPI_PHASE_define

	u16 Baudrate_Prescaler;		// Specifies
								// @ref SPI_BAUDRATEPRESCALER_define
								// Note: The communication CLK is derived from the master clock, the slave clock doesn't need to be set

	u16 IRQ_Enable;				// Specifies enabling of the interrupt
								// @ref SPI_IRQ_define

	void (* P_ISR_CallBack)(S_IRQ_SRC);	// The ISR Function

}SPI_Config_t;



/*****************************************************
---------   Macros Configuration Refrences  --------
******************************************************/
//@ref SPI_MODE_define						/*CR1 - MSTR*/
#define SPI_MODE_Master						(u32) (1<<2)
#define SPI_MODE_Slave						(u32) (0<<2)

//@ref SPI_COMM_MODE_define					/*CR1 - BIDIMODE, BIDIOE, RXONLY*/
#define SPI_COMM_2Lines_FD					(u32) ((0<<15) | (0<<10))
#define SPI_COMM_2Lines_Rx					(u32) ((0<<15) | (1<<10))
#define SPI_COMM_1Lines_Rx					(u32) ((1<<15) | (0<<14))
#define SPI_COMM_1Lines_Tx					(u32) ((1<<15) | (1<<14))

//@ref SPI_FRAME_define						/*CR1 - LSBFIRST*/
#define SPI_FRAME_MSB						(u32) (0<<7)
#define SPI_FRAME_LSB						(u32) (1<<7)

//@ref SPI_DATA_define						/*CR1 - SSM*/
#define SPI_DATA_8_Bit						(u32) (0<<11)
#define SPI_DATA_16_Bit						(u32) (1<<11)

//@ref SPI_NSS_define						/*CR2 - DFF || CR1 - SSM, SSI*/
#define SPI_NSS_Hardware_Master_SS_ENABLE	(u32) (1<<2)
#define SPI_NSS_Hardware_Master_SS_DISABLE	(u32) (0<<2)
#define SPI_NSS_Hardware_Slave				(u32) (0<<9)
#define SPI_NSS_Software_Slave_RESET		(u32) ((1<<9) | (0<<8))
#define SPI_NSS_Software_Slave_SET			(u32) ((1<<9) | (1<<8))

//@ref SPI_POLARITY_define					/*CR1 - CPOL*/
#define SPI_IDEAL_High						(u32) (1<<1)
#define SPI_IDEAL_Low						(u32) (0<<1)


//@ref SPI_PHASE_define						/*CR1 - CPHA*/
#define SPI_PHASE_First_Edge				(u32) (0<<0)
#define SPI_PHASE_Second_Edge				(u32) (1<<0)

//@ref SPI_BAUDRATEPRESCALER_define			/*CR1 - BR*/
#define SPI_BAUDRATEPRESCALER_2				(u32) (0b000U << 3)
#define SPI_BAUDRATEPRESCALER_4				(u32) (0b001U << 3)
#define SPI_BAUDRATEPRESCALER_8				(u32) (0b010U << 3)
#define SPI_BAUDRATEPRESCALER_16			(u32) (0b011U << 3)
#define SPI_BAUDRATEPRESCALER_32			(u32) (0b100U << 3)
#define SPI_BAUDRATEPRESCALER_64			(u32) (0b101U << 3)
#define SPI_BAUDRATEPRESCALER_128			(u32) (0b110U << 3)
#define SPI_BAUDRATEPRESCALER_256			(u32) (0b111U << 3)

//@ref SPI_IRQ_define						/*CR2*/
#define SPI_IRQ_NONE						(u32) (0)
#define SPI_IRQ_TXEIE						(u32) (1 << 7)
#define SPI_IRQ_RXNEIE						(u32) (1 << 6)
#define SPI_IRQ_ERRIE						(u32) (1 << 5)




/*****************************************
----------------- APIs  -----------------
*****************************************/
void MCAL_SPI_Init(SPI_t* SPIx, SPI_Config_t* SPI_Cfg);
void MCAL_SPI_DeInit(SPI_t* SPIx);
void MCAL_SPI_GPIO_SetPins(SPI_t* SPIx);

void MCAL_SPI_SendData(SPI_t* SPIx, u16* pTxBuffer, enum SPI_Polling_mechanism PollingEn);
void MCAL_SPI_SendString(SPI_t* SPIx, u16* pTxString, u16 Delimiter);
void MCAL_SPI_ReceiveData(SPI_t* SPIx, u16* pRxBuffer, enum SPI_Polling_mechanism PollingEn);

void MCAL_SPI_TxRx(SPI_t* SPIx, u16 *pTxBuffer, enum SPI_Polling_mechanism PollingEn);















#endif /* INC_STM32F103C6_SPI_DRIVER_H_ */
