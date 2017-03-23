/*
* Eric Sheen
*/

#include <stdlib.h>
#include <stdio.h>
#include "gameover.h"
#include "background.h"
#include "myLib.h"
#include "trashbin.h"
#include "text.h"
#include "bananaskin.h"
#include "paper.h"
#include "heart.h"
#include "money.h"
#include "victory.h"

#define NUMOBJ 3

extern u16* videoBuffer;
extern const unsigned char fontdata_6x8[12288];

void drawImage3(int r, int c, int width, int height, const u16* image);

enum {MENU, GAME, WIN, LOSE};
player p1;

int main(void)
{
    	p1.x = 180;
    	p1.y = 120;
    	int seed = 0;
    	int state = MENU;
    
    	while(1) {
        	switch(state) {
        	case MENU:
		    seed = menu();
		    state = GAME;
		case GAME:
		    state = game(seed);
		    break;
		case LOSE:
		    lose();
		    state = MENU;
		    break;
		default:
		    break;
        	}
    	}
}


int game(int seed) {
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	MOVOBJ objs[NUMOBJ];
	int life = 3;
    	int level = 1;
    	int score = 0;
    	char buffer1[41];
    	char buffer2[41];
	char buffer3[41];
	srand(seed);
	
	eraseimage(0, 0, 160, 240);
	
	int i;
	for (i = 0; i < NUMOBJ; i++) {
		objs[i].r = rand() % 150;
		objs[i].c = 3;
		objs[i].oldR = objs[i].r;
		objs[i].oldC = objs[i].c;
		objs[i].rd = ((rand() % 1) + 1) + (level - 1); 
		objs[i].id = (int) (rand() % 10);
	}


	while(1) {
		int oldX = p1.x;
		int oldY = p1.y;
	
		if (KEY_DOWN_NOW(BUTTON_LEFT)) {
		    if(p1.x <= 10) {
		        p1.x = 10;
		    } else {
		        p1.x -= 5;
		    }
		   
		}
		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
			if(p1.x >= 200) {
				p1.x = 200;
			} else {
				p1.x += 5;
			}		
		}
		if(KEY_DOWN_NOW(BUTTON_UP)) {
			if (oldY <= 10) {
			   p1.y = 10;
			}
			else {
			p1.y -= 5;
			}	
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN)) {
			if (oldY >= 140) {
			   p1.y = 140;
			}
			else {
			p1.y += 5;
			}
		}

		while(KEY_DOWN_NOW(BUTTON_SELECT)) {
			menu();
			game(1);
		}


		int i;
		for (i = 0; i < NUMOBJ; i++) {
			objs[i].oldC = objs[i].c;
			objs[i].c = objs[i].c + objs[i].rd;
			if (objs[i].r > p1.y + 15) {
			} else if (objs[i].c > p1.x + 20) {
			} else if (objs[i].c < p1.x - 20) {
			} else if (objs[i].r < p1.y - 15) {
			} else {
				if (objs[i].id == 9) {
					life++;
				} else if (objs[i].id >= 7 && objs[i].id < 9) {
					life--;
					if (life == 0)  {
						lose();
						game(1);
					}
				} else {
					score += 10;
					if (score == (50 * level)) {
						level += 1;
						if (level == 4) {
							win();
							menu();
							game(1);
						}
					}
				}

				eraseimage(objs[i].oldR, objs[i].oldC, 13, 13);
				objs[i].r = rand() % 150;
				objs[i].c = 3;
				objs[i].oldR = objs[i].r;
				objs[i].oldC = objs[i].c;
				objs[i].rd = ((rand() % 1) + 1) + (level - 1); 
				objs[i].id = (int) (rand() % 10);

			}
		
	
			if (objs[i].c > 220) {
				if (objs[i].id < 7) {
					life--;
					if (life == 0)  {
						lose();
						game(1);
					}
				}
				eraseimage(objs[i].oldR, objs[i].oldC, 13, 13);
				objs[i].r = rand() % 150;
				objs[i].c = 3;
				objs[i].oldR = objs[i].r;
				objs[i].oldC = objs[i].c;
				objs[i].rd = ((rand() % 1) + 1) + (level - 1); 
				objs[i].id = (int) (rand() % 10);
			}

		}
		

		sprintf(buffer1, "Score: %u", score);
		sprintf(buffer2, "Lives: %d", life);
		sprintf(buffer3, "Level: %u", level);
		waitForVblank();
		
		eraseimage(0, 0, 10, 240);
		drawString(0, 0, buffer1, BLACK);
		drawString(0, 80, buffer2, BLACK);
		drawString(0, 160, buffer3, BLACK);
		eraseimage(oldY, oldX, 20, 30);

		drawImage3(p1.y, p1.x, 30, 20, trashbin);
		
		for (i = 0; i < NUMOBJ; i++) {
			eraseimage(objs[i].oldR, objs[i].oldC, 13, 13);
			if (objs[i].id >= 0 && objs[i].id < 3) {
				drawImage3(objs[i].r, objs[i].c, 13, 13, paper);
			} else if (objs[i].id >= 3 && objs[i].id < 7) {
				drawImage3(objs[i].r, objs[i].c, 13, 13, bananaskin);
			} else if (objs[i].id >= 7 && objs[i].id < 9) {
				drawImage3(objs[i].r, objs[i].c, 13, 13, money);
			} else {
				drawImage3(objs[i].r, objs[i].c, 13, 13, heart);
			}
		}

	}

}
