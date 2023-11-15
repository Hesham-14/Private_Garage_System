/*
 * MyApp.c
 *
 *  Created on: Nov 6, 2023
 *      Author: hesham mohamed
 */


/* =========================================================================
 *                                  Includes
 * =========================================================================*/
#include "MyApp.h"



/* =========================================================================
 *                                  Typedefs
 * =========================================================================*/




/* =========================================================================
 *                             Global Variables
 * =========================================================================*/
UART_Config_t uartCfg;
uint8_t Slave_TxData = 'm';
uint8_t Glob_Available_Slots = 3;

/********* ADMIN Vars *********/


/********* User Vars *********/


/* =========================================================================
 * 						Generic Functions Prototypes
 * =========================================================================*/
static void APP_CLK_init(void);
static void APP_UART_init(void);
static void APP_SPI_init(void);
static void APP_EXTI_init(void);
static void APP_LEDs_Init(void);
static void APP_Start_Tringer_init(void);
static void APP_Start_Wait_Trigger(void);

void UART_Welcome_Message(void);
void SPI_Master_Welcome_Message(void);
void SPI_Slave_Welcome_Message(void);
void LCD_Waiting_Message(void);
void LCD_Welcome_Message(void);

void ECU1_APP_Valid_Access(void);
void ECU1_APP_InValid_Acess(void);

void delay_ms(uint32_t time);

/********* Callback Definition *********/
static void ECU1_UART_IRQ_CallBack(void);
static void ECU1_SPI_IRQ_CallBack(struct S_IRQ_SRC irq_src);


/* =========================================================================
 * 								Generic Functions
 * =========================================================================*/


/* ==============================================
 * 		Initialization  & Start FCNs
 * ==============================================*/
static void APP_CLK_init(void)
{
	//1. GPIO port A clock enable
	RCC_GPIOA_CLK_EN();

	//2. GPIO port B clock enable
	RCC_GPIOB_CLK_EN();

	//3. AFIO clock enable
	RCC_AFIO_CLK_EN();
}

static void APP_UART_init(void)
{
	/********* UART Configurations *********/
	//PA09 Tx
	//PA10 Rx
	//PA11 CTS
	//PA12 RTS

	uartCfg.BaudRate = USART_BAUDRATE_115200;
	uartCfg.HW_Flow_CTRL = HW_FLOW_CTRL_CTS_NONE;
	uartCfg.IRQ_EN = USART_IRQ__EN_RXNEIE;
	uartCfg.IRQ_CallBack = ECU1_UART_IRQ_CallBack;
	uartCfg.Parity = USART_PARITY_NONE;
	uartCfg.PayLoad = USART_PAYLOAD_8B;
	uartCfg.StopBit = UASRT_STOPBIT_1;
	uartCfg.USART_Mode = USART_MODE_Tx_Rx;

	MCAL_UART_Init(USART1, &uartCfg);
	MCAL_UART_GPIO_SetPins(USART1);

	UART_Welcome_Message();
}

static void APP_SPI_init(void)
{
	/*------ SPI1 Configurations ------*/
	//PA4 : NSS
	//PA5 : SCK
	//PA6 : MISO
	//PA7 : MOSI

	SPI_Config_t spi1Cfg;

	/* Common Configuration */
	spi1Cfg.Phase = SPI_PHASE_Second_Edge;
	spi1Cfg.Polarity = SPI_IDEAL_High;
	spi1Cfg.Frame_Format = SPI_FRAME_MSB;
	spi1Cfg.Data_Size = SPI_DATA_8_Bit;
	spi1Cfg.Baudrate_Prescaler = SPI_BAUDRATEPRESCALER_8;
	spi1Cfg.Comm_Mode = SPI_COMM_2Lines_FD;

#ifdef MCU_Act_As_Master
	spi1Cfg.Mode = SPI_MODE_Master;
	spi1Cfg.IRQ_Enable = SPI_IRQ_NONE;
	spi1Cfg.P_ISR_CallBack = NULL;
	spi1Cfg.NSS = SPI_NSS_Software_Slave_SET;

	/* Configure SS on PA4 by GPIO */
	GPIO_PinConfig_t pinCfg;
	pinCfg.GPIO_PinNumber = GPIO_PIN_4;
	pinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
	pinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(GPIOA, &pinCfg);
	/* Force the SS to high idle Mode */
	MCAL_GPIO_WritePin(GPIOA, pinCfg.GPIO_PinNumber, GPIO_PIN_SET);
#endif

#ifdef MCU_Act_As_Slave
	spi1Cfg.Mode = SPI_MODE_Slave;
	spi1Cfg.IRQ_Enable = SPI_IRQ_RXNEIE;
	spi1Cfg.P_ISR_CallBack = ECU1_SPI_IRQ_CallBack;
	spi1Cfg.NSS = SPI_NSS_Hardware_Slave;
#endif

	MCAL_SPI_Init(SPI1, &spi1Cfg);
	/* Set GPIO Pins for SPI */
	MCAL_SPI_GPIO_SetPins(SPI1);

	//LCD_clear_screen_2();
	//HAL_LCD_Send_String("SPI READY       MODE: Slave");
	//delay_ms(500);
}

