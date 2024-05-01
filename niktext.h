#ifndef NIKTEXT_H
#define NIKTEXT_H


//#ifdef NIK_NIKDRAWING


#define NT_TEST_EVERYTHING 1


#include "../../more_src/nlib/nlibdbug.h"


/* //////////////////////////////////////////////////////////////////////////////
//  Start of globals
/////////////////////////////////////////////////////////////////////////////// */

/*
WARNING! WARNING! WARNING!

Don't assume ANY of these are valid or useful or implemented!
*/


extern char *niktextFont;

extern unsigned int niktextNumWide;
extern unsigned int niktextNumHigh;

extern unsigned int niktextFontWidth;
extern unsigned int niktextFontHeight;


/* //////////////////////////////////////////////////////////////////////////////
//  Start of drawing
/////////////////////////////////////////////////////////////////////////////// */

#ifdef __cplusplus
extern "C" {
#endif


void niktextDrawString(const char *st,unsigned int x,unsigned int y);
int niktextLoadFont(const char *theName,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh);


#ifdef __cplusplus
};
#endif


/* ///////////////////////////////////////////////////////////////////////////////
//  End of drawing
/////////////////////////////////////////////////////////////////////////////// */

//#endif // NIK_NIKDRAWING


#endif // NIKTEXT_H

