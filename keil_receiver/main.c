#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"

void Init_spi() {
	// enabling clock for spi 1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	// PA4 TO PA7 -> ALTERNATIVE MODE.
	GPIOA->MODER |= (2<<8) | (2<<10) | (2<<14);
	// PA4: NSS, PA5: SCK, PA6:MISO, PA7: MOSI
	GPIOA->AFR[0] |= (5<<16) | (5<<20) | (5<<28);
	// BIDIMODE = 0, BIDIOE = 0, CRCEN = 0, CRCNEXT = 0, DFF = 1, RXONLY = 1, SSM = 0, SSI = 0, LSBFIRST = 0, SPE = 0, BR = 000, MSTR = 0, CPOL = 0, CPHA = 1.
	// 0000110000000001
	SPI1->CR1 = 0x0c01;
	//enabling:
	SPI1->CR1 |= (1<<6);
}

void rec_spi(uint32_t* data);

void print_data(uint32_t* data) {
	char msg[10];
	LCD_Puts(0, 0, "data1:");
	sprintf(msg, "%d", data[0]);
	LCD_Puts(8, 0, msg);
	LCD_Puts(0, 1, "2,3:");
	sprintf(msg, "%d", data[1]);
	LCD_Puts(5, 1, msg);
	sprintf(msg, "%d", data[2]);
	LCD_Puts(10, 1, msg);
}
int main(void)
{
	SystemCoreClockUpdate();
	HAL_Init();
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	
	
	LCD_Init();
	Init_spi();
	uint32_t data[3] = {0, 0, 0};
	//char msg[10];
	//LCD_Puts(0, 0, "waiting for data");
	rec_spi(data);
	print_data(data);
	while (1) {
		
		/*
		data2 = rec_spi();
		data3 = rec_spi();
		LCD_Puts(0, 0, "NEW DATA");
		sprintf(msg, "%d", data);
		LCD_Puts(9, 0, msg);
		sprintf(msg, "%d", data2);
		LCD_Puts(0, 1, msg);
		sprintf(msg, "%d", data3);
		LCD_Puts(8, 1, msg);*/
		
	}
	return 0;
}

void rec_spi(uint32_t* data) {
	while((SPI1->SR & (1<<0)) == 0);
	data[0] = SPI1->DR;
	while((SPI1->SR & (1<<0)) == 0);
	data[1] = SPI1->DR;
	while((SPI1->SR & (1<<0)) == 0);
	data[2] = SPI1->DR;
	
}
void SysTick_Handler(void)
{
  HAL_IncTick();
}