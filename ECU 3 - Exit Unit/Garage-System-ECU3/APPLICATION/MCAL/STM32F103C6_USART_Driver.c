/*
 * STM32F103C6_USART_Driver.c
 *
 *  Created on: May 6, 2023
 *      Author: hesham mohamed
 */


#include "STM32F103C6_USART_Driver.h"
/*
 * =========================================================================
 * 								Generic Macros
 * =========================================================================
 */
#define USARTDIV(_PCLK_, _BAUD_)           (uint32_t)( _PCLK_/(16 * _BAUD_))
#define USARTDIV_MUL100(_PCLK_, _BAUD_)    (uint32_t)((25* _PCLK_) / (4 * _BAUD_))
#define Div_Mantissa_MUL100(_PCLK_, _BAUD_) (uint32_t)(USARTDIV(_PCLK_, _BAUD_)* 100)
#define Div_Mantissa(_PCLK_, _BAUD_)    (uint32_t)(USARTDIV(_PCLK_, _BAUD_))
#define Div_Fraction(_PCLK_, _BAUD_)    (uint32_t)( ( (USARTDIV_MUL100(_PCLK_, _BAUD_) - Div_Mantissa_MUL100(_PCLK_, _BAUD_)) * 16) / 100)
#define USART_BRR_Register(_PCLK_, _BAUD_) ((Div_Mantissa(_PCLK_, _BAUD_) << 4) | (Div_Fraction(_PCLK_, _BAUD_) & 0xF ))


/*
 * =========================================================================
 * 								Global Variables
 * =========================================================================
 */
UART_Config_t* GLOBAL_UART1_Config = NULL;
UART_Config_t* GLOBAL_UART2_Config = NULL;
UART_Config_t* GLOBAL_UART3_Config = NULL;
uint32_t Pclk;

/*
 * =========================================================================
 * 								Generic Functions
 * =========================================================================
 */


//==========================================================================
/*
 * =========================================================================
 * 								APIs Functions Definitions
 * =========================================================================
 */

/**================================================================
 * @Fn			- MCAL_UART_Init
 * @brief 		- Initialize the UART prephiral
 * @param [in]	- USARTx: where x can be (1..3 depending on device used)
 * @param [in]	- UART_Cfg: All UART configuration
 * @retval 		- none
 * Note			- Only support asynch. mode & clock at 8 MHz
 */
void MCAL_UART_Init(USART_t* USARTx, UART_Config_t* UART_Cfg)
{
	//0- save uart config in global variable
	if(USARTx == USART1)
		GLOBAL_UART1_Config = UART_Cfg;
	else if(USARTx == USART2)
		GLOBAL_UART2_Config = UART_Cfg;
	else
		GLOBAL_UART3_Config = UART_Cfg;

	//1- enable UART peripheral
	if(USARTx == USART1)
		RCC_USART1_CLK_EN();
	else if(USARTx == USART2)
		RCC_USART2_CLK_EN();
	else if(USARTx == USART3)
		RCC_USART3_CLK_EN();

	//2- enable UART_CR module
	USARTx->CR1 |= (1 << 13);

	//3- enable UART_CR mode
	USARTx->CR1 |= UART_Cfg->USART_Mode;

	//4- enable UART_CR payload
	USARTx->CR1 |= UART_Cfg->PayLoad;

	//5- enable UART_CR parity
	USARTx->CR1 |= UART_Cfg->Parity;

	//6- enable UART_CR2 stop bits
	USARTx->CR2 |= UART_Cfg->StopBit;

	//7- enable UART_CR3 flow ctrl
	USARTx->CR3 |= UART_Cfg->HW_Flow_CTRL;

	//8- Configure BRR value
	if(USARTx == USART1)
	{
		Pclk = MCAL_RCC_GetPCLK2Freq();
	}
	else
	{
		Pclk = MCAL_RCC_GetPCLK1Freq();
	}
	USARTx->BRR = USART_BRR_Register(Pclk, UART_Cfg->BaudRate);

	//9- enable UART interrupt and NVIC
	if(UART_Cfg->IRQ_EN != USART_IRQ__EN_NONE)
	{
		USARTx->CR1 |= UART_Cfg->IRQ_EN;

		if(USARTx == USART1)
			NVIC_USART1_IRQ37_Enable();
		else if(USARTx == USART2)
			NVIC_USART2_IRQ38_Enable();
		else if(USARTx == USART3)
			NVIC_USART3_IRQ39_Enable();
	}
}



