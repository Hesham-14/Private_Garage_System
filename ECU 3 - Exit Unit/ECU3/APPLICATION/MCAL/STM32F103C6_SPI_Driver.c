/*
 * STM32F103C6_SPI_Driver.c
 *
 *  Created on: Oct 16, 2023
 *      Author: hesham mohamed
 */

/*
 * STM32F103C6_USART_Driver.c
 *
 *  Created on: May 6, 2023
 *      Author: hesham mohamed
 */

/*************************************************
------------------ Includes  --------------------
 **************************************************/
#include "STM32F103C6_SPI_Driver.h"



/***************************************************************
---------------------- Global Variables  ----------------------
 ****************************************************************/
SPI_Config_t* Glob_SPI_Config[2] = {NULL, NULL};
SPI_Config_t Glob_SPI1_Config;
SPI_Config_t Glob_SPI2_Config;



/*************************************************************
---------------------- Generic Macros  ----------------------
 **************************************************************/
#define SPI1_Index		0
#define SPI2_Index		1
#define SPI_SR_TXE		(u16) (0x02)
#define SPI_SR_RXNE 	(u16) (0x01)


/***************************************************************
---------------------- Generic Functions  ----------------------
 ****************************************************************/

/**================================================================
 * @Fn			- MCAL_SPI_GPIO_SetPins
 * @brief 		- Initialize the GPIO Pins for SPI Interface
 * @param [in]	- SPIx: where x can be (1..3 depending on device used)
 * @retval 		- none
 */
void MCAL_SPI_GPIO_SetPins(SPI_t* SPIx)
{
	GPIO_PinConfig_t PinCfg;

	/* For SPI 1 */
	if(SPIx == SPI1)
	{
		//		PA4 : SPI1_NSS
		//		PA5 : SPI1_SCK
		//		PA6 : SPI1_MISO
		//		PA7 : SPI1_MOSI

		/* Master Mode */
		if(Glob_SPI_Config[SPI1_Index]->Mode == SPI_MODE_Master)
		{
			/* set NSS by checking which NSS Mode -> PA4 */
			if(Glob_SPI_Config[SPI1_Index]->NSS == SPI_NSS_Hardware_Master_SS_DISABLE)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_4;
				PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else if(Glob_SPI_Config[SPI1_Index]->NSS == SPI_NSS_Hardware_Master_SS_ENABLE)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_4;
				PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}


			/* Set SCK -> PA5 */
			PinCfg.GPIO_PinNumber = GPIO_PIN_5;
			PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			/* Set MISO -> PA6 */
			if(Glob_SPI_Config[SPI1_Index]->Comm_Mode == SPI_COMM_2Lines_FD)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_6;
				PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else
			{
				//todo for Half-Duplex
			}
			/* Set MOSI -> PA7 */
			if(Glob_SPI_Config[SPI1_Index]->Comm_Mode == SPI_COMM_2Lines_FD)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_7;
				PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else
			{
				//todo for Half-Duplex
			}
		}
		/* Slave Mode*/
		else
		{

			/* set NSS by checking which NSS Mode */
			if(Glob_SPI_Config[SPI1_Index]->NSS == SPI_NSS_Hardware_Slave)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_4;
				PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}

			/* Set SCK */
			PinCfg.GPIO_PinNumber = GPIO_PIN_5;
			PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			/* Set MISO */
			if(Glob_SPI_Config[SPI1_Index]->Comm_Mode == SPI_COMM_2Lines_FD)	//Full duplex slave (point to point)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_6;
				PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else
			{
				//todo for Half-Duplex
			}
			/* Set MOSI */
			if(Glob_SPI_Config[SPI1_Index]->Comm_Mode == SPI_COMM_2Lines_FD)	//Full duplex slave (point to point)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_7;
				PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else
			{
				//todo for Half-Duplex
			}
		}
	}
	/* For SPI 2 */
	else if(SPIx == SPI2)
	{
		//		PB12 : SPI2_NSS
		//		PB13 : SPI2_SCK
		//		PB14 : SPI2_MISO
		//		PB15 : SPI2_MOSI

		/* Master Mode */
		if(Glob_SPI_Config[SPI2_Index]->Mode == SPI_MODE_Master)
		{
			/* set NSS by checking which NSS Mode */
			switch(Glob_SPI_Config[SPI2_Index]->NSS)
			{
			case SPI_NSS_Hardware_Master_SS_DISABLE:
				PinCfg.GPIO_PinNumber = GPIO_PIN_12;
				PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
				break;
			case SPI_NSS_Hardware_Master_SS_ENABLE:
				PinCfg.GPIO_PinNumber = GPIO_PIN_12;
				PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
				break;
			}

			/* Set SCK */
			PinCfg.GPIO_PinNumber = GPIO_PIN_13;
			PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			/* Set MISO */
			if(Glob_SPI_Config[SPI2_Index]->Comm_Mode == SPI_COMM_2Lines_FD)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_14;
				PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else
			{
				//todo for Half-Duplex
			}
			/* Set MOSI */
			if(Glob_SPI_Config[SPI2_Index]->Comm_Mode == SPI_COMM_2Lines_FD)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_15;
				PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else
			{
				//todo for Half-Duplex
			}
		}
		/* Slave Mode*/
		else
		{

			/* set NSS by checking which NSS Mode */
			if(Glob_SPI_Config[SPI2_Index]->NSS == SPI_NSS_Hardware_Slave)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_12;
				PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}

			/* Set SCK */
			PinCfg.GPIO_PinNumber = GPIO_PIN_13;
			PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			/* Set MISO */
			if(Glob_SPI_Config[SPI2_Index]->Comm_Mode == SPI_COMM_2Lines_FD)	//Full duplex slave (point to point)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_14;
				PinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else
			{
				//todo for Half-Duplex
			}
			/* Set MOSI */
			if(Glob_SPI_Config[SPI2_Index]->Comm_Mode == SPI_COMM_2Lines_FD)	//Full duplex slave (point to point)
			{
				PinCfg.GPIO_PinNumber = GPIO_PIN_15;
				PinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
			else
			{
				//todo for Half-Duplex
			}
		}

	}
}


