#include "stm32l476xx.h"
#include "SysTick.h"
#include "LED.h"
#include "LCD.h"
#include <stdio.h>


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


//delay function
//input: delay period length
void delay_fun(int time)
{
	int i;
	for (i = 0; i < time; i++)
	{
		
	}
	
}

//joystick1_pressed()
//if the joystick for player 1 has been pressed, returns true, otherwise false
bool joystick1_pressed()
{
	return ((GPIOA->IDR & 0x1) == 0x1);
}

//joystick2_pressed()
//if the joystick for player 2 has been pressed, returns true, otherwise false
bool joystick2_pressed()
{
	return ((GPIOA->IDR & (8U)) == 8U);
}

//move_right
// moves the ball one position to the right
// parameters: the current ball position
//	returns: the new ball position
int move_right(int curr_pos)
{
	LCD_Clear();
	LCD_WriteChar((uint8_t *) "-", 0, 0, curr_pos+1);
	return curr_pos+1;
}

//move_left
// moves the ball one position to the left
// parameters: the current ball position
//	returns: the new ball position
int move_left(int curr_pos)
{
	LCD_Clear();
	LCD_WriteChar((uint8_t *) "-", 0, 0, curr_pos-1);
	return curr_pos-1;
}

//hit_ball_left
// hit the ball when it is on the left side of the screen
// parameters: current position of the ball, delay of the game
// returns: new position of the ball
int hit_ball_left(int curr_pos, volatile int *delay)
{
	int i = 0;
	// if ball is on left side of screen and player 1 has pressed the joystick
	if ( joystick1_pressed() && curr_pos == 0)
	{
		// move ball towards the right side 
		for (i = 0; i < 5; i++)
		{
		curr_pos = move_right(curr_pos);
		delay_fun(*delay);
		// with each hit the delay gets smaller (game gets faster)
		*delay -= 1000;
		}
	}
	// if ball is on left side of screen and player 1 has not pressed the joystick
	else if ((!joystick1_pressed() && curr_pos == 0))
	{
		// ball goes off screen
		curr_pos = move_left(curr_pos);
	}
	
	return curr_pos;
	
}
	
//hit_ball_right
// hit the ball when it is on the right side of the screen
// parameters: current position of the ball, delay of the game
// returns: new position of the ball
int hit_ball_right(int curr_pos, volatile int *delay)
{
	int i = 0;
	// if ball is on right side of screen and player 2 has pressed the joystick
	if (joystick2_pressed() && curr_pos == 5)
	{
		// move ball towards left side
		for (i = 0; i < 5; i++)
		{
		curr_pos = move_left(curr_pos);
		delay_fun(*delay);
		// delay gets smaller wit every hit (game gets faster)
		*delay -= 1000;
		}
	}
	// if ball is on right side of screen and player 2 has not pressed the joystick
	else if ((!joystick2_pressed() && curr_pos == 5))
	{
		// ball goes off screen
		curr_pos = move_right(curr_pos);
	}
	return curr_pos;
}	


//player_serve
//holds the ball on one side of the screen until button is pressed
//mimics a player serve
// parameters: the player number to serve, and the delay of the game
// returns: the new position of the ball
int player_serve(int player_num, volatile int *delay)
{
	int pos;
	// if player 1
	if (player_num == 1)
	{
		//hold ball on left side of screen
		LCD_WriteChar((uint8_t *) "-", 0, 0, 0);
		pos = 0;
		//wait for button to be pressed
		while (!joystick1_pressed())
	{
		
	}
		// hit ball after button is pressed (serve)
		pos = hit_ball_left(pos, delay);
		
	}
	// if player 2
	else
	{
		//hold ball on right side of screen
		LCD_WriteChar((uint8_t *) "-", 0, 0, 5);
		pos = 5;
		// wait for button to be pressed
			while (!joystick2_pressed())
	{
		
	}
		// hit ball after button is pressed (serve)
		pos = hit_ball_right(pos, delay);
	}
	
		return pos;
	
}


//show_score
//converts player scores from int to uint8_t and displays them on screen
//parameters: player 1 score, player 2 score
void show_score(int score1, int score2)
{
	// convert from int to unint8_t
	uint8_t score_p2_string = score2+48;
	uint8_t score_p1_string = score1+48;
	// show score on screen
	LCD_WriteChar((uint8_t *) "", 0, 0, 1);
	LCD_WriteChar((uint8_t *) "", 0, 0, 3);
	LCD_WriteChar((uint8_t *) "", 0, 0, 4);
	LCD_WriteChar(&score_p2_string, 0, 0, 5);
	LCD_WriteChar((uint8_t *) "", 0, 1, 2);
	LCD_WriteChar(&score_p1_string, 0, 0, 0);
	delay_fun(5000000);
	LCD_Clear();
	
	
}

