#ifndef TETRIS_H
#define TETRIS_H

#include "LPC17xx.h"
#include "../GLCD/GLCD.h"
#include <stdio.h>
#include <stdlib.h>

extern volatile int punteggio;
extern volatile int top_score;
extern volatile int gamestatus; //0 se partita da iniziare o dopo game over, 1 se partita in corso, 2 se partita in pausa
extern int current_rotation;
extern int currentPosX;
extern int currentPosY;
extern int currentShape;
extern volatile int timer_flag;
extern volatile int joyL_flag;
extern volatile int joyR_flag;
extern volatile int joyU_flag;
extern volatile int joyD_flag;
extern volatile int num_lines;

extern volatile int linesPowerUp;
extern volatile int slowDown_timer;

extern uint16_t AREA[20][10];

typedef struct {
    const uint8_t (*shape)[4][4]; 
    uint16_t color;             
} TetrisPiece;

extern const TetrisPiece TETROMINOES[7];

void Tetris_Init(void);
void Tetris_Start(void);
void Tetris_GameOver(void);

void DrawShape(int x, int y, uint16_t color);
void DrawBlock();
void DrawMap();
void WriteScore(void);

void Spawn(void);
void RotatePiece(void);
void MoveLeft(void);
void MoveRight(void);
void MoveDown(void);
void MoveDown2X(void);
void DropDown(void);

int CollisionCheck(int px, int py, int rot);
void LockPiece(void);
void CheckFullLine(void);

void SpawnPowerup(void);
void ClearHalfLines(void);
void SlowDown(void);
void Malus(void);

#endif