/**================================================================
 * @Fn			- MCAL_UART_DeInit
 * @brief 		- Deinitialize the UART prephiral
 * @param [in]	- USARTx: where x can be (1..3 depending on device used)
 * @retval 		- none
 * Note			- Only support asynch. mode & clock at 8 MHz
 */
void MCAL_UART_DeInit(USART_t* USARTx)
{
	//check UARTx
	//then, disable NVIC and reset RCC for UART
	if(USARTx == USART1)
	{
		RCC_USART1_CLK_Reset();
		NVIC_USART1_IRQ37_Disable();
	}
	else if(USARTx == USART2)
	{
		RCC_USART2_CLK_Reset();
		NVIC_USART2_IRQ38_Disable();
	}
	else if(USARTx == USART2)
	{
		RCC_USART3_CLK_Reset();
		NVIC_USART3_IRQ39_Disable();
	}
}

/**================================================================
 * @Fn			- MCAL_UART_GPIO_SetPins
 * @brief 		- Deinitialize the UART prephiral
 * @param [in]	- USARTx: where x can be (1..3 depending on device used)
 * @retval 		- none
 * Note			- Only support asynch. mode & clock at 8 MHz
 */
void MCAL_UART_GPIO_SetPins(USART_t* USARTx)
{
	GPIO_PinConfig_t PinCfg;

	if(USARTx == USART1)
	{
		//PA9 TX
		//PA10 RX
		//PA11 CTS
		//PA12 RTS

		//PA9 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_9;
		PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		//PA10 RX
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_Mode = GPIO_MODE_AF_INPUT;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		if(GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_CTS_EN || GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_CTS_EN)
		{
			//PA11 CTS
			PinCfg.GPIO_PinNumber = GPIO_PIN_11;
			PinCfg.GPIO_Mode = GPIO_MODE_AF_INPUT;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}

		if(GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_EN || GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_CTS_EN)
		{
			//PA12 RTS
			PinCfg.GPIO_PinNumber = GPIO_PIN_12;
			PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
	}
	else if(USARTx == USART2)
	{
		//PA2 TX
		//PA3 RX
		//PA0 CTS
		//PA1 RTS

		//PA2 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_2;
		PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		//PA3 RX
		PinCfg.GPIO_PinNumber = GPIO_PIN_3;
		PinCfg.GPIO_Mode = GPIO_MODE_AF_INPUT;
		MCAL_GPIO_Init(GPIOA, &PinCfg);

		if(GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_CTS_EN || GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_CTS_EN)
		{
			//PA0 CTS
			PinCfg.GPIO_PinNumber = GPIO_PIN_0;
			PinCfg.GPIO_Mode = GPIO_MODE_AF_INPUT;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}

		if(GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_EN || GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_CTS_EN)
		{
			//PA1 RTS
			PinCfg.GPIO_PinNumber = GPIO_PIN_1;
			PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);
		}
	}
	else if(USARTx == USART3)
	{
		//PB10 TX
		//PB11 RX
		//PB13 CTS
		//PB14 RTS

		//PB10 TX
		PinCfg.GPIO_PinNumber = GPIO_PIN_10;
		PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
		PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		//PB11 RX
		PinCfg.GPIO_PinNumber = GPIO_PIN_11;
		PinCfg.GPIO_Mode = GPIO_MODE_AF_INPUT;
		MCAL_GPIO_Init(GPIOB, &PinCfg);

		if(GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_CTS_EN || GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_CTS_EN)
		{
			//PB13 CTS
			PinCfg.GPIO_PinNumber = GPIO_PIN_13;
			PinCfg.GPIO_Mode = GPIO_MODE_AF_INPUT;
			MCAL_GPIO_Init(GPIOB, &PinCfg);
		}

		if(GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_EN || GLOBAL_UART1_Config->HW_Flow_CTRL == HW_FLOW_CTRL_RTS_CTS_EN)
		{
			//PB14 RTS
			PinCfg.GPIO_PinNumber = GPIO_PIN_14;
			PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOB, &PinCfg);
		}
	}
}


