#include <stdint.h>
#include <stdio.h>
#include <pic32mx.h>
#include "display.h"
#include "main.h"
#include "displaysetup.h"
#include "mipslab.h"
#include "objects.h"
#include "map.h"
#include "mainfunc.h"

short playerani = 0;

/*******************************************************
 * Animations
 *******************************************************/

short runningRight() {
    short state = 0;
    if(player.x % 6 == 0){
        state = 2;
    } else {
        state = 3;
    }
    return state;
}

short runningLeft() {
    short state = 0;
    if(player.x % 6 == 0){
        state = 4;
    } else {
        state = 5;
    }
    return state;
}

/*******************************************************
 * Idel standing animation
 *******************************************************/

void drawStand(Player p) {
    po((p.x),   (p.y));
    po((p.x),   (p.y-3));
    po((p.x-3), (p.y-3));
    po((p.x-3), (p.y));
    po((p.x-2), (p.y-1));
    po((p.x-2), (p.y-2));
    po((p.x-4), (p.y-2));
    po((p.x-2), (p.y-3)); 
    po((p.x-2), (p.y-4)); 
    po((p.x-1), (p.y-1));
    po((p.x-1), (p.y-2));
    po((p.x+1), (p.y-2));
    po((p.x-1), (p.y-3));
    po((p.x-1), (p.y-4));

    po((p.x), (p.y-5));
    po((p.x), (p.y-6));
    po((p.x+1), (p.y-7));
    po((p.x), (p.y-8));

    po((p.x-1), (p.y-6));
    po((p.x-1), (p.y-7));
    po((p.x-1), (p.y-8));

    po((p.x-2), (p.y-5));
    po((p.x-2), (p.y-6));
    po((p.x-4), (p.y-7));
    po((p.x-2), (p.y-8));

    po((p.x-3), (p.y-5));
    po((p.x-3), (p.y-6));
    po((p.x-3), (p.y-7));
    po((p.x-3), (p.y-8));

}

/*******************************************************
 * Falling animation
 *******************************************************/

void drawFall(Player p) {
    po((p.x),   (p.y));
    po((p.x),   (p.y-3));
    po((p.x-3), (p.y-3));
    po((p.x-3), (p.y));
    po((p.x-2), (p.y-1));
    po((p.x-2), (p.y-2));
    po((p.x-4), (p.y-4));
    po((p.x-5), (p.y-4));
    po((p.x-2), (p.y-3)); 
    po((p.x-2), (p.y-4)); 
    po((p.x-1), (p.y-1));
    po((p.x-1), (p.y-2));
    po((p.x+1), (p.y-4));
    po((p.x+2), (p.y-5));
    po((p.x-1), (p.y-3));
    po((p.x-1), (p.y-4));

    po((p.x), (p.y-5));
    po((p.x), (p.y-6));
    po((p.x+1), (p.y-7));
    po((p.x), (p.y-8));

    po((p.x-1), (p.y-6));
    po((p.x-1), (p.y-7));
    po((p.x-1), (p.y-8));

    po((p.x-2), (p.y-6));
    po((p.x-4), (p.y-7));
    po((p.x-2), (p.y-8));

    po((p.x-3), (p.y-5));
    po((p.x-3), (p.y-6));
    po((p.x-3), (p.y-7));
    po((p.x-3), (p.y-8));

}

/*******************************************************
 * Running Right animation 1
 *******************************************************/

void drawRightRun1(Player p) {
    po((p.x),   (p.y));
    po((p.x),   (p.y-2));
    po((p.x-3), (p.y-3));
    po((p.x-4), (p.y));
    po((p.x-3), (p.y-1));
    po((p.x-2), (p.y-2));
    po((p.x-4), (p.y-2));
    po((p.x-2), (p.y-3)); 
    po((p.x-2), (p.y-4)); 
    po((p.x-1), (p.y-1));
    po((p.x-1), (p.y-2));
    po((p.x+1), (p.y-3));
    po((p.x-1), (p.y-3));
    po((p.x-1), (p.y-4));

    po((p.x), (p.y-5));
    po((p.x), (p.y-6));
    po((p.x+1), (p.y-7));
    po((p.x), (p.y-8));

    // po((p.x-1), (p.y-5));
    po((p.x-1), (p.y-6));
    po((p.x-1), (p.y-7));
    po((p.x-1), (p.y-8));

    po((p.x-2), (p.y-5));
    po((p.x-2), (p.y-6));
    po((p.x-4), (p.y-7));
    po((p.x-2), (p.y-8));

    po((p.x-3), (p.y-5));
    po((p.x-3), (p.y-6));
    po((p.x-3), (p.y-7));
    po((p.x-3), (p.y-8));

}

