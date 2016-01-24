//
//  a2graph.h
//  test
//
//  Created by Paulo Garcia on 2015-11-26.
//  Copyright © 2015 Paulo Garcia. All rights reserved.
//

#ifndef a2graph_h
#define a2graph_h

#define PAGE_1     0
#define PAGE_2     1024

#define MODE_TEXT_PAGE_1             0x0
#define MODE_TEXT_PAGE_2             0x1
#define MODE_LORES_FULL_PAGE_1       0x2
#define MODE_LORES_FULL_PAGE_2       0x3
#define MODE_HIRES_FULL_PAGE_1       0x4
#define MODE_HIRES_FULL_PAGE_2       0x5
#define MODE_LORES_TEXT_PAGE_1       0x6

/* Colors */

typedef enum {
    BLACK = 0,
    MAGENTA,
    DARK_BLUE,
    PURPLE,
    DARK_GREEN,
    GREY,
    MEDIUM_BLUE,
    LIGHT_BLUE,
    BROWN,
    ORANGE,
    GREY_2,
    PINK,
    GREEN,
    YELLOW,
    AQUA,
    WHITE
} colors_t;

/* base addresses for primary text page */
/* also the base addresses for the 48 scanline pairs */
/* for lores graphics mode 40 x 48 x 16 colors */
int textbase[24]={
    0x0400,
    0x0480,
    0x0500,
    0x0580,
    0x0600,
    0x0680,
    0x0700,
    0x0780,
    0x0428,
    0x04A8,
    0x0528,
    0x05A8,
    0x0628,
    0x06A8,
    0x0728,
    0x07A8,
    0x0450,
    0x04D0,
    0x0550,
    0x05D0,
    0x0650,
    0x06D0,
    0x0750,
    0x07D0
};

/* Function Prototypes */
extern void gr_setmode(unsigned int CRTMODE);
extern void gr_plot(int x,int y, colors_t color,int page);
extern void gr_hlin(unsigned int x1, unsigned int x2, unsigned int y, colors_t color, int page);
extern void gr_vlin(unsigned int y1, unsigned int y2, unsigned int x, colors_t color, int page);
extern void gr_fill(int page, unsigned ch);
extern int gr_khit();
extern void gr_clearkey();
extern void gr_sprite(unsigned int x, unsigned int y, unsigned int w, unsigned int h, int page, unsigned char *);

/* private functions */
void gr_plot1(int x, int y, unsigned char c2);

#endif /* a2graph_h */
