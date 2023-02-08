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


char towriteout[9] = "ss abcdef";
char letter1 = 78;
char letter2 = 78;
char  timetemp[5];
short temparray[10][2];
short notzero = 0;



void delay(int k) {
    int j = 0;
    while (k > 0){
        for (j = 0; j < 7500; j++){
        }
        k--;
    }
}

int getbtns(void){
    return ((PORTD >> 5)&0x07);     
}




void btnpress() {
    int btn = getbtns();
    if(btnpressed == 0 && btn == 4 && (player.y == mapholder[player.x + map_mover][0]-1 || 
        player.y == mapholder[player.x-4 + map_mover][0]-1)) {            
        btnpressed = 1;  
        delay(40);
    }    
}

void walkingRight()  {
    if (player.x != 125 && map_mover < 256 && !((mapholder[player.x + 1 + map_mover][0] == player.y))) {
        if (player.x  + 2 > 90 && map_mover < 128) {
            if((player.y == mapholder[player.x + map_mover][0]-1 || 
        player.y == mapholder[player.x-4 + map_mover][0]-1))
                delay(7);
            if(map_mover < 128){
                map_mover += 1;
                animationstate = runningRight();
            }
        } else {
            if (!((mapholder[player.x + 1 + map_mover][0] == player.y))){        
                player.x += 1;
                if((player.y == mapholder[player.x + map_mover][0]-1 || 
        player.y == mapholder[player.x-4 + map_mover][0]-1)){
                    delay(7);
                    animationstate = runningRight();
                }
            }   
        }
    }
}

void walkingLeft() {
    if (player.x != 6 && !((mapholder[player.x - 1 + map_mover][0] == player.y))) {
        if (player.x -1  < 20 && map_mover > 0) {
            if((player.y == mapholder[player.x + map_mover][0]-1 || 
        player.y == mapholder[player.x-4 + map_mover][0]-1))
                    delay(7);
            
            map_mover -= 1;
            animationstate = runningLeft();
        } else {
            if (!((mapholder[player.x - 1 + map_mover][0] - player.y) <= 0)) {
                player.x -= 1;
                // coins +=1;
                if((player.y == mapholder[player.x + map_mover][0]-1 || 
        player.y == mapholder[player.x-4 + map_mover][0]-1)){
                    delay(7);
                    animationstate = runningLeft();
                }
                    
            }
        }
    }    
}

void respawn() {
    if (player.y > 40){
        map_mover = 0;
        player.x = 9;
        player.y = 15;
        lives -= 1;
        draw1(player,map_mover,1,lives);

    }
}

void gravity() {
    if(!(player.y == mapholder[player.x + map_mover][0]-1 || 
        player.y == mapholder[player.x-4 + map_mover][0]-1) && btnpressed == 0)
    {
        animationstate = 1;
        if (player.y != 50)
        {
            if (player.y > 40) {
                delay(60);
                respawn();
            }    
            player.y += 1;
            delay(4);;
        }
    } else if (animationstate == 1) {

        animationstate = 0;
    }
}

void jumping() {
    if(btnpressed == 1) {
            if(player.y != 2 && jumpcount != 0){
                player.y -= 1;
                jumpcount -= 1;
                delay(4);;

            } else {
                delay(20);
                btnpressed = 0;
                jumpcount  = 20;
            }
    }    
}

void gameSetup() {
    IFSCLR(1) = 0x0002;
    AD1CON1SET = 0x0004;
    while (!IFS(1) & 0x0002);
    
    ValueofX = 512;
    ValueofY = 512;

    if (AD1CON2 & 0x0080) {
        ValueofX = ADC1BUF0;
        ValueofY = ADC1BUF1;
    } else {
        ValueofX = ADC1BUF8;
        ValueofY = ADC1BUF9;
    }    
}



void letterSwitch(char letter, short pick) {
    towriteout[8] = '\0';
    while(1) {
        gameSetup();
        if (ValueofY < 200) {
            if (letter > 64 || letter < 91) {
                if(letter == 65){
                    letter = 90;
                } else {
                    letter -= 1;
                } 
            }
                
        } else if (ValueofY > 800) {
            if (letter > 64 || letter < 91) {
                if(letter == 90){
                    letter = 65;
                } else {
                    letter += 1;
                } 
            }
        }
        towriteout[pick] = letter;
        int i = 0;
        for (i = 0; i < 5; i++) {
            towriteout[i+3] = timetemp[i];
        }
        delay(40);
        display_string(2, towriteout);
        display_update();
        startbtn = getbtns();
        if (startbtn == 4){ 
            break;
        }
    }
}




