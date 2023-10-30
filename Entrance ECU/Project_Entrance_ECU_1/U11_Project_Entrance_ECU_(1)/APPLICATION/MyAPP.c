
/*
 * MyAPP.c
 *
 *  Created on: Oct 3, 2023
 *  Author: hesham mohamed
 */


/* ==================================================
 *					   Includes
 * =================================================*/
#include "MyAPP.h"

/* ==================================================
 *					Global Variables
 * =================================================*/
char received_data;
uint8_t x=1, spi_data=SPI_MASTER_ACK, *spi_string, uart_data, *uart_string;
uint8_t Glob_key_pressed = NULL;
unsigned int Glob_IQ_Flag=0;
uint8_t Glob_ID_SCANNED_Flag=0;
uint16_t Glob_UART_char =0;
int i=0;

uint8_t Glob_Available_Slots = MAX_SLOTS_IN_GARAGE;

/********* User Vars *********/


/* =====================================================
 *				Generic Functions Prototypes
 * ====================================================*/
void UART_Welcome_Message();
void SPI_Master_Welcome_Message();
void SPI_Slave_Welcome_Message();
void LCD_Welcome_Message();
void APP_SPI_Init();
void APP_UART_Init();
void APP_LEDs_Init();

/* =====================================================
 *					Generic Functions
 * ====================================================*/
void UART_Welcome_Message()
{
	LCD_clear_screen();
	LCD_write_string("UART is READY ");
	_delay_ms(100);
	
	MCAL_USART_Transmit_string("\r\n**************** WELCOME TO ETCH GARAGE :) ****************\r\n");
	MCAL_USART_Transmit_string("Enter ID: ");
}

void SPI_Master_Welcome_Message()
{
	LCD_clear_screen();
	LCD_write_string("SPI is READY ");
	_delay_ms(200);
	LCD_GOTO(2,0);
	LCD_write_string("MODE: MASTER");
	_delay_ms(500);
}

void SPI_Slave_Welcome_Message()
{
	LCD_clear_screen();
	LCD_write_string("SPI is READY ");
	_delay_ms(200);
	LCD_GOTO(2,0);
	LCD_write_string("MODE: SLAVE");
	_delay_ms(500);
	LCD_clear_screen();
	LCD_write_string("Received: ");
	_delay_ms(50);
}

void LCD_Welcome_Message()
{
	LCD_clear_screen();
	LCD_write_string("Welcome Sir");
	_delay_ms(200);
	LCD_GOTO(2,0);
	LCD_write_string("I'm READY... :)");
	_delay_ms(200);
	
	LCD_clear_screen();
	LCD_write_string("Available");
	LCD_GOTO(2,0);
	LCD_write_string("Slots: ");
	LCD_display_number(Glob_Available_Slots);
	_delay_ms(750);
}


/* =============================================
 *				Initialization FCNs
 * ============================================*/
void APP_SPI_Init()
{
	/*SPI Configuration*/
	SPI_Config_t* SPI_Cfg;
	SPI_Cfg->Master_Speed = SPI_MASTER_SPEED_f_4;
	SPI_Cfg->CLK_Speed = SPI_CLK_SPEED_Normal;
	SPI_Cfg->Data_Order = SPI_DATA_ORDER_MSB;
	SPI_Cfg->CLK_Polarity = SPI_CLK_Polarity_Leading_Falling;
	
	/*SPI Mode Configuration*/
	#ifdef MCU_SPI_Master
	SPI_Cfg->SPI_Mode = SPI_MODE_MASTER;
	SPI_Cfg->IRQ_Enable = SPI_IRQ_None;
	SPI_Cfg->p_IRQ_CallBack = NULL;
	#endif
	
	#ifdef MCU_SPI_Slave
	SPI_Cfg->SPI_Mode = SPI_MODE_SLAVE;
	SPI_Cfg->IRQ_Enable = SPI_IRQ_None;
	SPI_Cfg->p_IRQ_CallBack = NULL;
	
	/* Set Entrance EXTI pin */
	Set_Bit(EXTI_DDR, Entrance_EXTI);
	Clear_Bit(EXTI_Port, Entrance_EXTI);
	#endif
	
	MCAL_SPI_Init(SPI_Cfg);
	
	#ifdef MCU_SPI_Master
	SPI_Master_Welcome_Message();
	/* Set Idle High (Active High), set "SS" by 1 */
	PORTB |= (1<<SS);
	#endif
	
	#ifdef MCU_SPI_Slave
	SPI_Slave_Welcome_Message();
	#endif
}

