#include "button.h"
#include "LPC17xx.h"

#include "../RIT/RIT.h"	

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	enable_RIT();
	NVIC_DisableIRQ(EINT0_IRQn);		
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	enable_RIT();										
	NVIC_DisableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	
	LPC_SC->EXTINT &= (1 << 1);
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	enable_RIT();										
	NVIC_DisableIRQ(EINT2_IRQn);		
	LPC_PINCON->PINSEL4    &= ~(1 << 24);
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */   
}