static void APP_EXTI_init(void)
{
#ifdef MCU_Act_As_Slave
	GPIO_PinConfig_t pin_cfg;
	pin_cfg.GPIO_PinNumber = EXTI_Pin;
	pin_cfg.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
	pin_cfg.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(EXTI_Port, &pin_cfg);
#endif
}

static void APP_LEDs_Init(void)
{
	// Setup Valid LED
	GPIO_PinConfig_t pinCfg;
	pinCfg.GPIO_PinNumber = VALID_LED;
	pinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
	pinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(LED_PORT, &pinCfg);

	// Setup InValid LED
	pinCfg.GPIO_PinNumber = INVALID_LED;
	MCAL_GPIO_Init(LED_PORT, &pinCfg);
}

static void APP_Start_Tringer_init(void)
{
	GPIO_PinConfig_t pinCfg;
	pinCfg.GPIO_PinNumber = Start_Trigger_Pin;
	pinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(Start_Trigger_PORT, &pinCfg);
}

static void APP_Start_Wait_Trigger(void)
{
	while(MCAL_GPIO_ReadPin(Start_Trigger_PORT, Start_Trigger_Pin) == 0)
	{

	}
}

/* ==============================================
 * 					LCD FCNs
 * ==============================================*/
void UART_Welcome_Message(void)
{
	//LCD_clear_screen_2();
	//HAL_LCD_Send_String("UART is READY ");
	//delay_ms(50);

	MCAL_UART_SendString(USART1, addString("**************** WELCOME TO ETCH GARAGE :) ****************\r\n\0"), '\0');
	MCAL_UART_SendString(USART1, addString("Enter ID: "), '\0');
}

void SPI_Master_Welcome_Message(void)
{
	LCD_clear_screen_2();
	HAL_LCD_Send_String("SPI is READY    MODE: MASTER");
	delay_ms(500);
}

void SPI_Slave_Welcome_Message(void)
{
	LCD_clear_screen_2();
	HAL_LCD_Send_String("SPI is READY    MODE: Slave");
	delay_ms(500);
}

void LCD_Waiting_Message(void)
{
	LCD_clear_screen_2();
	HAL_LCD_Send_String("System Getting  Ready ......");
}

void LCD_Welcome_Message(void)
{
	LCD_clear_screen_2();
	HAL_LCD_Send_String("Welcome Sir,    Scan Your ID :)");
}

/* ==============================================
 * 					SYSTEM FCNs
 * ==============================================*/
void ECU1_APP_Valid_Access(void)
{
	Glob_Available_Slots--;
	/* Turn ON Valid LED */
	MCAL_GPIO_WritePin(LED_PORT, VALID_LED, GPIO_PIN_SET);

	LCD_clear_screen_2();
	MCAL_UART_SendString(USART1, "*** Valid ID ***\r\n", '\0');
	HAL_LCD_Send_String("Welcome Sir     - Opening Gate");


	Servo1_Entry_Gate(UP);

	LCD_clear_screen_2();
	HAL_LCD_Send_String("Please,         Move Your Car :)");

	/* Wait until Sensor is off */
	while(Motion_Sensor_Get_State() != 0);
	LCD_clear_screen_2();
	HAL_LCD_Send_String("- Closing Gate");

	Servo1_Entry_Gate(DOWN);

	/* Turn OFF Valid LED */
	MCAL_GPIO_WritePin(LED_PORT, VALID_LED, GPIO_PIN_RESET);

	Glob_Available_Slots--;

	MCAL_UART_SendString(USART1, "\r\n**************** WELCOME TO ETCH GARAGE :) ****************\r\n", '\0');
	MCAL_UART_SendString(USART1, addString("Enter ID: "), '\0');

	LCD_Welcome_Message();
}

