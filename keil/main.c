#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"
int read_keyboard();
uint16_t Read_ADC();
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

int main(void)
{
	SystemCoreClockUpdate();
	HAL_Init();
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	
	
	LCD_Init();
	Init_KeyPad();
	Init_ADC();
	LCD_Puts(0, 0, "Hello!");
	int i = 0;
	int command = 0;
	int signal = 0;
	int freq = 0;
	int duration = 0;
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
	LCD_Puts(0,1,"Enter key");
	while (1) {
		/*command = read_keyboard();
		if (command == -1)
			continue;
		LCD_Clear();
		if ((command == '*') || (command == '#')) {
			char str[2] = {(char) command, '\0'};
			LCD_Puts(0, 0, str);
		}
		else {
			char str[2] = {(char) (command+'0'), '\0'};
			LCD_Puts(0, 0, str);
		}*/
		uint16_t value = Read_ADC();
		float v_in = (float) value*5/4095;
		v_in *= 100;
		value = (uint16_t) v_in;
		char str[10];
		sprintf(str, "%d", value);
		LCD_Clear();
		LCD_Puts(0, 0, str);
		
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
void SysTick_Handler(void)
{
  HAL_IncTick();
}
