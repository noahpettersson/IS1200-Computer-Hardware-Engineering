#include <stdint.h>
#include <stdio.h>
#include <pic32mx.h>
#include "display.h"
#include "assets.h"
#include "main.h"
#include "displaysetup.h"
#include "mipslab.h"
#include "objects.h"
#include "map.h"
#include "playerani.h"
#include "mainfunc.h"


/*******************************************************
 * All the initial variables that will be used 
 *******************************************************/
short map_mover   = 0;
int offset      = 0;
int btnpressed  = 0;
int jumpcount   = 20;
short currentmap = 1;
short animationstate    = 0;
short mapholder[256][6] = {0};
short lives = 5;
short coins = 0;
int ValueofX;
int ValueofY;
int startbtn;
short aspect = 0;
short gameBool = 0;
int mytime = 0x0000;
int timeoutcount = 0;
char textstring[] = "00:00";
int tempvalue;
short high1;
short high2;
short high3;
short high4;
short highTemp;
char highScore1[9];
char highScore2[9];
char highScore3[9];
char highScore4[9];
char highScoreTemp[9];


/*******************************************************
 * This is the main function where all the code will 
 * be run
 *******************************************************/

void main() {
    high1 = 23498; 
    high2 = 23498;
    high3 = 23498;
    high4 = 23498;
    gameinit();
    joyStickSetup();
    renderScreen(titleScreen);
    delay(1000);
    while(1)
    {
        startMenu();
        if (startbtn == 1){
            if(aspect == 0){
                gameBool = 1;
                time();
                mytime = 0x0000;
                while(gameBool){
                    gameRun();    
                }
                gameinit();
                gameReset();
                renderScreen(gameMenu);
                startMenu();
                
            } 
            if (aspect == 1) {
                highScoreRun();
            }
            if (aspect == 2 && startbtn == 1) {
                renderScreen(actuallcredits);
                delay(1500);
            }
        }

        
    }
}
