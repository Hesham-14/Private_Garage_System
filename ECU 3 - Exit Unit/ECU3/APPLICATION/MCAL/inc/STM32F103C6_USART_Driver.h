/*
 * STM32F103C6_USART_Driver.h
 *
 *  Created on: Oct 1, 2023
 *      Author: hesham mohamed
 */

#ifndef INC_STM32F103C6_USART_DRIVER_H_
#define INC_STM32F103C6_USART_DRIVER_H_

//===============================================
//Includes
//===============================================
#include "STM32F103C6_GPIO_Driver.h"
#include "STM32F103C6_RCC_Driver.h"
#include "STM32F103x6.h"
//===============================================
//User type definitions (structures), what i need to know from the user
//===============================================
typedef struct
{
	uint8_t USART_Mode				;		// Specifies the Tx/Rx mode
											// This prameter can be a value of @ref USART_MODE_define

	uint32_t BaudRate				;		// Specifies the baud rate for USART
											// This prameter can be a value of @ref USART_BAUDRATE_define

	uint8_t PayLoad					;		// Specifies the number of data bits transmitted or recieved in a frame
											// This prameter can be a value of @ref USART_PAYLOAD_define

	uint8_t Parity					;		// Specifies parity mode
											// This prameter can be a value of @ref USART_PARITY_define

	uint8_t StopBit					;		// Specifies the number of stop bits
											// This prameter can be a value of @ref UASRT_STOPBIT_define

	uint8_t HW_Flow_CTRL			;		// Specifies where the HW flow CTRL is enable or disable
											// This prameter can be a value of @ref HW_FLOW_CTRL_define

	uint8_t IRQ_EN					;		// enable or disable IRQ Tx/Rx
											// This prameter can be a value of @ref USART_IRQ__EN_define

	void(* IRQ_CallBack)(void)		;		// Set the C function which will be called once the IRQ happen


}UART_Config_t;


//===============================================
// Macros Configuration References
//===============================================

//@ref USART_MODE_define
#define USART_MODE_Rx					(uint32_t) (1<<2)
#define USART_MODE_Tx					(uint32_t) (1<<3)
#define USART_MODE_Tx_Rx				(uint32_t) (1<<2 | 1<<3)

//@ref USART_BAUDRATE_define
#define USART_BAUDRATE_2400				(uint32_t) 2400
#define USART_BAUDRATE_9600				(uint32_t) 9600
#define USART_BAUDRATE_19200			(uint32_t) 19200
#define USART_BAUDRATE_57600			(uint32_t) 57600
#define USART_BAUDRATE_115200			(uint32_t) 115200
#define USART_BAUDRATE_230400			(uint32_t) 230400
#define USART_BAUDRATE_460800			(uint32_t) 460800
#define USART_BAUDRATE_921600			(uint32_t) 921600
#define USART_BAUDRATE_2250000			(uint32_t) 2250000
#define USART_BAUDRATE_4500000			(uint32_t) 4500000

//@ref USART_PAYLOAD_define
#define USART_PAYLOAD_8B				(uint32_t) (0<<12)
#define USART_PAYLOAD_9B				(uint32_t) (1<<12)

//@ref USART_PARITY_define
#define USART_PARITY_NONE				(uint32_t) (0<<10)
#define USART_PARITY_EVEN				(uint32_t) (1<<10)
#define USART_PARITY_ODD				(uint32_t) (1<<10 | 1<<9)

//@ref UASRT_STOPBIT_define
#define UASRT_STOPBIT_half				(uint32_t) (1<<12)
#define UASRT_STOPBIT_1					(uint32_t) (0<<12)
#define UASRT_STOPBIT_1_half			(uint32_t) (3<<12)
#define UASRT_STOPBIT_2					(uint32_t) (2<<12)

//@ref HW_FLOW_CTRL_define
#define HW_FLOW_CTRL_CTS_NONE			(uint32_t) (0<<8 | 0<<9)
#define HW_FLOW_CTRL_CTS_EN				(uint32_t) (1<<9)
#define HW_FLOW_CTRL_RTS_EN				(uint32_t) (1<<8)
#define HW_FLOW_CTRL_RTS_CTS_EN			(uint32_t) (1<<8 | 1<<9)

//@ref USART_IRQ__EN_define
#define USART_IRQ__EN_NONE			(uint32_t) (0)
#define USART_IRQ__EN_TXE			(uint32_t) (1<<7)	//transmit data register empty
#define USART_IRQ__EN_TC			(uint32_t) (1<<6)	//transmission complete
#define USART_IRQ__EN_RXNEIE		(uint32_t) (1<<5)	//recieved data ready to read & overrun error detected
#define USART_IRQ__EN_PE			(uint32_t) (1<<8)	//parity error

enum USART_IRQ_Event
{
	USART_IRQ_TXE,		//transmit data register empty
	USART_IRQ_TC,		//transmission complete
	USART_IRQ_RXNE,		//recieved data ready to read
	USART_IRQ_ORE,		//overrun error detected
	USART_IRQ_PE		//parity error
};

enum UART_Polling_mechanism
{
	enable,
	disable
};


//===============================================
// APIs Supported by "MCAL USART DRIVER"
//===============================================

void MCAL_UART_Init(USART_t* USARTx, UART_Config_t* UART_Cfg);
void MCAL_UART_DeInit(USART_t* USARTx);
void MCAL_UART_GPIO_SetPins(USART_t* USARTx);

void MCAL_UART_SendData(USART_t* USARTx, uint16_t* pTxBuffer, enum UART_Polling_mechanism PollingEn);
void MCAL_UART_SendString(USART_t* USARTx, uint8_t* pTxString, uint16_t Delimiter);
void MCAL_UART_ReceiveData(USART_t* USARTx, uint16_t* pRxBuffer, enum UART_Polling_mechanism PollingEn);

void MCAL_UART_WAIT_TC (USART_t* USARTx);

void(* P_IRQ_CallBack)(enum USART_IRQ_Event flag);		//set the c function which will be called once the IRQ happen

//todo MCAL_USART_Init()
//todo MCAL_USART_DeInit()
//todo MCAL_USART_DMA_Init()

#endif /* INC_STM32F103C6_USART_DRIVER_H_ */