void ECU1_APP_InValid_Acess(void)
{
	/* Turn ON Invalid LED */
	MCAL_GPIO_WritePin(LED_PORT, INVALID_LED, GPIO_PIN_SET);

	LCD_clear_screen_2();
	HAL_LCD_Send_String("Invalid Access");
	MCAL_UART_SendString(USART1, "*** Received InValid ID ***\r\n", '\0');

	Buzzer_Start();

	Servo1_Entry_Gate(DOWN);

	/* Turn OFF Invalid LED */
	MCAL_GPIO_WritePin(LED_PORT, INVALID_LED, GPIO_PIN_RESET);

	MCAL_UART_SendString(USART1, addString("\r\n**************** WELCOME TO ETCH GARAGE :) ****************\r\n\0"), '\0');
	MCAL_UART_SendString(USART1, addString("Enter ID: "), '\0');

	LCD_Welcome_Message();
}

/* ==============================================
 * 					GENERIC FCNs
 * ==============================================*/
void delay_ms(uint32_t time){
	uint32_t i, j;
	for (i = 0; i < time; i++)
		for (j = 0; j < 255; j++)
			;
}

/* =========================================================================
 * 							APIs Functions Definitions
 * =========================================================================*/

/*************************************************************************
 * @Fn			- ECU1_Dashboard_APP_SETUP
 * @brief 		- This function is used to make MCAL & HAL initialization.
 * @param [in]	- none
 * @retval 		- none
 **************************************************************************/
void ECU1_Dashboard_APP_SETUP(void)
{
	APP_CLK_init();

	//HAL initialization
	HAL_LCD_Init();
	Servo1_Entry_Gate_Init();

	LCD_Waiting_Message();

	APP_LEDs_Init();
	//HAL_keypad_init();

	//MCAL initialization
	MCAL_Timer2_init();
	APP_UART_init();
	APP_SPI_init();
	APP_EXTI_init();

	APP_Start_Tringer_init();
	APP_Start_Wait_Trigger();

	LCD_Welcome_Message();
}
void ECU1_Dashboard_APP_RUN(void)
{
	while(1)
	{

	}
}



/* =========================================================================
 * 								Callback Functions
 * =========================================================================*/
static void ECU1_UART_IRQ_CallBack(void)
{
	uint8_t check_Termination, LOC_RxData;
	MCAL_UART_ReceiveData(USART1, &LOC_RxData, enable);
	MCAL_UART_SendData(USART1, &LOC_RxData, enable);

	Slave_TxData = LOC_RxData;
	check_Termination = LOC_RxData;

	/* inform Master */
	MCAL_GPIO_WritePin(EXTI_Port, EXTI_Pin, GPIO_PIN_SET);
	MCAL_GPIO_WritePin(EXTI_Port, EXTI_Pin, GPIO_PIN_RESET);

	if(check_Termination == 13)
	{
		MCAL_GPIO_WritePin(EXTI_Port, EXTI_Pin, GPIO_PIN_SET);
		MCAL_GPIO_WritePin(EXTI_Port, EXTI_Pin, GPIO_PIN_RESET);
	}

//	if(check_Termination == 13)
//	{
//		MCAL_UART_SendString(USART1, "*** Requesting Access ***\r\n", '\0');
//	}
}

static void ECU1_SPI_IRQ_CallBack(struct S_IRQ_SRC irq_src)
{
#ifdef MCU_Act_As_Slave
	if(irq_src.RXNE)
	{
		MCAL_SPI_TxRx(SPI1, &Slave_TxData, PollingDisable);

		if(Slave_TxData == 'T')
			ECU1_APP_Valid_Access();
		else if(Slave_TxData == 'F')
			ECU1_APP_InValid_Acess();

		Slave_TxData = 'w';
	}

#endif
}

