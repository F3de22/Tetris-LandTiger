/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "tetris.h"
#include "../adc/adc.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
volatile int start_flag = 0;
volatile int timer_flag = 0;
volatile int joyL_flag = 0;
volatile int joyR_flag = 0;
volatile int joyU_flag = 0;
volatile int joyD_flag = 0;
volatile int slowDown_timer = 0;
extern volatile int gamestatus;
extern volatile unsigned short AD_current;

const int melody_length = 39;

static int note_index = 0;
static int note_counter = 0;
volatile int sfx_timer = 0; 
volatile int gameover_flag = 0;

const int melody[] = { //canzone tetris
	NOTE_E5, 8, NOTE_B4, 4, NOTE_C5, 4, NOTE_D5, 8, NOTE_C5, 4, NOTE_B4, 4,
	NOTE_A4, 8, NOTE_A4, 4, NOTE_C5, 4, NOTE_E5, 8, NOTE_D5, 4, NOTE_C5, 4,
	NOTE_B4, 6, NOTE_C5, 2, NOTE_D5, 8, NOTE_E5, 8,
	NOTE_C5, 8, NOTE_A4, 8, NOTE_A4, 8, PAUSE, 4
};

const int melody_gameover[] = { //musica di gameover
	NOTE_E5, 8, 
	NOTE_D5, 8, 
	NOTE_C5, 8, 
	NOTE_B4, 8, 
	NOTE_A4, 16, 
	PAUSE, 4
};
const int gameover_melody_length = 12;
static int go_note_index = 0;
static int go_note_counter = 0;

void playNote(int freq) {
	if(freq > 0) {
		reset_timer(0);
		init_timer(0, freq); //setta il timer alla frequenza della nota
		enable_timer(0);
	} else {
		disable_timer(0);
	}
}

void playSound(int freq, int durata) {
	sfx_timer = durata;
	playNote(freq);
}

void RIT_IRQHandler (void)
{	
	LPC_RIT->RICTRL |= 0x1;
	static int down1=0;
	static int down2=0;
	static int counter = 0;
	static int J_left = 0;
	static int J_right = 0;
  static int	J_up = 0;
	static int J_down = 0;
	static int adc_counter = 0;
	adc_counter++;
	if(adc_counter >= 10) {
		ADC_start_conversion();
		adc_counter = 0;
	}
	int adc_value = AD_current;
	
	if(gamestatus == 1) { 
		gameover_flag = 0;
		go_note_index = 0;
		if(sfx_timer > 0) {
			sfx_timer--;
		} 
		else {
			if(note_counter <= 0) {
				int freq = melody[note_index * 2];     
				int dur  = melody[note_index * 2 + 1]; 
				
				playNote(freq);
				note_counter = dur;
				
				note_index++;
				if(note_index >= (melody_length / 2)) {
					note_index = 0; 
				}
			}else {
				note_counter--;
			}
		}
	}else if (gameover_flag == 1) {
        // musica gameover
        if(go_note_counter <= 0) {
					if(go_note_index < (gameover_melody_length / 2)) {
						int freq = melody_gameover[go_note_index * 2];     
						int dur  = melody_gameover[go_note_index * 2 + 1]; 
						playNote(freq);
						go_note_counter = dur;
						go_note_index++;
					} else {
						playNote(0); 
						gameover_flag = 0; 
						disable_timer(0);
					}
        }else{
					go_note_counter--;
				}
		}else {
		disable_timer(0); 
	}
	
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ 
		down1++;
		switch(down1){
			case 1:
				//qui imposto solo lo stato o flag perché sennò l'esecuzione dell'interrupt dura di più del RIT e si impalla la cpu
				if(gamestatus == 0) start_flag = 1;
				else if(gamestatus == 1) gamestatus = 2; // pausa
				else if(gamestatus == 2) gamestatus = 1; //riprendi
				break;
			default: break;
		}
	}else { 
			down1=0; }		
	
	if((LPC_PINCON->PINSEL4 & (1 << 24)) == 0){
		down2++;
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){   //int0
			switch(down2){
				case 1:
					if(gamestatus == 1) { 
						joyD_flag = 1; 
					}
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down2=0;			
			LPC_SC->EXTINT |= (1 << 2);
			NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}	
	
	
	if(gamestatus == 1) {
		counter++;
		int base_ticks = 20 - (adc_value * 16 / 4095);
		if (base_ticks < 4) base_ticks = 4;

		//slow down powerup
		if (slowDown_timer > 0) {
			slowDown_timer--;
			if (base_ticks < 20) {
				base_ticks = 20;
			}
		}

		int current_threshold = base_ticks;
		//joystick GIU
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
			current_threshold = base_ticks / 2; //dimezza i tick per raddoppiare la velocità
			if(current_threshold < 1) current_threshold = 1; 
		}
		if(counter >= current_threshold) {
			timer_flag = 1; 
			counter = 0;
		}
		
		//joystick SINISTRA
    if((LPC_GPIO1->FIOPIN & (1<<27)) == 0) { 
			J_left++;
			if(J_left == 1) joyL_flag = 1; 
		} else J_left = 0;
        //joystick DESTRA
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
			J_right++;
			if(J_right == 1) joyR_flag = 1;
		} else J_right = 0;

		//joystick SU (Ruota)
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {
			J_up++;
			if(J_up== 1) joyU_flag = 1;
		} else J_up = 0;
  }
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
