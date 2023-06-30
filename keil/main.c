#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"
int read_keyboard();
uint16_t Read_ADC();
void send_spi(uint16_t param1, uint16_t param2, uint16_t param3);
void Init_KeyPad(){
	GPIO_InitTypeDef PinConfig;
	PinConfig.Pin = GPIO_PIN_0;
	PinConfig.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfig.Pull = GPIO_NOPULL;
	PinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &PinConfig);
	
	PinConfig.Pin = GPIO_PIN_1;
	PinConfig.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfig.Pull = GPIO_NOPULL;
	PinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &PinConfig);
	
	PinConfig.Pin = GPIO_PIN_2;
	PinConfig.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfig.Pull = GPIO_NOPULL;
	PinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &PinConfig);
	
	PinConfig.Pin = GPIO_PIN_3;
	PinConfig.Mode = GPIO_MODE_INPUT;
	PinConfig.Pull = GPIO_PULLDOWN;
	PinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &PinConfig);
	
	PinConfig.Pin = GPIO_PIN_4;
	PinConfig.Mode = GPIO_MODE_INPUT;
	PinConfig.Pull = GPIO_PULLDOWN;
	PinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &PinConfig);
	
	PinConfig.Pin = GPIO_PIN_5;
	PinConfig.Mode = GPIO_MODE_INPUT;
	PinConfig.Pull = GPIO_PULLDOWN;
	PinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &PinConfig);
	
	PinConfig.Pin = GPIO_PIN_6;
	PinConfig.Mode = GPIO_MODE_INPUT;
	PinConfig.Pull = GPIO_PULLDOWN;
	PinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &PinConfig);
}

void Init_ADC() {
	RCC->APB2ENR |= (1<<8);
	GPIOA->MODER |= 3 << 2;
	ADC1->SQR3 = 1;
	ADC1->SQR1 = 0;
	ADC1->CR2 |= (1<<0);
}

void Init_spi() {
	// spi1 clock enable.
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	// PA5 AND PA7 -> ALTERNATIVE MODE, PA4 -> ENABLE SELECT.
	GPIOA->MODER |= (1<<8) | (2<<10) | (2<<14);
	// PA5: SCK, PA7: MOSI
	GPIOA->AFR[0] |= (5<<20) | (5<<28);
	// PA4, PA5, PA7 -> VERY FAST OUTPUT. MUST CHANGE IF YOU WANT THE SLAVE MODE.
	GPIOA->OSPEEDR |= (3<<8) | (3<<10) | (2<<14);
	GPIOA->ODR |= (1<<4);
	// now lets configure the spi1 as master.
	// BIDIMODE = 0, BIDIOE = 0, CRCEN = 0, CRCNEXT = 0, DFF = 1, RXONLY = 0, SSM = 1, SSI = 1, LSBFIRST = 0, SPE = 0, BR = 011, MSTR = 1, CPOL = 0, CPHA = 1.
	// 0000101100011101
	SPI1->CR1 = 0x0b1d;
	SPI1->CR1 |= (1<<6);
}

