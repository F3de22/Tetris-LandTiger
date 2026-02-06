/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons and manage the bouncing effect
 *        	- key1 switches on the led at the left of the current led on, 
 *					- it implements a circular led effect. 	
  * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.h"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "RIT/RIT.h"
#include "timer/timer.h"
#include "GLCD/GLCD.h"
#include "joystick/joystick.h"
#include "TouchPanel/TouchPanel.h"
#include "tetris.h"

extern volatile int start_flag;
extern volatile int timer_flag;
extern volatile int joyL_flag;
extern volatile int joyR_flag;
extern volatile int joyU_flag;
extern volatile int gamestatus;
#ifdef SIMULATOR
extern uint8_t ScaleFlag;
#endif
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	SystemInit();  												
  //LED_init();                           
  BUTTON_init();		
	joystick_init();											
	init_RIT(0x004C4B40);									
	LCD_Initialization();									
	LCD_Clear(Black);
	
	ADC_init();
	ADC_start_conversion();
	
	LPC_SC->PCON |= 0x1;
	LPC_SC->PCON &= ~(0x2);
	
	LPC_PINCON->PINSEL1 |= (2 << 20);
	init_timer(0, 0);
	enable_timer(1);
	
	enable_RIT();
	Tetris_Init();
	
	int old_gamestatus = -1;
	start_flag = 0;
	
  while (1) {
		if (start_flag == 1) {
			disable_RIT();
			srand(LPC_TIM1->TC); //per evitare che venga generata sempre la stessa sequenz di forme			
			Tetris_Start();
			start_flag = 0;   
			enable_RIT(); 
			old_gamestatus = 1;
		}
		
		if (gamestatus == 2) {
			GUI_Text(50, 150, (uint8_t *)"PAUSE", Green, Black);
    }

		if (gamestatus == 1) {	
			if(old_gamestatus == 2) {
				GUI_Text(50, 150, (uint8_t *)"     ", Black, Black); //cancella la scritta pause
				DrawMap();
			}
			if (timer_flag == 1) {
				MoveDown();     
				timer_flag = 0; 
			}
			if (joyL_flag == 1) {
				MoveLeft();
				joyL_flag = 0;
			}
			if (joyR_flag == 1) {
				MoveRight();
				joyR_flag = 0;
			}
			if (joyU_flag == 1) {
				RotatePiece();
				joyU_flag = 0;
			}
			if (joyD_flag == 1) {
				DropDown();
				joyD_flag = 0;
			}
		}
		old_gamestatus = gamestatus;
  }

}
