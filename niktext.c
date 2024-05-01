

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//#ifdef NIK_SDL_DRAWING


#include "nikdraw.h"

#include "niktext.h"


#ifndef BOOL
#define BOOL int
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


char *niktextFont=NULL;

unsigned int niktextNumWide=16;
unsigned int niktextNumHigh=16;

unsigned int niktextFontWidth=8;
unsigned int niktextFontHeight=8;


/* ///////////////////////////////////////////////////////////////////////////////
//  Start of drawing
/////////////////////////////////////////////////////////////////////////////// */


void niktextDrawString(const char *st,unsigned int x,unsigned int y)
{
  if(!st) return;
  /*
  if(niktextFont) {
    unsigned int sx=0,sy=0;
    char c;
    unsigned int t=0;
    for(t=0;t<strlen(st);t++) {
      c=st[t];
      // These may be backwards...
      sy=(c/niktextNumHigh)*niktextFontHeight;
      sx=(c%niktextNumWide)*niktextFontWidth;
      //nikCopyBlock(niktextFont,sx,sy,x,y,niktextFontWidth,niktextFontHeight);
      */
      debug_msg("niktextDrawString not implemented!\n");
      exit(EXIT_FAILURE);
      /*
    }
  }
  else {
    exit(EXIT_FAILURE);
  }
  */
}


int niktextLoadFont(const char *theName,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh)
{
  debug_msg("niktextLoadFont("); debug_quotes(theName); debug_msg(",...)...\n");
  int retVal=0;
  if(!theName) {
    debug_msg("no fontname supplied!\n");
    return retVal;
  }
  /*
  AImageDecoder *dec=NULL;
  (AImageDecoder *)ASmartDecoder::pickBestDecoder(theName);
  */
  if(retVal) {
  //if(!dec) {
    debug_msg("Couldn't create a decoder for that file!\n");
    return retVal;
  }
  //nikTheFont=dec->createNewBitmapObject();
  if(retVal) {
  //if(!nikTheFont) {
    debug_msg("Couldn't create a bitmap for that font!\n");
    return retVal;
  }
  //nikTheFont->convert(24,A_BITMAP_CHUNKY,NULL);
  return retVal;
}


/* ///////////////////////////////////////////////////////////////////////////////
//  End of drawing
/////////////////////////////////////////////////////////////////////////////// */


//#endif