/**================================================================
 * @Fn			- MCAL_UART_SendData
 * @brief 		- Deinitialize the UART prephiral
 * @param [in]	- USARTx: where x can be (1..3 depending on device used)
 * @param [in]	- pTxBuffer: includes the data to send
 * @param [in]	- PollingEn: define if the pooling is enable or not
 * @retval 		- none
 * Note			- Only support asynch. mode & clock at 8 MHz
 */
void MCAL_UART_SendData(USART_t* USARTx, uint16_t* pTxBuffer, enum UART_Polling_mechanism PollingEn)
{

	//1. wait TXE flag in SR until to be set
	if(PollingEn == enable)
		while(!(USARTx->SR & 1<<7));

	//0. select which USARTx
	UART_Config_t* USARTx_Config = NULL;
	if(USARTx == USART1)
		USARTx_Config = GLOBAL_UART1_Config;
	else if(USARTx == USART2)
		USARTx_Config = GLOBAL_UART2_Config;
	else if(USARTx == USART3)
		USARTx_Config = GLOBAL_UART3_Config;

	//2. set word length for the DR
	if(GLOBAL_UART1_Config->PayLoad == USART_PAYLOAD_9B)
	{
		USARTx->DR = (*pTxBuffer & (uint16_t) 0x01FF);
	}
	else //8 bit word length
	{
		USARTx->DR = (*pTxBuffer & (uint16_t) 0xFF);
	}
}

void MCAL_UART_SendString(USART_t* USARTx, uint8_t* pTxString, uint16_t Delimiter)
{
	uint16_t LOC_Counter = 0;
	while(pTxString[LOC_Counter] != Delimiter)
	{
		MCAL_UART_SendData(USARTx,(uint16_t *) &pTxString[LOC_Counter], enable);
		LOC_Counter++;
	}
}

void MCAL_UART_ReceiveData(USART_t* USARTx, uint16_t* pRxBuffer, enum UART_Polling_mechanism PollingEn)
{
	//0. wait TXE flag in SR until to be set
	if(PollingEn == enable)
		while(!(USARTx->SR & 1<<5));

	//1. select which USARTx
	UART_Config_t* USARTx_Config = NULL;
	if(USARTx == USART1)
		USARTx_Config = GLOBAL_UART1_Config;
	else if(USARTx == USART2)
		USARTx_Config = GLOBAL_UART2_Config;
	else if(USARTx == USART3)
		USARTx_Config = GLOBAL_UART3_Config;


	//2. check word length
	if(USARTx_Config->PayLoad == USART_PAYLOAD_9B)
	{
		if(USARTx_Config->Parity == USART_PARITY_NONE)
			*((uint16_t*) pRxBuffer) = (USARTx->DR);

		else //parity ON
			*((uint16_t*) pRxBuffer) = (USARTx->DR & (uint8_t) 0xFF);
	}

	else //8 bit word length
	{
		if(USARTx_Config->Parity == USART_PARITY_NONE)
			*((uint16_t*) pRxBuffer) = (USARTx->DR & (uint8_t) 0xFF);

		else //parity ON
			*((uint16_t*) pRxBuffer) = (USARTx->DR & (uint8_t) 0x7F);
	}

}

void MCAL_UART_WAIT_TC (USART_t* USARTx)
{
	//wait until TC is set in the SR
	while(! (USARTx->SR & 1<<6));
}



/*
 * =========================================================================
 * 								ISR Functions
 * =========================================================================
 */

void USART1_IRQHandler(void)
{
	GLOBAL_UART1_Config->IRQ_CallBack();
}

void USART2_IRQHandler(void)
{
	GLOBAL_UART2_Config->IRQ_CallBack();
}

void USART3_IRQHandler(void)
{
	GLOBAL_UART3_Config->IRQ_CallBack();
}


