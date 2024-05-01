#ifndef NIKDRAW_H
#define NIKDRAW_H


#define ND_TEST_EVERYTHING 1


//#ifdef NIK_NIKDRAWING


//#include "niktext.h"


#include "../../more_src/nlib/nlibdbug.h"


///////////////

// Opaque

#define NIK_DRAWABLE_BITMAP void
#define NIK_BITMAP void
#define NIK_PALETTE void


///////////////

/* Useful triplet breaking macros... */
/* These are for getting 8 bit colors out of a 24 bit triplet */
#define nikdrawGetRGBRed24(x)   ((unsigned int)((x>>16)&0xff))
#define nikdrawGetRGBGreen24(x) ((unsigned int)((x>>8)&0xff))
#define nikdrawGetRGBBlue24(x)  ((unsigned int)(x&0xff))
/* These are for getting 4 bit colors out of a 12 bit triplet */
#define nikdrawGetRGBRed12(x)   ((unsigned int)((x>>8)&0xf))
#define nikdrawGetRGBGreen12(x) ((unsigned int)((x>>4)&0xf))
#define nikdrawGetRGBBlue12(x)  ((unsigned int)(x&0xf))
/* These are for getting 3 bit colors out of a 9 bit triplet */
#define nikdrawGetRGBRed9(x)    ((unsigned int)((x>>6)&0x7))
#define nikdrawGetRGBGreen9(x)  ((unsigned int)((x>>3)&0x7))
#define nikdrawGetRGBBlue9(x)   ((unsigned int)(x&0x7))
/* Build 24 bit triplet from 8 bit colors */
#define nikdrawMakeRGB24(x,y,z) ((z&0xff)+((y&0xff)<<8)+((x&0xff)<<16))
/* Build 12 bit triplet from 4 bit colors */
#define nikdrawMakeRGB12(x,y,z) ((z&0xf)+((y&0xf)<<4)+((x&0xf)<<8))
/* Build 9 bit triplet from 3 bit colors */
#define nikdrawMakeRGB9(x,y,z) ((z&0x7)+((y&0x7)<<3)+((x&0x7)<<6))


/* //////////////////////////////////////////////////////////////////////////////
//  Start of globals
/////////////////////////////////////////////////////////////////////////////// */

/*
WARNING! WARNING! WARNING!

Don't assume ANY of these are valid or useful or implemented!
*/

extern unsigned int nikdrawColorRed;
extern unsigned int nikdrawColorGreen;
extern unsigned int nikdrawColorBlue;
extern unsigned long nikdrawColorLong;


extern unsigned int nikdrawWidth;
extern unsigned int nikdrawHeight;
extern unsigned int nikdrawDepth;

extern unsigned int nikdrawBytesPerPixel;
extern unsigned char *nikdrawThePixels;
extern unsigned int nikdrawThePixelsSize;


/* //////////////////////////////////////////////////////////////////////////////
//  Start of drawing
/////////////////////////////////////////////////////////////////////////////// */

#ifdef __cplusplus
extern "C" {
#endif

void nikdrawSetColorRGB(unsigned int r,unsigned int g,unsigned int b);
void nikdrawSetColorLong(unsigned long c);
void nikdrawWritePixel(unsigned int x,unsigned int y);
unsigned long nikdrawReadPixel(unsigned int x,unsigned int y);
void nikdrawDrawScaledLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2);
void nikdrawDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2);
void nikdrawDrawCircle(unsigned int x, unsigned int y, unsigned int r);
void nikdrawDrawOval(unsigned int x, unsigned int y, unsigned int r);
void nikdrawCopyToFrameBuffer(NIK_BITMAP *srcBitmap);
void nikdrawClearFrameBuffer(void);
void nikdrawCopyBlockToFrameBuffer(NIK_BITMAP *srcBitmap,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh);
void nikdrawClearBlockFrameBuffer(unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh);

#ifdef __cplusplus
};
#endif


/* ///////////////////////////////////////////////////////////////////////////////
//  End of drawing
/////////////////////////////////////////////////////////////////////////////// */

//#endif // NIK_NIKDRAWING


#endif // NIKDRAW_H

