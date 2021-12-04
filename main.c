#include "stm32l476xx.h"
#include "SysTick.h"
#include "LED.h"
#include "LCD.h"

void System_Clock_Init(void){
	
	RCC->CR |= RCC_CR_MSION; 
	
	// Select MSI as the clock source of System Clock
	RCC->CFGR &= ~RCC_CFGR_SW; 
	
	// Wait until MSI is ready
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 	
	
	// MSIRANGE can be modified when MSI is OFF (MSION=0) or when MSI is ready (MSIRDY=1). 
	RCC->CR &= ~RCC_CR_MSIRANGE; 
	RCC->CR |= RCC_CR_MSIRANGE_7;  // Select MSI 8 MHz	
 
	// The MSIRGSEL bit in RCC-CR select which MSIRANGE is used. 
	// If MSIRGSEL is 0, the MSIRANGE in RCC_CSR is used to select the MSI clock range.  (This is the default)
	// If MSIRGSEL is 1, the MSIRANGE in RCC_CR is used. 
	RCC->CR |= RCC_CR_MSIRGSEL; 
	
	// Enable MSI and wait until it's ready	
	while ((RCC->CR & RCC_CR_MSIRDY) == 0); 		
}

void delay_fun(int time)
{
	int i;
	for (i = 0; i < time; i++)
	{
		
	}
	
}

bool joystick_pressed()
{
	return ((GPIOA->IDR & 0x1) == 0x1);
}

int move_right(int curr_pos)
{
	LCD_Clear();
	LCD_WriteChar((uint8_t *) "-", 0, 0, curr_pos+1);
	return curr_pos+1;
}

int move_left(int curr_pos)
{
	LCD_Clear();
	LCD_WriteChar((uint8_t *) "-", 0, 0, curr_pos-1);
	return curr_pos-1;
}

int hit_ball_left(int curr_pos)
{
	int i = 0;
	/*if (joystick_pressed() && curr_pos != 0)
	{
		curr_pos = -1;
		while (joystick_pressed())
		{
		}
		return curr_pos;
	}*/
	if ( joystick_pressed() && curr_pos == 0)
	{
		for (i = 0; i < 5; i++)
		{
		curr_pos = move_right(curr_pos);
		delay_fun(300000);
		}
	}
	else if ((!joystick_pressed() && curr_pos == 0))
	{
		curr_pos = move_left(curr_pos);
	}
	
	return curr_pos;
	
}
	
	
int hit_ball_right(int curr_pos)
{
	int i = 0;
	if (joystick_pressed() && curr_pos == 5)
	{
		for (i = 0; i < 5; i++)
		{
		curr_pos = move_left(curr_pos);
		delay_fun(300000);
		}
	}
	else if ((!joystick_pressed() && curr_pos == 5))
	{
		curr_pos = move_right(curr_pos);
	}
	
	return curr_pos;
}	



int player_serve(int player_num)
{
	int pos;
	if (player_num == 1)
	{
		LCD_WriteChar((uint8_t *) "-", 0, 0, 0);
		pos = 0;
		
	}
	
	else
	{
		LCD_WriteChar((uint8_t *) "-", 0, 0, 5);
		pos = 5;
	}
	while (!joystick_pressed())
	{
		
	}
		pos = hit_ball_left(pos);
		pos = hit_ball_right(pos);
		return pos;
	
	
	
}



int main(void){
	
	volatile int ball_pos = -1;
	int i = 0;
	bool start = 0;
	// system clock initialize
	System_Clock_Init();
	
	// LCD initialize
	LCD_Initialization();
	
	//joystick initialize
	//clock
	RCC->AHB2ENR |=  RCC_AHB2ENR_GPIOAEN;
	//set to input mode
	GPIOA->MODER &= ~(3U);
	
	ball_pos = move_right(ball_pos);
	LCD_WriteChar((uint8_t *) "-", 0, 0, 0);
	while (!start)
	{
		if (joystick_pressed())
	{
		start = 1;
	}
		
	}
	while(1)
	{

		ball_pos = hit_ball_left(ball_pos);
		ball_pos = hit_ball_right(ball_pos);
		
		if(ball_pos == -1)
		{
			LCD_WriteChar((uint8_t *)"2", 0, 0, 0);
			LCD_Clear();
			ball_pos = player_serve(2);
		}
		if(ball_pos == 6)
		{
			LCD_WriteChar((uint8_t *)"1", 0, 0, 0);
			LCD_Clear();
			ball_pos = player_serve(1);
		}
		
	
	}

}

