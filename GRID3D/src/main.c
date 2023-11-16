////////////////////////////////////////
// { GRID3D } { VERSION }
// Author:
// License:
// Description: Its like 3d squares
////////////////////////////////////////

/*
* The comments in this file are here to guide you initially. Note that you shouldn't actually
* write comments that are pointless or obvious in your own code, write useful ones instead!
* See this for more details: https://ce-programming.github.io/toolchain/static/coding-guidelines.html
*
* Have fun!
*/

/* You probably want to keep these headers */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

/* Here are some standard headers. Take a look at the toolchain for more. */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphx.h>
#include <keypadc.h>
/* Put function prototypes here or in a header (.h) file */

/* Note: uint8_t is an unsigned integer that can range from 0-255. */
/* It performs faster than just an int, so try to use it (or int8_t) when possible */
void printText(const char *text, uint8_t x, uint8_t y);

typedef struct {
    int16_t x;
    int16_t y;
} vert2D;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} vert3D;


struct camera {
    int x;
    int y;
    int z;
    int rotX;
} Camera;

typedef struct {
    vert3D verts[3];
} triangle;

const int16_t numPlaneVerts = 1;
struct PlaneGeometry {
    vert3D verts[numPlaneVerts];
    int16_t lineTo[numPlaneVerts];
};


int cos_cache[360];

void draw3D(int camY, int camX, int camZ, int rotX) {
    uint16_t startZ = (camY / 1200 * 10) + camZ%10+1;
    uint16_t z = (camY / 1200 * 10) + camZ%10+1; // (camY / 1200 * 10) +
    uint16_t prevCalcZ = 240;
    uint16_t calcZ;
    uint16_t calcX0;
    bool isRed = true;

    for(uint16_t i = 0; i < 10; i++) {
        calcZ = ((camY/z)+120);
        calcX0 = 320;

        if(prevCalcZ > 240)
            prevCalcZ = 240;
        gfx_SetColor(224);

        gfx_Line(0,calcZ,calcX0,calcZ);
        z+=10;
        prevCalcZ=calcZ;
    }

    // TODO: fancy algebra to clip line

    // * (1000/cos_cache[rotX])

    for(int16_t i = -160+Camera.x%10; i < 160; i+=10) {
        gfx_Line(
            (i*(45000/(camY/10)))/20 + (1000/cos_cache[rotX]) * 160,
            240,
            i + 160,
            calcZ
        );
    }

}

int main() {

    boot_Set48MHzMode();

    uint8_t backc = 0x00;
    os_ClrHomeFull();
    gfx_Begin(gfx_8bpp);
    //gfx_SetDrawBuffer(); Use this in a loop or something

    float float_i = 0;
    for(int i=0; i < 360; i++){
        cos_cache[i] = (int)(cos(float_i*0.01745)*1000);//*(r_quality)); // all floating point ints are scaled by 1k, also stored in degrees
        float_i++;
    }

    gfx_SetTextFGColor(0x00);
    gfx_SetColor(255);

    Camera.y = 6000;

    while (true){ // Refer to https://ce-programming.github.io/toolchain/libraries/keypadc.html for key register info
        gfx_BlitScreen();
        gfx_FillScreen(backc);

        kb_Scan();
        draw3D(Camera.y, Camera.x, Camera.z, Camera.rotX);

        if (kb_Data[1] & kb_2nd) {
            Camera.z+=1;
        }
        if(kb_Data[1] & kb_Mode){
            Camera.z-=1;
        }

        if(kb_Data[7] & kb_Up){
            Camera.y+=100;
        }
        if(kb_Data[7] & kb_Down){
            Camera.y-=100;
        }
        if(kb_Data[7] & kb_Right){
            Camera.x-=1;
        }
        if(kb_Data[7] & kb_Left){
            Camera.x+=1;
        }

        if(kb_Data[3] & kb_Sin){
            Camera.rotX+=20;
        }
        if(kb_Data[4] & kb_Cos){
            Camera.rotX-=20;
        }

        if(kb_On){
            break; //Stops program
        }
        gfx_SwapDraw();
    }

        //A for loop that copies temp_life to lifez
    gfx_End();
    return 0;
}
