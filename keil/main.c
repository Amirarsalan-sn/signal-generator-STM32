#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"

int main(void)
{
	SystemCoreClockUpdate();
	HAL_Init();
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitTypeDef PinConfig;
	PinConfig.Pin = GPIO_PIN_5;
	PinConfig.Mode = GPIO_MODE_OUTPUT_PP;
	PinConfig.Pull = GPIO_NOPULL;
	PinConfig.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &PinConfig);
	
	LCD_Init();
	LCD_Puts(0, 0, "Hello!");
	int i = 0;
	for (i = 0; i <= 1000000 ; i++);
	LCD_Clear();
	LCD_Puts(0, 0, "99243047");
	LCD_Puts(0, 1, "choose signal");
	for (i = 0; i <= 3000000 ; i++);
	LCD_Clear();
	LCD_Puts(0, 0, "1)");
	while (1);
	return 0;
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