void saveUser() {
        display_string(0, "Enter your ");
        display_string(1, "initials");
        display_string(3, "---------------------------");
        // int fn = checking();
        // scores[fn][0] = tempvalue;
        // sort_numbers_ascending(10);
        // high1 = 23498; 
        letterSwitch(letter1,0);   
        delay(30); 
        letterSwitch(letter2,1);
        int i = 0;

        if(tempvalue <= high1 ) {
            for (i = 0; i < 9; i++)
                highScore4[i] = highScore3[i];
            for (i = 0; i < 9; i++)
                highScore3[i] = highScore2[i];
            for (i = 0; i < 9; i++)
                highScore2[i] = highScore1[i];
            for (i = 0; i < 9; i++)
                highScore1[i] = towriteout[i];

            high4 = high3;
            high3 = high2;
            high2 = high1;
            high1 = tempvalue;
            
        } else if(tempvalue <= high2) {
            for (i = 0; i < 9; i++)
                highScore4[i] = highScore3[i];
            for (i = 0; i < 9; i++)
                highScore3[i] = highScore2[i];
            for (i = 0; i < 9; i++)
                highScore2[i] = towriteout[i];
                        high4 = high3;

            high4 = high3;
            high3 = high2;
            high2 = tempvalue;
            
        } else if(tempvalue <= high3)
          {
            for (i = 0; i < 9; i++)
                highScore4[i] = highScore3[i];
            for (i = 0; i < 9; i++)
                highScore3[i] = towriteout[i];

            high4 = high3;
            high3 = tempvalue;
            
        } else if(tempvalue <= high4) {
            for (i = 0; i < 9; i++)
                highScore4[i] = towriteout[i];
            high4 = tempvalue;            
          }

     
}

void highScoreRun() {
    // *highScore[1] = "asdkjhfds";
    display_string(0,"temp1");
    display_string(1,"temp2");
    display_string(2,"temp3");
    display_string(3,"temp4");
    display_update();
    display_string(0, highScore1);
    display_string(1, highScore2);
    display_string(2, highScore3);
    display_string(3, highScore4);
    display_update();

    delay(1500);
}


void mapswitch(){
    if(currentmap == 1){
        mapcopy(map1);
    } else if (currentmap == 2) {
        mapcopy(map2);
        coins += 1;
    } else if (currentmap == 3) {
        mapcopy(map3);
        coins += 1;
    } else if (currentmap == 4) {
        int i =0;
        gameBool = 0;
        tempvalue = mytime;
        time2string(textstring, mytime);
        for (i = 0; i < 5; i++) {
            timetemp[i] = textstring[i];
        }
    }
    map_mover = 0;
    player.x = 9;
    player.y = 15;
    draw1(player,map_mover,1);
}

void winnerflag() {
    if (player.x > 123 && (player.y == mapholder[player.x + map_mover][0]-1 || 
        player.y == mapholder[player.x-4 + map_mover][0]-1)) {
        player.y -= 1;
        currentmap += 1;
        mapswitch();        
    }
}

void coinSystem() {

    if (map_mover == 0){    
        if(currentmap == 1 && player.x == 33 && player.y > 8 && player.y < 28 && coins == 0) {
            coins += 1;
        }
        if(currentmap == 2 && player.x == 34 && player.y > 8 && player.y < 28 && coins == 2) {
            coins += 1;
        }
        if(currentmap == 3 && player.x == 60 && player.y > 8 && player.y < 28 && coins == 4) {
            coins += 1;
        }
    }
}

void gameinit() {
    spiinit();
    display_init();
    player.x = 9;
    player.y = 15;
    mapcopy(map1);
    draw1(player);
    coinSystem(currentmap, coins, map_mover);

}
 
void joyStickSetup() {
    // reset
    AD1CON1 = 0x0000;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0000;
    
    AD1CON1SET = 0x4;
    AD1CON1SET = 0xE0;
    AD1CON1SET = 0x400;
    AD1CON2SET = 0x2;
    AD1CON2SET = 0x4;
    AD1CON2SET = 0x400;
    AD1CON3SET = 0xF00;
    AD1CSSLSET = 0x0110;
    AD1CON1SET = 0x8000;

}



void startMenu() {
    startbtn = getbtns();
    
    if (startbtn == 4){
        if (aspect == 2){
            aspect = 0;
        } else {
            aspect += 1;
        }
    } 
    if (startbtn == 2) {
        if (aspect == 0){
            aspect = 2;
        } else {
            aspect -= 1;
        }
    } 

    if (!gameBool) {
        if(aspect == 0){
            delay(55);
            renderScreen(gameMenu);
        }
        if (aspect == 1) {
            delay(55);
            renderScreen(highScoreMenu);
        } 
        if (aspect == 2) {
            delay(55);
            renderScreen(creditMenu);
        }       
    }    
}





void time() {
    T2CONSET = 0x70;    
    PR2 = 31250;      
    TMR2 = 0;          
    T2CONSET = 0x8070;  
    IEC(0) = IEC(0)|0x100;
    IPC(2) = IPC(2)|0X1F;
    enable_interrupt();
}

void gameReset() {
    lives = 5;
    coins = 0;
    map_mover = 0;
    currentmap = 1;
    mapswitch();
}

void gameRun() {

    while(lives != 0 && gameBool == 1) {   
        gameSetup();
        winnerflag();
        btnpress();
        jumping();

        if(ValueofX > 800) {
            walkingRight();
            coinSystem();
        }

        if (ValueofX < 200) {
            walkingLeft();
        }
        coinSystem();
        gravity();

        draw1(player,map_mover,animationstate,lives,coins);    
    }

    if(lives != 0){
        saveUser();
    } else {    
        renderScreen(gameOverScreen);
    }
    
    delay(1000);
    gameBool = 0;
}