/***************************************************************
------------------ APIs Functions Definitions  -----------------
 ****************************************************************/

/**================================================================
 * @Fn			- MCAL_SPI_Init
 * @brief 		- Initialize the SPI Interface
 * @param [in]	- SPIx: where x can be (1..3 depending on device used)
 * @param [in]	- SPI_Cfg: All UART configuration
 * @retval 		- none
 * Note			- Support for SPI Full Duplex Master/Slave only & NSS HW/SW
 *				- in case of mater you have to configure pin and drive it
 */
void MCAL_SPI_Init(SPI_t* SPIx, SPI_Config_t* SPI_Cfg)
{
	/* Safety for Register*/
	u32 TempCR1 = 0;
	u32 TempCR2 = 0;

	/* Save Config & Enable CLK*/
	if(SPIx == SPI1)
	{
		Glob_SPI1_Config = *SPI_Cfg;
		Glob_SPI_Config[SPI1_Index] = &Glob_SPI1_Config;
		RCC_SPI1_CLK_EN();
	}
	else if (SPIx == SPI2)
	{
		Glob_SPI2_Config = *SPI_Cfg;
		Glob_SPI_Config[SPI1_Index] = &Glob_SPI2_Config;
		RCC_SPI2_CLK_EN();
	}

	/* Set SPI Configurations */
	//1. Set BaudRate Prescaler
	TempCR1 |= SPI_Cfg->Baudrate_Prescaler;

	//2. Set CLK Phase
	TempCR1 |= SPI_Cfg->Phase;

	//3. Set CLK Polarity
	TempCR1 |= SPI_Cfg->Polarity;

	//4. Set Data Size
	TempCR1 |= SPI_Cfg->Data_Size;

	//5. Set Frame Format
	TempCR1 |= SPI_Cfg->Frame_Format;

	//6. Set NSS
	if(SPI_Cfg->NSS == SPI_NSS_Hardware_Master_SS_ENABLE)
	{
		TempCR2 |= SPI_Cfg->NSS;
	}
	else if (SPI_Cfg->NSS == SPI_NSS_Hardware_Master_SS_DISABLE)
	{
		TempCR2 &= SPI_Cfg->NSS;
		//todo if you got unknown error check this step
	}
	else if (SPI_Cfg->NSS == SPI_NSS_Software_Slave_SET || SPI_Cfg->NSS == SPI_NSS_Software_Slave_RESET)
	{
		TempCR1 |= SPI_Cfg->NSS;
	}
	else if (SPI_Cfg->NSS == SPI_NSS_Hardware_Slave)
	{
		TempCR2 &= SPI_Cfg->NSS;
	}

	//7. Set Communication Mode
	TempCR1 |= SPI_Cfg->Comm_Mode;

	//8. Set Mode - Master/Slave
	TempCR1 |= SPI_Cfg->Mode;

	//9. Enable SPI - SPE Bit in CR1
	TempCR1 |= (1 << 6);

	//10. Set IRQ & Enable NVIC
	if(SPI_Cfg->IRQ_Enable != SPI_IRQ_NONE)
	{
		TempCR2 |= SPI_Cfg->IRQ_Enable;
		if(SPIx == SPI1){
			NVIC_SPI1_IRQ35_Enable();
		}
		else if(SPIx == SPI2){
			NVIC_SPI2_IRQ36_Enable();
		}
	}
	else
	{
		TempCR2 &= SPI_Cfg->IRQ_Enable;
	}

	/* Add values to the real resigter */
	SPIx->CR1 = TempCR1;
	SPIx->CR2 = TempCR2;

}



/**================================================================
 * @Fn			- MCAL_SPI_DeInit
 * @brief 		- Deinitialize the SPI Interface
 * @param [in]	- SPIx: where x can be (1..3 depending on device used)
 * @retval 		- none
 */
