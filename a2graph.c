//
//  a2graph.c
//  test
//
//  Created by Paulo Garcia on 2015-11-26.
//  Copyright © 2015 Paulo Garcia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <peekpoke.h>

#include "a2graph.h"

void gr_setmode(unsigned int CRTMODE)
{
    /* note: the following constants must be hex values
     or cc65 complains that the constant is a long */
    switch(CRTMODE)
    {
        case MODE_TEXT_PAGE_1:
            POKE(0xc051,0);  /* text */
            POKE(0xc054,0);  /* page 1 */
            break;
        case MODE_TEXT_PAGE_2:
            POKE(0xc051,0);  /* text */
            POKE(0xc055,0);  /* page 2 */
            break;
        case MODE_LORES_TEXT_PAGE_1:
            POKE(0xc056,0);   /* lo res */
            POKE(0xc054,0);   /* page 1 */
            POKE(0xc050,0);   /* set graphics */
            break;
        case MODE_LORES_FULL_PAGE_1:
            POKE(0xc056,0);   /* lo res */
            POKE(0xc054,0);   /* page 1 */
            POKE(0xc050,0);   /* set graphics */
            POKE(0xc052,0);   /* full graphics */
            break;
        case MODE_LORES_FULL_PAGE_2:
            POKE(0xc056,0);   /* lo res */
            POKE(0xc055,0);   /* page 2 */
            POKE(0xc050,0);   /* set graphics */
            POKE(0xc052,0);   /* full graphics */
            break;
        case MODE_HIRES_FULL_PAGE_2:
            POKE(0xc057,0);   /* hi res */
            POKE(0xc055,0);   /* page 2 */
            POKE(0xc050,0);   /* set graphics */
            POKE(0xc052,0);   /* full graphics */
            break;
        case MODE_HIRES_FULL_PAGE_1:
        default:
            POKE(0xc057,0);   /* hi res */
            POKE(0xc054,0);   /* page 1 */
            POKE(0xc050,0);   /* set graphics */
            POKE(0xc052,0);   /* full graphics */
    }
}




void gr_fill(int page, unsigned ch)
{
    int idx;
    unsigned fillcolor = ch * 0x11;
    
    for (idx = 0; idx < 8; idx++) {
        memset ((char *)textbase[idx]+page, fillcolor, 0x78);
    }
}

void gr_hlin(unsigned int x1, unsigned int x2, unsigned int y, colors_t color, int page)
{

    int length;
    int y1, pos;
    unsigned char c2 = (unsigned char ) ((int)color & 15);
    unsigned char *crt, c1;
    
    if (x2<x1) {
        //swap
        pos=x2;
        x2=x1;
        x1=pos;
    }
    
    length = x2-x1;
    
    y1 = y >> 1;
    x1 = x1 + page;

    if (y&1) {
        /* odd rows in high nibble mask value to preserve low nibble */
        c1 = 0xf;
        c2 = c2 << 0x4;
    } else {
        /* even rows in low nibble mask value to preserve high nibble */
        c1 = 0xf0;
    }
    
    crt = malloc(length);
    
    // Copy video memory first
    memcpy((void *)crt, (void *)(textbase[y1]+x1), length);
    
    // Apply color to all positions
    
    if (crt) {
        for (pos=0;pos<length;pos++) {
            *(crt+pos) = (*(crt+pos) & c1) | c2;
        }
        // Copy it back to video position
        memcpy((void *)(textbase[y1]+x1), (void *)crt, length);
        free(crt);
    }
}

void gr_vlin(unsigned int y1, unsigned int y2, unsigned int x, colors_t color, int page)
{
    int pos;
    unsigned char c2;
    
    if (y2<y1) {
        //swap
        pos=y2;
        y2=y1;
        y1=pos;
    }
    
    x = x + page;
    c2 = (unsigned char ) ((int)color & 15);
    
    for (pos=y1;pos<=y2;pos++) {
        gr_plot1(x,pos,c2);
    }
    
}

void gr_plot(int x,int y,colors_t color,int page)
{
    unsigned char c2;
    
    x = x + page;
    c2 = (unsigned char ) ((int)color & 15);
   
    gr_plot1(x,y,c2);

}

/*
  Internal plot - don't call directly
  x already account for page, and color is calculated too
 */
void gr_plot1(int x, int y, unsigned char c2)
{
    unsigned char *crt, c1;
    int y1 = y >> 1;
    int *pos = textbase;
    
    if (y&1) {
        /* odd rows in high nibble */
        /* mask value to preserve low nibble */
        c1 = 0xf;
        c2 = c2 << 0x4;
    } else {
        /* even rows in low nibble */
        /* mask value to preserve high nibble */
        c1 = 0xf0;
    }
    
    crt = (unsigned char *)(*(pos+y1))+x;
    *crt = (*crt & c1) | c2;
}

int gr_khit()
{
    unsigned char *KP = (unsigned char*)0xC000;
    unsigned char c;
    
    /* read the keyboard buffer    */
    /* and return 0 if no character is waiting */
    
    c = KP[0];
    if(c<128)return 0;
    return (int)c;
}

void gr_clearkey()
{
    
    /* return the last key press  */
    unsigned char *KEYPRESS = (unsigned char*)0xC000;
    /* clear the last key press   */
    unsigned char *KEYCLEAR = (unsigned char*)0xC010;
    
    /* clear stragglers from the keyboard buffer */
    while(KEYPRESS[0] > 127)KEYCLEAR[0]=0;
}

void gr_sprite(unsigned int x, unsigned int y, unsigned int w, unsigned int h, int page, unsigned char* cl)
{
    int x1,y1;
    unsigned char ci = 0;
    unsigned char *cl1 = cl;
    int plt =0;    // 0 = with mask - 1 = full byte

    x = x + page;

    y1 = y;

    do {
        plt=0;
        if (!(y&1) && (y1<(y+h-1))) {
            // even row
            plt=1; //doulbe
        }
        
        if (plt) {
            y1++;
        } else {
            // plot single line
            for (x1=x;x1<(x+w);x1++) {
                gr_plot1(x1,y1,*(cl1+ci));
                ci++;
            }
        }
        y1++;
    } while (y1<(y+h));

}

