

#include "myLib.h"
#include "text.h"

void drawChar(int r, int c, char ch, u16 color) {
    int i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 6; j++) {
            if (fontdata_6x8[OFFSET(i, j, 6) + ch*48]) {
                setPixel(r + i, c + j, color);
            }
	}
    }
}

void drawString(int r, int c, char *str, u16 color) {
    while (*str) {
        drawChar(r, c, *str++, color);
        c = c + 6;
    }
}
