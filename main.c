/*
 * main.c
 * gr2
 *
 * Created by Paulo Garcia on 2015-12-16.
 * Copyright (c) 2015 Paulo Garcia. All rights reserved.
 *
 */


#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <peekpoke.h>

#include "a2graph.h"

unsigned char sprite[36] = { BLACK, BLACK, BLACK, MAGENTA, MAGENTA, BLACK,
                            BLACK, BLACK, BLACK, MAGENTA, BLACK, BLACK,
                            BLACK, WHITE, BLACK, MAGENTA, BLACK, BLACK,
                            WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                            BLACK, BLACK, BLACK, MAGENTA, BLACK, BLACK,
                            BLACK, BLACK, BLACK, MAGENTA, MAGENTA, BLACK};

int my_test()
{
    int page=0;
    int x,y;
    //fill_screen(PAGE_1,0);
    for (x=5;x<35;x++) {
        for(y=10;y<36;y+=2) {
            gr_plot(x, y, DARK_BLUE, page);
        }
    }
    for (y=10;y<35;y++) {
        for(x=5;x<36;x+=2) {
            gr_plot(x, y, YELLOW, page);
        }
    }
}

int main(void)
{
    int i,j;
    
    gr_clearkey();
    printf("Press any key to start...");
    while(gr_khit()==0);

    gr_setmode(MODE_LORES_TEXT_PAGE_1);
    gr_fill(PAGE_1, BLACK);

    while(gr_khit()==0);
    
    my_test();
    j=0;
    for(i=0;i<48;i++) {
        gr_hlin(39,0,i,j,PAGE_1);
        j++;
        if (j==16) j=0;
    }

    j=0;
    for(i=0;i<40;i++) {
        gr_vlin(47,0,i,j,PAGE_1);
        j++;
        if (j==16) j=0;
    }
    gr_clearkey();
    while(gr_khit()==0);

    for (i=33;i>0;i--) {
        gr_sprite(i, 10, 6, 6, PAGE_1, sprite);
    }
    
    gr_clearkey();
    while(gr_khit()==0);
    for (i=0;i<16;i++) {
        gr_fill(0, i);
    }
    
    return 0;
}