void clear_rest(char* msg, int size) {
	int j = 0;
	while(msg[j] != '\0') {
		j++;
	}
	while(j < size-1) {
		msg[j++] = ' ';
	}
}
enum state{keypad_input, freq_input, duration_input, send_data};
char waves[][4] = {"", "sin", "squ", "tri", "asn", "stp", "saw"};
int main(void)
{
	enum state program_state = keypad_input;
	SystemCoreClockUpdate();
	HAL_Init();
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	
	
	LCD_Init();
	Init_KeyPad();
	Init_ADC();
	Init_spi();
	LCD_Puts(0, 0, "Hello!");
	int i = 0;
	int command = 0;
	int signal = 0;
	int freq = 0;
	float min_freq = 1;
	float max_freq = 1000;
	int duration = 0;
	float min_duration = 500;
	float max_duration = 10000;
	float v_in = 0;
	char msg[6];
	for (i = 0; i <= 1000000 ; i++);
	LCD_Clear();
	LCD_Puts(0, 0, "99243047");
	for (i = 0; i <= 1000000 ; i++);
	LCD_Clear();
	LCD_Puts(0, 0, "Choose signal in");
	for (i = 0; i <= 300; i++) {
		if (i < 99) {
			LCD_Puts(0,1, "3");
		}
		if (i >= 99 && i < 198) {
			LCD_Puts(0,1, "2");
		}
		if (i >= 198) {
			LCD_Puts(0,1, "1");
		}
	}
	LCD_Clear();
	while (1) {
		switch(program_state) {
			case keypad_input:
				for (i = 0; i <= 100000; i++); // for user to let go of the * key.
				LCD_Puts(0, 0, "Choose signal");
				LCD_Puts(0, 1, "Enter key");
				do{
					signal = read_keyboard();
				} while(signal == -1);
				if ((signal < 1) || (signal > 6)) {
					LCD_Clear();
					LCD_Puts(0, 0, "not a valid wave");
					for (i = 0; i <= 2000000; i++);
					break;
				}
				program_state = freq_input;
				LCD_Clear();
				break;
			case freq_input:
				for (i = 0; i <= 100000; i++); // for user to let go of the * key.
				LCD_Puts(0, 0, "Frq(#=ok,*=back)");
				LCD_Puts(0, 1, waves[signal]);
				freq = Read_ADC();
				v_in = (float) freq*5/4095;
				freq = (int) (v_in*(max_freq-min_freq)/5 + min_freq);
				sprintf(msg, "%d", freq);
				clear_rest(msg, 6);
				LCD_Puts(5, 1, msg);
				
				command = read_keyboard();
				if (command == '#') {
					program_state = duration_input;
					LCD_Clear();
				}
				else if (command == '*') {
					program_state = keypad_input;
					LCD_Clear();
				}
				break;
			case duration_input:
				for (i = 0; i <= 100000; i++); // for user to let go of the * key.
				LCD_Puts(0, 0, "T  (#=ok,*=back)");
				LCD_Puts(0, 1, waves[signal]);
				duration = Read_ADC();
				v_in = (float) duration*5/4095;
				duration = (int) (v_in*(max_duration-min_duration)/5 + min_duration);
				sprintf(msg, "%d", duration);
				clear_rest(msg, 6);
				LCD_Puts(5, 1, msg);
			
				command = read_keyboard();
				if (command == '#') {
					program_state = send_data;
					LCD_Clear();
				}
				else if (command == '*') {
					program_state = freq_input;
					LCD_Clear();
				}
				break;
			case send_data:
				LCD_Puts(0, 0, "sending data");
				send_spi((uint16_t) signal, (uint16_t) freq, (uint16_t) duration);			
				break;
		}
	}
	return 0;
}

int read_keyboard() {
	GPIOC->ODR = 1;
	uint32_t data = (GPIOC->IDR & (0xf << 3)) >> 3;
	if (data !=0 ) {
		switch(data) {
			case 1:
				return 1;
			case 2:
				return 4;
			case 4:
				return 7;
			case 8:
				return '*';
		}
	}
	GPIOC->ODR = 2;
	data = (GPIOC->IDR & (0xf << 3)) >> 3;
	if (data !=0 ) {
		switch(data) {
			case 1:
				return 2;
			case 2:
				return 5;
			case 4:
				return 8;
			case 8:
				return 0;
		}
	}
	GPIOC->ODR = 4;
	data = (GPIOC->IDR & (0xf << 3)) >> 3;
	if (data !=0 ) {
		switch(data) {
			case 1:
				return 3;
			case 2:
				return 6;
			case 4:
				return 9;
			case 8:
				return '#'; 
		}
	}
	return -1;
}

uint16_t Read_ADC() {
	ADC1->SR = 0;
	ADC1->CR2 |= (1<<30);
	while((ADC1->SR & (1<<1)) == 0);
	uint16_t result = ADC1->DR & 0x0fff;
	return result;
}

void send_spi(uint16_t param1, uint16_t param2, uint16_t param3) {
	GPIOA->ODR &= ~(1<<4);
	while((SPI1->SR & (1<<1)) == 0);
	SPI1->DR = param1;
	while((SPI1->SR & (1<<1)) == 0);
	SPI1->DR = param2;
	while((SPI1->SR & (1<<1)) == 0);
	SPI1->DR = param3;
	while((SPI1->SR & (1<<1)) == 0);
	while((SPI1->SR & (1<<7)) != 0);
	GPIOA->ODR |= (1<<4);
}
void SysTick_Handler(void)
{
  HAL_IncTick();
}