//LED_show
//flashes the green and red LEDs once each
void LED_show()
{
	//initialize LEDs
	LED_Init();
	// start values for LEDs
	Green_LED_On();
	Red_LED_Off();
	// toggle both LEDs
	delay_fun(250000);
	Green_LED_Toggle();
	delay_fun(50000);
	Red_LED_Toggle();
	delay_fun(50000);
	
	
}



//	movingString
// Scrolls a string across the LCD
// parameters: a pointer to the string to display, and a value of the length of the string
void movingString(uint8_t* str, uint8_t len) {
	
	uint8_t *cur = str; //pointer to the current start of the string
	uint8_t dot = 0;
	uint8_t colon = 0;
	
	int i;
	for(i = 0; i < len ;i++) { //loop over entire string
		int j;
		for(j = 0; j < 6; j++) { //loop through and print current 6 letters of the string
			//check to see if next char is a dot or colon
			if (*cur + i == ':') {
				colon = 1;
			}
			else {
				colon = 0;
			}
			if (*cur + i == '.') {
				dot = 1;
			}
			else {
				dot = 0;
			}
			LCD_WriteChar(cur+j,dot,colon,j); //print character currently being pointed to
		}
		cur = str + i; //set current character to the next character in the string
		LED_show();
		delay_fun(500000); //delay between LCD changes
	}
}



int main(void){
	
	volatile int ball_pos = 0;
	bool start = 0;
	volatile int time_delay = 300000;
	volatile int score_p1 = 0;
	volatile int score_p2 = 0;
	uint8_t *win_msg;
	
	// system clock initialize
	System_Clock_Init();
	
	// LCD initialize
	LCD_Initialization();
	
	//joystick initialize
	//clock
	RCC->AHB2ENR |=  RCC_AHB2ENR_GPIOAEN;
	//set to input mode
	GPIOA->MODER &= ~(3U);
	GPIOA->MODER &= ~(3U<<6);
	//set to pull down
	GPIOA->PUPDR &= ~(3U);
	GPIOA->PUPDR &= ~(3U<<6);
	GPIOA->PUPDR |= (2U);
	GPIOA->PUPDR |= (2U<<6);
	
	// initial state of game, ball on left side of screen
	LCD_WriteChar((uint8_t *) "-", 0, 0, 0);
	
	// game loop
	while(1)
	{
		// wait until player 1 presses his button
		while (!start)
	{
		if (joystick1_pressed())
	{
		start = 1;
	}
		
	}
		//try to hit the ball at either side
		ball_pos = hit_ball_left(ball_pos, &time_delay);
		ball_pos = hit_ball_right(ball_pos, &time_delay);
		
		// if ball is off screen on the left side, player 2 gets a point
		if(ball_pos == -1)
		{
			score_p2++;
			show_score(score_p1, score_p2);
			// if player 2 has a score of 9 points, they win
			if (score_p2 == 9)
			{
				// display win message player 2
				win_msg = (uint8_t *) "PLAYER 2 WINS. PRESS RESET TO PLAY AGAIN";
				while(1)
				{
				movingString(win_msg, 40);
				LCD_Clear();
				}
				
			}
			
			// if player 2 has not won the game, they get to serve next round
			// time delay is reset for next round, game back to original speed
			time_delay = 300000;
			ball_pos = player_serve(2, &time_delay);
		}
		// if ball is off screen on the right side, player 1 gets a point
		if(ball_pos == 6)
		{
			score_p1++;
			// show score on screen
			show_score(score_p1, score_p2);
			// if player 1 reaches 9 points, the win
			if (score_p1 == 9)
			{
				// display win message player 1
				win_msg = (uint8_t *) "PLAYER 1 WINS. PRESS RESET TO PLAY AGAIN";
				while(1)
				{
				movingString(win_msg, 40);
				}
			}
			//reset game speed for next round
			time_delay = 300000;
			// player 1 serves next round if game is not over.
			ball_pos = player_serve(1, &time_delay);
		}
		
			
	}

}

