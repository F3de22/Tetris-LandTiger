#include "tetris.h"
#include "RIT.h"
volatile int punteggio = 0;
volatile int top_score = 0;
volatile int gamestatus = 0; //0 se partita da iniziare o dopo game over, 1 se partita in corso, 2 se partita in pausa
volatile int num_lines = 0;
volatile int linesPowerUp = 0;
extern volatile int slowDown_timer;
extern volatile int gameover_flag;
int current_rotation;
int currentPosX;
int currentPosY;
int currentShape; //0 se I, 1 se O, 2 se T, 3 se J, 4 se L, 5 se S, 6 se Z

uint16_t AREA[20][10];

//forme con le  4 rotazioni possibili
const uint8_t Forma_I[4][4][4] = {
    { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} }, 
    { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} }, 
    { {0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0} }, 
    { {0,0,1,0}, {0,0,1,0}, {0,0,1,0}, {0,0,1,0} }
};

const uint8_t Forma_O[4][4][4] = {
    { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} },
    { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} },
    { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} },
    { {0,0,0,0}, {0,1,1,0}, {0,1,1,0}, {0,0,0,0} } 
};

const uint8_t Forma_T[4][4][4] = {
    { {0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, 
    { {0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} }, 
    { {0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0} }, 
    { {0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }  
};

const uint8_t Forma_J[4][4][4] = {
    { {1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, 
    { {0,1,1,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }, 
    { {0,0,0,0}, {1,1,1,0}, {0,0,1,0}, {0,0,0,0} }, 
    { {0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0} }
};

const uint8_t Forma_L[4][4][4] = {
    { {0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0} }, 
    { {0,1,0,0}, {0,1,0,0}, {0,1,1,0}, {0,0,0,0} }, 
    { {0,0,0,0}, {1,1,1,0}, {1,0,0,0}, {0,0,0,0} }, 
    { {1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0} }  
};

const uint8_t Forma_S[4][4][4] = {
    { {0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0} }, 
    { {0,1,0,0}, {0,1,1,0}, {0,0,1,0}, {0,0,0,0} }, 
    { {0,0,0,0}, {0,1,1,0}, {1,1,0,0}, {0,0,0,0} }, 
    { {1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0} }  
};

const uint8_t Forma_Z[4][4][4] = {
    { {1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0} }, 
    { {0,0,1,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0} }, 
    { {0,0,0,0}, {1,1,0,0}, {0,1,1,0}, {0,0,0,0} }, 
    { {0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0} } 
};

const TetrisPiece TETROMINOES[7] = {
    { Forma_I, 0x07FF},
    { Forma_O, 0xFFE0},
    { Forma_T, 0xF81F},
    { Forma_J, 0x001F},
    { Forma_L, 0xFC00},
    { Forma_S, 0x07E0},
    { Forma_Z, 0xF800}
};


//!!!!!! FUNZIONI !!!!!

void Tetris_Init(void){
	gamestatus = 0;
	LCD_Clear(Black);
	GUI_Text(160, 40, (uint8_t *)"TETRIS", Blue, White);
	GUI_Text(60, 140, (uint8_t *)"Press Key1", Green, Black);
}

void Tetris_Start(void){
	LCD_Clear(Black);
	LCD_DrawLine(150, 0, 150, 319, 0xFFFF); //dx
	LCD_DrawLine(9, 19, 150, 19, White); // su
  LCD_DrawLine(9, 300, 150, 300,   White); // giu
  LCD_DrawLine(9, 19, 9, 300, White); //sx
	
	GUI_Text(160, 40, (uint8_t *)"TETRIS", Blue, White);
  GUI_Text(160, 80, (uint8_t *)"Score:", 0xFFFF, 0x0000);
  GUI_Text(160, 140, (uint8_t *)"Top-Score:", 0xFFFF, 0x0000);
	GUI_Text(160, 200, (uint8_t *)"Lines:", 0xFFFF, 0x0000);
	
	int i,j ;
	for(i = 0; i<20; i++){
		for(j = 0; j<10; j++)
			AREA[i][j] = 0;
	}
	
	joyD_flag = 0; 
	joyL_flag = 0; 
	joyR_flag = 0; 
	joyU_flag = 0;
	timer_flag = 0;
	
	punteggio = 0;
	num_lines = 0;
	linesPowerUp = 0;
  slowDown_timer = 0;
	WriteScore();
	Spawn();
	gamestatus = 1;
}

void DrawBlok(int x, int y, uint16_t color){
	int i, j;
	uint16_t screenX = 10 + (x * 14);
	uint16_t screenY = 20 + (y * 14);

	for (i = 0; i < 14; i++) {
		for (j = 0; j < 14; j++) {
			if (i == 0 || j == 0 || i == 13 || j == 13) { //disegna il bordo nero
					LCD_SetPoint(screenX + j, screenY + i, Black);
			} else {
					LCD_SetPoint(screenX + j, screenY + i, color);
			}
		}
	}
}

void DrawShape(int x, int y, uint16_t color){
	int i,j,r,c;
	for(r = 0; r < 4; r++) {
		for(c = 0; c < 4; c++) {
			if(TETROMINOES[currentShape].shape[current_rotation][r][c] != 0)
				DrawBlok(x+c, y+r, color);
		}
	}
}

void DrawMap(void) {
	int i, j;
	for(i=0; i<20; i++) {
		for(j=0; j<10; j++) {
			if(AREA[i][j] != 0){
				DrawBlok(j, i, AREA[i][j]);
			}
			else{
				DrawBlok(j, i, Black);
			}
		}
	}
}

void WriteScore(void){
	char string[20];
	
	sprintf(string, "%d", punteggio);
	GUI_Text(160, 100, (uint8_t *)string, Green, Black);
	
	sprintf(string, "%d", top_score);
	GUI_Text(160, 160, (uint8_t *)string, Red, Black);
	
	sprintf(string, "%d", num_lines);
  GUI_Text(160, 220, (uint8_t *)string, Yellow, Black);
}

void Tetris_GameOver(void){
	gamestatus = 0;
	gameover_flag = 1;
	if(punteggio>top_score)
		top_score = punteggio;
	
	WriteScore();
	
	GUI_Text(20, 150, (uint8_t *)"GAME OVER", Red, Black);
	playSound(NOTE_G4, 20);
}

int CollisionCheck(int px, int py, int rot){
	int collided = 0;
	int r,c;
	int pieceX, pieceY;
	for(r=0;r<4;r++){
		for(c=0;c<4;c++){
			if(TETROMINOES[currentShape].shape[rot][r][c] != 0){
				pieceX = px + c;
        pieceY = py + r;
				if(pieceX<0||pieceX>=10||pieceY>=20) collided = 1;//se esco fuori dall'area di gioco
				else if(AREA[pieceY][pieceX] != 0) collided = 1; // collisione con un altro pezzo
			}
		}
	}
	return collided;
}

void LockPiece(void){
	int r,c;
	for(c=0;c<4;c++){
		for(r=0;r<4;r++){
			if(TETROMINOES[currentShape].shape[current_rotation][r][c] != 0)
				AREA[currentPosY + r][currentPosX+c] = TETROMINOES[currentShape].color;
		}
	}
	punteggio = punteggio + 10;
	WriteScore();
}
	
void CheckFullLine(void){
	int i,j;
	int linee_rimosse = 0;
	
	int num_clear_half = 0;
  int num_slow_down = 0;
	
	for(i=19; i>=0; i--){
		int full = 1;
		for(j=0;j<10;j++){
			if(AREA[i][j]==0){
				full = 0;
				break;
			}
		}
		if(full == 1){
			for(j=0; j<10; j++) {
				if(AREA[i][j] == 0xFFFF) num_clear_half = 1;
				if(AREA[i][j] == 0x001F) num_slow_down = 1;
			}
			linee_rimosse = linee_rimosse + 1;
			int k;
			for(k = i; k>0; k--){
				for(j=0;j<10;j++){
					AREA[k][j] = AREA[k-1][j]; //sostituisco la riga piena con la riga sopra e così via
				}
			}
			for(j=0;j<10;j++)
				AREA[0][j] = 0;
			
			i++;//in caso più linee siano piene devo ricontrollarla
		}
	}
	
	if(linee_rimosse >= 1){
		int old_lines = num_lines;
		num_lines = num_lines + linee_rimosse;
		if(linee_rimosse == 4){
			playSound(NOTE_E5, 10);
			punteggio = punteggio + 600;}
		else{
			punteggio = punteggio + (linee_rimosse * 100);
			playSound(NOTE_A4, 5);
			}
		//per la gestione dello spawn dei powerup/malus
		if ((old_lines / 5) < (num_lines / 5)) {
			SpawnPowerup();
			}
		if ((old_lines / 10) < (num_lines / 10)) {
			Malus();
		}
		DrawMap();
		WriteScore();
		
		if(num_clear_half) {
			ClearHalfLines();
		}
		if(num_slow_down) {
			SlowDown();
		}
	}
}

void Spawn(void){
	int random = rand()% 7; //genera casualmente una forma
  current_rotation = 0;
	currentPosX = 3;
	currentPosY = 0;
	currentShape = random;
	
	if(CollisionCheck(currentPosX, currentPosY, current_rotation))
		Tetris_GameOver();
	else
		DrawShape(currentPosX, currentPosY, TETROMINOES[currentShape].color);
}

void RotatePiece(void){
	int new_rotation = current_rotation + 1;
	if (new_rotation == 4)
		new_rotation = 0;
	if (CollisionCheck(currentPosX, currentPosY, new_rotation) == 0){
		DrawShape(currentPosX, currentPosY, Black);
		current_rotation = new_rotation;
		DrawShape(currentPosX, currentPosY, TETROMINOES[currentShape].color);
		playSound(NOTE_C5, 1);
	}
}

void MoveLeft(void){
	if(CollisionCheck(currentPosX-1, currentPosY, current_rotation) == 0){
		DrawShape(currentPosX, currentPosY, Black);
		currentPosX = currentPosX -1;
		DrawShape(currentPosX, currentPosY, TETROMINOES[currentShape].color);
		playSound(3000, 1);
	}
}

void MoveRight(void){
	if(CollisionCheck(currentPosX+1, currentPosY, current_rotation) == 0){
		DrawShape(currentPosX, currentPosY, Black);
		currentPosX = currentPosX +1;
		DrawShape(currentPosX, currentPosY, TETROMINOES[currentShape].color);
		playSound(3000, 1);
	}
}

void MoveDown(void){
	if(CollisionCheck(currentPosX, currentPosY+1, current_rotation) == 0){
		DrawShape(currentPosX, currentPosY, Black);
		currentPosY = currentPosY +1;
		DrawShape(currentPosX, currentPosY, TETROMINOES[currentShape].color);
	}
	else{
		LockPiece();
		CheckFullLine();
		Spawn();
	}
}

void DropDown(void){
	DrawShape(currentPosX, currentPosY, Black);
	while(CollisionCheck(currentPosX, currentPosY+1, current_rotation)==0)
		currentPosY = currentPosY +1;
	
	DrawShape(currentPosX, currentPosY, TETROMINOES[currentShape].color);
	
	playSound(1000, 1);
	
	LockPiece();
	CheckFullLine();
	Spawn();
}

void SpawnPowerup(void) {
	int count = 0;
	int i, j;
	
	for(i=0; i<20; i++) {
		for(j=0; j<10; j++) {
			if(AREA[i][j] != 0 && AREA[i][j] != 0xFFFF && AREA[i][j] != 0x7BEF) {
				count++;
			}
		}
	}
	if(count == 0) return;
	
	int idx = rand() % count;
  int current_idx = 0;
	
	for(i=0; i<20; i++) {
		for(j=0; j<10; j++) {
			if(AREA[i][j] != 0 && AREA[i][j] != 0xFFFF && AREA[i][j] != 0x7BEF) {
				if(current_idx == idx) {
					if(rand() % 2 == 0) {
						AREA[i][j] = 0xFFFF;
					} else {
						AREA[i][j] = 0x7BEF;
					}
					DrawMap();
					return;
				}
				current_idx++;
			}
		}
	}
}

void ClearHalfLines(void) {
	int firstLine = 20;
	int i, j;
	for(i=10; i<20; i++) {
		int found = 0;
		for(j=0; j<10; j++){ 
			if(AREA[i][j] != 0 && AREA[i][j] != Black) {
				found = 1;
				break;
			}
		}
		if(found) {
			firstLine = i;
			break;
		}
	}
	
	if(firstLine == 20) return;
	
	int height = 20 - firstLine;
	int lines_to_remove = height / 2; //se sono dispari si tronca
	if(lines_to_remove == 0) return; //in caso sia solo una riga


	int lines_groups = lines_to_remove / 4;
	int remaining_lines = lines_to_remove % 4;

	punteggio += (lines_groups * 600) + (remaining_lines * 100);
	
	for(i = 19; i >= firstLine + lines_to_remove; i--) {
		for(j=0; j<10; j++) {
			AREA[i][j] = AREA[i - lines_to_remove][j];
		}
	}
	for(i = firstLine; i < firstLine + lines_to_remove; i++) {
		for(j=0; j<10; j++) {
			AREA[i][j] = 0;
		}
	}

	DrawMap();
	WriteScore();
}

void SlowDown(void) {
    slowDown_timer = 300; //300*50ms = 15secondi di powerup
}

void Malus(void) {
    int i, j;
    for(j=0; j<10; j++) {
			if(AREA[0][j] != 0) {
				Tetris_GameOver();
				return;
			}
    }
    for(i=0; i<19; i++) {
			for(j=0; j<10; j++) {
				AREA[i][j] = AREA[i+1][j];
			}
    }
    for(j=0; j<10; j++) AREA[19][j] = 0;
    
    int blocks_placed = 0;
    while(blocks_placed < 7) {
			int pos = rand() % 10;
			if(AREA[19][pos] == 0) {
				AREA[19][pos] = Red; //colore della linea aggiunta
				blocks_placed++;
			}
    }
    DrawMap();
}



