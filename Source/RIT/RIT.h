/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           RIT.h
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_RIT, funct_RIT, IRQ_RIT .c files
** Correlated files:    lib_RIT.c, funct_RIT.c, IRQ_RIT.c
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __RIT_H
#define __RIT_H

#include <stdint.h>
#include "../tetris.h"
#include "../GLCD/GLCD.h"

#define NOTE_C4  2120
#define NOTE_D4  1890
#define NOTE_E4  1684
#define NOTE_F4  1592
#define NOTE_G4  1417
#define NOTE_A4  1263
#define NOTE_B4  1125
#define NOTE_C5  1062
#define NOTE_D5  946
#define NOTE_E5  843
#define PAUSE    0

/* init_RIT.c */
extern uint32_t init_RIT( uint32_t RITInterval );
extern void enable_RIT( void );
extern void disable_RIT( void );
extern void reset_RIT( void );
/* IRQ_RIT.c */
extern void RIT_IRQHandler (void);

#endif /* end __RIT_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
