/*
* Eric Sheen
*/

#include "myLib.h"
#include "background.h"
#include "gameover.h"
#include "trashbin.h"
#include "bananaskin.h"
#include "paper.h"
#include "heart.h"
#include "money.h"
#include "victory.h"
#include "text.h"

u16* videoBuffer = (u16*)0x6000000;

void setPixel(int r, int c, u16 color);
void drawRect(int r, int c, int width, int height, u16 color);
void drawHollowRect(int r, int c, int width, int height, u16 color);
void plotLine(int x0, int y0, int x1, int y1, u16 color);
void drawImage3(int r, int c, int width, int height, const u16* image);
void eraseimage(int row, int col, int height, int width);


void setPixel(int r, int c, u16 color) {
	videoBuffer[((r) * 240 + (c))] = color;
}

/*

*/
void drawRect(int r, int c, int width, int height, u16 color) {
	int i, j;
	for(i = 0; i < width; i++)
	{
		for(j = 0; j < height; j++)
		{
			setPixel(r + j, c + i, color);
		}
	}
}


void drawHollowRect(int r, int c, int width, int height, u16 color) {
	int i;
	for(i = 0; i < width; i++) {
		setPixel(r, c + i, color);
		setPixel(r + height - 1, c + i, color);
	}
	for(i = 1; i < height - 1; i++) {
		setPixel(r + i, c, color);
		setPixel(r + i, c + width - 1, color);
	}
}	

void plotLine(int x0, int y0, int x1, int y1, u16 color) {
	int dx, dy;
	dx = x1 - x0;
	dy = y1 - y0;
	int D, D2;
	D = dy + dy - dx;
	D2 = dx + dx - dy;
	setPixel(y0, x0, color);
	int x, y;
	int i;
	y = y0;
	x = x0;
	if (dx >= dy) {
		for(i = x0 + 1; i < x1; i++) {
			if (D > 0) {
				y = y + 1;
				setPixel(y, i, color);
				D = D + (dy + dy - dx - dx);
			} else {
				setPixel(y, i, color);
				D = D + (dy + dy);
			}
		}
	} else {
		for (i = y0; i < y1; i++) {
			if (D2 > 0) { 
				x = x + 1;
				setPixel(i, x, color);
				D2 = D2 + (dx + dx - dy - dy);
			} else {
				setPixel(i, x, color);
				D2 = D2 + (dx + dx);
			}
		}
	}
}

void drawImage3(int r, int c, int width, int height, const u16* image) {
	int row;
	for(row = 0; row < height; row++) {
		DMA[3].src = &image[row * width];
		DMA[3].dst = &videoBuffer[OFFSET(r + row, c, 240)];
		DMA[3].cnt = DMA_ON | width;
	}
}

void eraseimage(int row, int col, int height, int width) {
	int r;
	for(r=0; r<height; r++) {
		DMA[3].src = &background[OFFSET(row + r, col, 240)];
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
		DMA[3].cnt = width | DMA_ON;	
	}
}

int menu() {
    	int seed = 0;
  	REG_DISPCTL = MODE3 | BG2_ENABLE;
    	drawImage3(0, 0, 240, 160, background);;
    	drawHollowRect(50, 35, 170, 60, BLACK);
    	drawString(20, 90, "Trash Bin", BLACK);
    	drawString(55, 40, "Instruction:", BLACK);
    	drawString(70, 45, "Catch    and    for score", BLACK);
    	drawString(85, 45, "Don't catch", RED);
    	drawString(100, 45, "Catch    for Life", BLACK);
    	drawString(130, 60, "Start (Press Enter)", YELLOW);
    	drawImage3(68, 80, 13, 13, paper);
    	drawImage3(68, 122, 13, 13, bananaskin);
    	drawImage3(83, 115, 13, 13, money);
    	drawImage3(98, 80, 13, 13, heart);
    

    	while(!KEY_DOWN_NOW(BUTTON_START)) {
		seed++;
    	}
    	while(KEY_DOWN_NOW(BUTTON_START));
    	return seed;
}

void win() {
    	drawImage3(0, 0, 240, 160, victory);
    	drawString(140, 60, "Press Start (Enter)", BLUE);
    	drawString(150, 60, "to go to main", BLUE);

   	while(!KEY_DOWN_NOW(BUTTON_START)&&!KEY_DOWN_NOW(BUTTON_SELECT));
    	while(KEY_DOWN_NOW(BUTTON_START)||KEY_DOWN_NOW(BUTTON_SELECT));
}

void lose() {
    	drawImage3(0, 0, 240, 160, gameover);
    	drawString(115, 60, "Press Start (Enter)", BLACK);
    	drawString(125, 60, "to try again!", BLACK);
    

    	while(!KEY_DOWN_NOW(BUTTON_START)&&!KEY_DOWN_NOW(BUTTON_SELECT));
	
    	while(KEY_DOWN_NOW(BUTTON_START)||KEY_DOWN_NOW(BUTTON_SELECT));
}

void waitForVblank() {
    	while(SCANLINECOUNTER > 100);
    	while(SCANLINECOUNTER < 100);

}