void APP_UART_Init()
{
	USART_Config_t uart_cfg;
	uart_cfg.SYNCH_MODE = USART_ASYNCH;
	uart_cfg.TRANSMIT = USART_TRANSMIT_ENABLE;
	uart_cfg.RECIEVE = USART_RECIEVE_ENABLE;
	uart_cfg.CHAR_SIZE = USART_8_Bit_CHAR;
	uart_cfg.SPEED_MODE = USART_NORMAL_SPEED;
	uart_cfg.PARITY_MODE = USART_NO_PARITY;
	uart_cfg.STOPBIT = USART_1_STOPBIT;
	uart_cfg.RECIEVE_INT = USART_RECIEVER_ENABLE;
	uart_cfg.TRANSMITER_INT = USART_TRANSMITER_DISABLE;
	MCAL_USART_Init(uart_cfg);
	MCAL_USART_callback_Rec_Int(ECU1_USART_IRQ_CallBack);
	
	UART_Welcome_Message();
}

void APP_LEDs_Init()
{
	// Setup Valid LED
	Set_Bit(LED_DDR, VALID_LED);
	Set_Bit(LED_DDR, INVALID_LED);
}

/* ==============================================
 * 					SYSTEM FCNs
 * ==============================================*/





/* =====================================================
 *				APIs Functions Definitions
 * ====================================================*/
/*************************************************************************
 * @Fn			- ECU1_APP_SETUP
 * @brief 		- This function is used to make MCAL & HAL initialization.
 * @param [in]	- none
 * @retval 		- none
 **************************************************************************/
void ECU1_APP_SETUP(void)
{
	// enable global interrupt
	sei();
	
	//Initialization
	LCD_init();
	Gate_Init();
	APP_LEDs_Init();
	APP_UART_Init();
	APP_SPI_Init();
}

void ECU1_APP_RUNNING(void)
{
	while(1)
	{
		LCD_Welcome_Message();
	}
}

void ECU1_APP_Valid_Access(Access_Status_t Status)
{
	Glob_Available_Slots--;	
	/* Turn ON Valid LED */
	Set_Bit(LED_PORT, VALID_LED);
	LCD_clear_screen();
	LCD_write_string("Valid Access");
	LCD_GOTO(2,0);
	LCD_write_string("Opening Gate");
	Gate_Open();
	LCD_clear_screen();
	LCD_write_string("Please,");
	LCD_GOTO(2,0);
	LCD_write_string("Move Your Car :)");
	
	/* Wait until Sensor is off */
	while(Motion_Sensor_Get_State() != 0);
	LCD_clear_screen();
	LCD_write_string("Closing Gate");
	Gate_Close();
	
	/* Turn OFF Valid LED */
	Clear_Bit(LED_PORT, VALID_LED);
}

void ECU1_APP_InValid_Acess(Access_Status_t Status)
{
	/* Turn ON Invalid LED */
	Set_Bit(LED_PORT, INVALID_LED);
	LCD_clear_screen();
	LCD_write_string("Invalid Access");
	Buzzer_Start();
	Gate_Close();
	
	/* Turn OFF Invalid LED */
	Clear_Bit(LED_PORT, INVALID_LED);
}



/* =====================================================
 *					Callback Functions
 * ====================================================*/
void ECU1_USART_IRQ_CallBack(void)
{
	MCAL_USART_Receive(&uart_data);
	
	LCD_clear_screen();
	LCD_write_string("Sent: ");
	LCD_write_char(uart_data);
	
	/* Inform Master */
	Set_Bit(EXTI_Port, Entrance_EXTI);
	
	MCAL_SPI_Tx_Rx(&uart_data, SPI_Polling_Disable);
	
	LCD_write_string("-R: ");
	LCD_write_char(uart_data);
	_delay_ms(500);
	
	Clear_Bit(EXTI_Port, Entrance_EXTI);
}