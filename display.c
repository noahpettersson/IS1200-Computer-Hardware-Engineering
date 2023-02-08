#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <stdlib.h>
#include "display.h"
#include "main.h"
#include "displaysetup.h"
#include "assets.h"
#include "mipslab.h"
#include "objects.h"
#include "map.h"
#include "playerani.h"
#include "mainfunc.h"
char textbuffer[4][16];


void mapcopy(char map[256][6]) {
    int temp1 = 0;
    int temp2 = 0;
    for(temp1 = 0; temp1 < 256; temp1++) {
        for(temp2 = 0; temp2 < 6; temp2++) {
            mapholder[temp1][temp2] = map[temp1][temp2];
        }
    }
}


void user_isr() {
    IFS(0) = (IFS(0)&0xfffffeff);
    if (timeoutcount++ % 5 == 0){
        time2string (textstring, mytime);
        tick(&mytime);
    }
}
/*****************************************************
      Initializes the controller of the SPI and
      sets the output of the control pins, in order
      for the OLED display to be prepared for use.
*****************************************************/
void spiinit(void) {

    OSCCON &= ~0x180000;
    OSCCON |= 0x080000;


    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;

    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);

    SPI2CON = 0;
    SPI2CON |= 0x60;
    SPI2BRG = 4;
    SPI2STATCLR = 0x40;
    SPI2CONSET = 0x8000;
}

void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

void tick( unsigned int * timep )
{
  /* Get current value, store locally */
  register unsigned int t = * timep;
  t += 1; /* Increment local copy */

  /* If result was not a valid BCD-coded time, adjust now */

  if( (t & 0x0000000f) >= 0x0000000a ) t += 0x00000006;
  if( (t & 0x000000f0) >= 0x00000060 ) t += 0x000000a0;
  /* Seconds are now OK */

  if( (t & 0x00000f00) >= 0x00000a00 ) t += 0x00000600;
  if( (t & 0x0000f000) >= 0x00006000 ) t += 0x0000a000;
  /* Minutes are now OK */

  if( (t & 0x000f0000) >= 0x000a0000 ) t += 0x00060000;
  if( (t & 0x00ff0000) >= 0x00240000 ) t += 0x00dc0000;
  /* Hours are now OK */

  if( (t & 0x0f000000) >= 0x0a000000 ) t += 0x06000000;
  if( (t & 0xf0000000) >= 0xa0000000 ) t = 0;
  /* Days are now OK */

  * timep = t; /* Store new value */
}


void display_string(int line, char *s) {
    int i;
    if(line < 0 || line >= 4)
        return;
    if(!s)
        return;

    for(i = 0; i < 16; i++)
        if(*s) {
            textbuffer[line][i] = *s;
            s++;
        } else
            textbuffer[line][i] = ' ';
}


void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;

			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

uint8_t game[128*32] = {0};

void clearGame() {
    int i;
    for (i = 0; i < sizeof(game); i++) { game[i] = 0; }
}

void renderScreen(uint8_t arr[]) {
    int i, j;

    for(i = 0; i < 4; i++)
    {
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0 & 0xF);
        spi_send_recv(0x10 | ((0 >> 4) & 0xF));

        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

        for(j = 0; j < 128; j++)
            spi_send_recv(arr[i*128 + j]);
    }
}


/*******************************************************
 * Turns on one pixel at a time depending on the inputs
 * x y that are saved in the game array to later be
 * shown on screen
 *******************************************************/

void po(int x, int y) {
    short offset = 0;
    if (y > 0) { offset = y / 8; }
    game[offset * 128 + x] |= 1 << (y - offset * 8);
}


/*******************************************************
 * Different player models are display to display
 * animation represents the character movment
 *******************************************************/

void drawPlayer(Player p,short swi) {
    if(swi == 0){
        drawStand(p);
    } else if(swi == 1){
        drawFall(p);
    } else if(swi == 2){
        drawRightRun1(p);
    } else if(swi == 3){
        drawRightRun2(p);
    } else if(swi == 4){
        drawLeftRun1(p);
    } else if(swi == 5){
        drawLeftRun2(p);
    }
}

/*******************************************************
 * Draw map takes in a short that is the offset that
 * moves the map accordingly
 *******************************************************/

void drawmap(short at) {

    int i = 0;
    for (i = at; i < 256; i++) {
        if(!(mapholder[i][0] == 0))
            po(i-at,mapholder[i][0]);
        if(!(mapholder[i][1] == 0))
            po(i-at,mapholder[i][1]);
        if(!(mapholder[i][2] == 0))
            po(i-at,mapholder[i][2]);
        if(!(mapholder[i][3] == 0))
            po(i-at,mapholder[i][3]);
        if(!(mapholder[i][4] == 0))
            po(i-at,mapholder[i][4]);
        if(!(mapholder[i][5] == 0))
            po(i-at,mapholder[i][5]);
    }
}

void drawCoin(short offx, short offy) {
    po(3 + offx, 3 + offy);
    po(4 + offx, 3 + offy);

    po(2 + offx, 4 + offy);
    po(3 + offx, 4 + offy);
    po(4 + offx, 4 + offy);
    po(5 + offx, 4 + offy);


    po(2 + offx, 5 + offy);
    po(3 + offx, 5 + offy);
    po(4 + offx, 5 + offy);
    po(5 + offx, 5 + offy);

    po(2 + offx, 6 + offy);
    po(3 + offx, 6 + offy);
    po(4 + offx, 6 + offy);
    po(5 + offx, 6 + offy);


    po(3 + offx, 7 + offy);
    po(4 + offx, 7 + offy);
}

void drawCoins(short at,short coins) {
    if (coins == 0)
        drawCoin(33,11);
    if (coins == 1)
        drawCoin(33,45);
    if (coins == 2)
        drawCoin(37,13);
    if (coins == 3)
        drawCoin(33,45);
    if (coins == 4)
        drawCoin(64,15);
    if (coins == 5)
        drawCoin(33,45);
}
void drawLive(short offset) {
    po(3 + offset, 2);
    po(5 + offset, 2);
    po(2 + offset, 3);
    po(3 + offset, 3);
    po(4 + offset, 3);
    po(5 + offset, 3);
    po(6 + offset, 3);
    po(3 + offset, 4);
    po(4 + offset, 4);
    po(5 + offset, 4);
    po(4 + offset, 5);
}

void drawLives(short lives) {
    if(lives >= 1)
        drawLive(0);
    if(lives >= 2)
        drawLive(6);
    if(lives >= 3)
        drawLive(12);
    if(lives >= 4)
        drawLive(18);
    if(lives >= 5)
        drawLive(24);
}

/*******************************************************
 * The draw funtion draws every frame of the game
 *******************************************************/

void draw1 (Player player, short m, short swi, short lives, short coins) {

	clearGame();
    drawLives(lives);
	drawPlayer(player,swi);
    drawCoins(m, coins);
    drawmap(m);
	renderScreen(game);
}