void MCAL_SPI_DeInit(SPI_t* SPIx)
{
	if(SPIx == SPI1)
	{
		NVIC_SPI1_IRQ35_Disable();
		RCC_SPI1_CLK_Reset();
	}
	else if (SPIx == SPI2)
	{
		NVIC_SPI2_IRQ36_Disable();
		RCC_SPI2_CLK_Reset();
	}
}


/**================================================================
 * @Fn			- MCAL_UART_GPIO_SetPins
 * @brief 		- Deinitialize the UART prephiral
 * @param [in]	- USARTx: where x can be (1..3 depending on device used)
 * @retval 		- none
 * Note			- Only support asynch. mode & clock at 8 MHz
 */



/**================================================================
 * @Fn			- MCAL_SPI_SendData
 * @brief 		- Send data frame through SPI
 * @param [in]	- SPIx: where x can be (1..3 depending on device used)
 * @param [in]	- pTxBuffer: includes the data to send
 * @param [in]	- PollingEn: define if the pooling is enable or not
 * @retval 		- none
 */
void MCAL_SPI_SendData(SPI_t* SPIx, u16* pTxBuffer, enum SPI_Polling_mechanism PollingEn)
{
	/* Check SPIx */
	u8 LOC_Index = 0;
	if(SPIx == SPI1)
		LOC_Index = SPI1_Index;
	else if(SPIx == SPI2)
		LOC_Index = SPI2_Index;


	/* Check Pooling */
	if(PollingEn == PollingEnable)
		while(!(SPIx->SR & SPI_SR_TXE));

	/* Write Data */
	SPIx->DR = (*pTxBuffer);

}

/**================================================================
 * @Fn			- MCAL_SPI_ReceiveData
 * @brief 		- Recieve data frame through SPI
 * @param [in]	- SPIx: where x can be (1..3 depending on device used)
 * @param [in]	- pRxBuffer: includes the data to recieve
 * @param [in]	- PollingEn: define if the pooling is enable or not
 * @retval 		- none
 */
void MCAL_SPI_ReceiveData(SPI_t* SPIx, u16* pRxBuffer, enum SPI_Polling_mechanism PollingEn)
{
	/* Check SPIx */
	u8 LOC_Index = 0;
	if(SPIx == SPI1)
		LOC_Index = SPI1_Index;
	else if(SPIx == SPI2)
		LOC_Index = SPI2_Index;


	/* Check Pooling */
	if(PollingEn == PollingEnable)
		while(!(SPIx->SR & SPI_SR_RXNE));

	/* Recieve Data */
	*pRxBuffer = SPIx->DR;


}

/**================================================================
 * @Fn			- MCAL_SPI_TxRx
 * @brief 		- Transmit & Recieve data frame through SPI
 * @param [in]	- SPIx: where x can be (1..3 depending on device used)
 * @param [in]	- pRxBuffer: includes the data to recieve
 * @param [in]	- PollingEn: define if the pooling is enable or not
 * @retval 		- none
 */
void MCAL_SPI_TxRx(SPI_t* SPIx, u16 *pTxBuffer, enum SPI_Polling_mechanism PollingEn)
{
	if(PollingEn == PollingEnable)
		while(!(SPIx->SR & 1 << 1));
	SPIx->DR = *pTxBuffer;

	if(PollingEn == PollingEnable)
		while(!(SPIx->SR & 1 ));
	*pTxBuffer = (SPIx->DR);
}
/**================================================================
 * @Fn			- MCAL_SPI_SendString
 * @brief 		- Send a string of data frame through SPI
 * @param [in]	- SPIx: where x can be (1..3 depending on device used)
 * @param [in]	- pTxString: includes the string to send
 * @param [in]	- PollingEn: define if the pooling is enable or not
 * @retval 		- none
 */
void MCAL_SPI_SendString(SPI_t* SPIx, u16* pTxString, u16 Delimiter)
{
	u16 LOC_Counter = 0;
	while(pTxString[LOC_Counter] != Delimiter)
	{
		MCAL_SPI_SendData(SPIx,(u16 *) &pTxString[LOC_Counter], PollingEnable);
		LOC_Counter++;
	}
}






/***************************************************************
------------------------ ISR Functions  ------------------------
 ****************************************************************/

void SPI1_IRQHandler()
{
	/*check What's causing the IRQ */
	struct S_IRQ_SRC irq_src;
	irq_src.TXE = ((SPI1->SR & (1<<1)) >> 1);
	irq_src.RXNE = ((SPI1->SR & (1<<0)) >> 0);
	irq_src.ERRI = ((SPI1->SR & (1<<4)) >> 4);

	/* Call ISR Function */
	Glob_SPI_Config[SPI1_Index]->P_ISR_CallBack(irq_src);
}

void SPI2_IRQHandler()
{
	/*check What's causing the IRQ */
	struct S_IRQ_SRC irq_src;
	irq_src.TXE = ((SPI2->SR & (1<<1)) >> 1);
	irq_src.RXNE = ((SPI2->SR & (1<<0)) >> 0);
	irq_src.ERRI = ((SPI2->SR & (1<<4)) >> 4);

	/* Call ISR Function */
	Glob_SPI_Config[SPI2_Index]->P_ISR_CallBack(irq_src);
}