/*******************************************************
 * Running Right animation 2
 *******************************************************/

void drawRightRun2(Player p) {
    po((p.x),   (p.y));
    po((p.x),   (p.y-3));
    po((p.x),   (p.y-2));
    po((p.x-3), (p.y-3));
    po((p.x-3), (p.y));
    po((p.x-2), (p.y-1));
    po((p.x-2), (p.y-2));
    po((p.x-4), (p.y-2));
    po((p.x-4), (p.y-3));
    po((p.x-2), (p.y-3)); 
    po((p.x-2), (p.y-4)); 
    po((p.x), (p.y-1));
    po((p.x-1), (p.y-2));
    po((p.x+1), (p.y-2));
    po((p.x-1), (p.y-3));
    po((p.x-1), (p.y-4));

    po((p.x), (p.y-5));
    po((p.x), (p.y-6));
    po((p.x+1), (p.y-7));
    po((p.x), (p.y-8));

    po((p.x-1), (p.y-6));
    po((p.x-1), (p.y-7));
    po((p.x-1), (p.y-8));

    po((p.x-2), (p.y-5));
    po((p.x-2), (p.y-6));
    po((p.x-4), (p.y-7));
    po((p.x-2), (p.y-8));

    po((p.x-3), (p.y-5));
    po((p.x-3), (p.y-6));
    po((p.x-3), (p.y-7));
    po((p.x-3), (p.y-8));
}

/*******************************************************
 * Running Left animation 1
 *******************************************************/

void drawLeftRun1(Player p) {
    po((p.x+1),   (p.y-2));

    po((p.x),   (p.y));
    po((p.x), (p.y-3));

    
    
    po((p.x-3), (p.y-1));
    po((p.x-3), (p.y-2));
    
    po((p.x-2), (p.y-3)); 
    po((p.x-2), (p.y-5)); 
    po((p.x-2), (p.y-4)); 
    po((p.x-2), (p.y-2));
    po((p.x-4), (p.y-3));
    po((p.x-1), (p.y-1));
    po((p.x-1), (p.y-2));
    
    po((p.x-1), (p.y-3));
    po((p.x-1), (p.y-4));

    po((p.x), (p.y-5));
    po((p.x), (p.y-6));
    po((p.x), (p.y-7));
    po((p.x+1), (p.y-7));
    po((p.x), (p.y-8));

    po((p.x-4), (p.y));
    po((p.x-1), (p.y-6));
    po((p.x-2), (p.y-7));
    po((p.x-1), (p.y-8));


    po((p.x-2), (p.y-6));
    po((p.x-4), (p.y-7));
    po((p.x-2), (p.y-8));

    po((p.x-3), (p.y-5));
    po((p.x-3), (p.y-6));
    po((p.x-2), (p.y-7));
    po((p.x-3), (p.y-8));

}

/*******************************************************
 * Running Left animation 2
 *******************************************************/

void drawLeftRun2(Player p) {
    po((p.x+1),   (p.y));
    po((p.x-3),   (p.y-3));
    po((p.x-1),   (p.y-2));
    po((p.x-3), (p.y-3));
    po((p.x-2), (p.y));
    po((p.x-2), (p.y-1));
    po((p.x-2), (p.y-2));
    po((p.x-4), (p.y-2));
    po((p.x+1), (p.y-3));
    po((p.x-2), (p.y-3)); 
    po((p.x-2), (p.y-4)); 
    po((p.x-2), (p.y-5)); 
    po((p.x), (p.y-1));
    po((p.x), (p.y-3));
    po((p.x-1), (p.y-2));
    po((p.x+1), (p.y-2));
    po((p.x-1), (p.y-3));
    po((p.x-1), (p.y-4));

    po((p.x), (p.y-7));
    po((p.x), (p.y-5));
    po((p.x), (p.y-6));
    po((p.x+1), (p.y-7));
    po((p.x), (p.y-8));

    po((p.x-2), (p.y-7));
    po((p.x-2), (p.y-7));
    po((p.x-1), (p.y-6));

    po((p.x-1), (p.y-8));


    po((p.x-2), (p.y-6));
    po((p.x-4), (p.y-7));
    po((p.x-2), (p.y-8));

    po((p.x-3), (p.y-5));
    po((p.x-3), (p.y-6));
    po((p.x-3), (p.y-8));
}