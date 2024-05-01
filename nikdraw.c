
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ND_TEST_EVERYTHING 1


//#ifdef NIK_NIKDRAWING


#include "nikdraw.h"


#include <math.h>


/* Foreground color, i.e. what nWritePixel will write */
unsigned int nikdrawColorRed=0;
unsigned int nikdrawColorGreen=0;
unsigned int nikdrawColorBlue=0;
unsigned long nikdrawColorLong=0;

unsigned int nikdrawWidth=0;
unsigned int nikdrawHeight=0;
unsigned int nikdrawDepth=0;

unsigned int nikdrawBytesPerPixel=0;
unsigned int nikdrawThePixelsSize=0;

unsigned char *nikdrawThePixels=NULL;


/* ///////////////////////////////////////////////////////////////////////////////
//  Start of drawing
/////////////////////////////////////////////////////////////////////////////// */


void nikdrawSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
  nikdrawColorLong=((r<<16)&0xff0000)+((b<<8)&0xff00)+(g&0xff);
  nikdrawColorRed=r;
  nikdrawColorGreen=g;
  nikdrawColorBlue=b;
}


void nikdrawSetColorLong(unsigned long c)
{
  nikdrawColorLong=c;
  nikdrawColorRed=nikdrawGetRGBRed24(c);
  nikdrawColorGreen=nikdrawGetRGBGreen24(c);
  nikdrawColorBlue=nikdrawGetRGBBlue24(c);
}


// NOTE: 24 bit pixel format is assumed...bad!
void nikdrawWritePixel(unsigned int x,unsigned int y)
{
  unsigned int minBlit=1;
  unsigned int maxBlit=nikdrawBytesPerPixel;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned int offset=tx+(y*(nikdrawBytesPerPixel*nikdrawWidth));
#ifdef ND_TEST_EVERYTHING
  if(!nikdrawThePixels) { debug_msg("nikdraw no pixels!"); debug_nl(); exit(EXIT_FAILURE); }
  if(x>(unsigned int)nikdrawWidth) x=nikdrawWidth-1;
  if(y>(unsigned int)nikdrawHeight) y=nikdrawHeight-1;
  if(offset>nikdrawThePixelsSize) { return; }
#endif
  unsigned char *dest=nikdrawThePixels+offset;
  dest[2]=nikdrawColorRed;
  dest[1]=nikdrawColorGreen;
  dest[0]=nikdrawColorBlue;
}


// NOTE: 24 bit pixel format is assumed...bad!
unsigned long nikdrawReadPixel(unsigned int x,unsigned int y)
{
  unsigned int minBlit=1;
  unsigned int maxBlit=nikdrawBytesPerPixel;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned int offset=tx+(y*(nikdrawBytesPerPixel*nikdrawWidth));
#ifdef ND_TEST_EVERYTHING
  if(!nikdrawThePixels) { debug_msg("nikdraw no pixels!"); debug_nl(); exit(EXIT_FAILURE); }
  if(x>(unsigned int)nikdrawWidth) x=nikdrawWidth-1;
  if(y>(unsigned int)nikdrawHeight) y=nikdrawHeight-1;
  if(offset>nikdrawThePixelsSize) { return 0; }
#endif
  unsigned char *src=nikdrawThePixels+offset;
  return nikdrawMakeRGB24(src[2],src[1],src[0]);
}


void nikdrawDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  int x1=xu1, x2=xu2, y1=yu1, y2=yu2;
  unsigned int x,y;
  int deltax,deltay,temp,err,i,swap,s1,s2;
  x=x1;  y=y1;
/* FIXME: The unsigned coords might mess the next 4 lines up a little... */
  deltax=abs(x2-x1);
  deltay=abs(y2-y1);
  if((x2-x1)<0) s1= -1; else s1=1;
  if((y2-y1)<0) s2= -1; else s2=1;
  if(deltay>deltax) {
    temp=deltax;  deltax=deltay;  deltay=temp;  swap=1;
  }
  else swap=0;
  err=2*deltay-deltax;
  for(i=1;i<=deltax;i++) {
    nikdrawWritePixel(x,y);
    while(err>=0) {
      if(swap) x+=s1; else y+=s2;
      err-=2*deltax;
    }
    if(swap) y+=s2; else x+=s1;
    err+=2*deltay;
  }
}


void nikdrawDrawCircle(unsigned int x, unsigned int y, unsigned int r)
{
	unsigned int i, j, k, rs, lj;
	rs = (lj = r)*r;
	for(i=0; i <= r; ++i) {
		j = k = sqrt(rs - (i*i));
		do {
			nikdrawWritePixel(x+i, y+j);
			nikdrawWritePixel(x+i, y-j);
			nikdrawWritePixel(x-i, y+j);
			nikdrawWritePixel(x-i, y-j);
		}
		while(++j < lj);
		lj = k;
	}
}


// TODO: Really just a circle for now...
void nikdrawDrawOval(unsigned int x, unsigned int y, unsigned int r)
{
	unsigned int i, j, k, rs, lj;
	rs = (lj = r)*r;
	for(i=0; i <= r; ++i) {
		j = k = sqrt(rs - (i*i));
		do {
			nikdrawWritePixel(x+i, y+j);
			nikdrawWritePixel(x+i, y-j);
			nikdrawWritePixel(x-i, y+j);
			nikdrawWritePixel(x-i, y-j);
		}
		while(++j < lj);
		lj = k;
	}
}


/* WARNING!  This only copies a full frame! */
void nikdrawCopyToFrameBuffer(NIK_BITMAP *srcBitmap)
{
#ifdef ND_TEST_EVERYTHING
  if(!nikdrawThePixels) { debug_msg("nikdraw no pixels!"); debug_nl(); exit(EXIT_FAILURE); }
  if(!srcBitmap) { debug_msg("nikdraw no src!"); debug_nl(); exit(EXIT_FAILURE); }
#endif
  unsigned int copySize=nikdrawBytesPerPixel*nikdrawWidth*nikdrawHeight;
  unsigned char *isrc=srcBitmap;
  unsigned char *idest=(unsigned char *)nikdrawThePixels;
  memcpy(idest,isrc,copySize);
}


void nikdrawClearFrameBuffer(void)
{
#ifdef ND_TEST_EVERYTHING
  if(!nikdrawThePixels) { debug_msg("nikdraw no pixels!"); debug_nl(); exit(EXIT_FAILURE); }
#endif
  unsigned int copySize=nikdrawBytesPerPixel*nikdrawWidth*nikdrawHeight;
  unsigned char *idest=(unsigned char *)nikdrawThePixels;
  memset(idest,0,copySize);
}


/* WARNING: Source size and format MUST match destination! */
void nikdrawCopyBlockToFrameBuffer(NIK_BITMAP *srcBitmap,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
#ifdef ND_TEST_EVERYTHING
  if(!nikdrawThePixels) { debug_msg("nikdraw no pixels!"); debug_nl(); exit(EXIT_FAILURE); }
  if(!srcBitmap) { debug_msg("nikdraw no src!"); debug_nl(); exit(EXIT_FAILURE); }
/* unsigned int dw=bm->getWidth(); */
/* if(dw>nActualWidth) dw=nActualWidth; */
/* unsigned int dh=bm->getHeight(); */
/* if(dh>nActualHeight) dh=nActualHeight; */
#endif
  unsigned int minBlit=1;
  unsigned int maxBlit=nikdrawBytesPerPixel;
  unsigned int smaxBlit=maxBlit;
  unsigned int tx=(dx/minBlit)*maxBlit;
  unsigned int dbpl=nikdrawBytesPerPixel*nikdrawWidth;
  unsigned int sdw=smaxBlit*bw;
  unsigned int sbpl=dbpl;
  unsigned char *isrc=srcBitmap+(sbpl*sy)+(smaxBlit*sx);
  unsigned char *idest=nikdrawThePixels+tx+(dy*dbpl);
  unsigned int cy=0;
  for(cy=0;cy<bh;cy++) {
    isrc+=sbpl;
    idest+=dbpl;
    memcpy(idest,isrc,sdw);
  }
}


void nikdrawClearBlockFrameBuffer(unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
#ifdef ND_TEST_EVERYTHING
/* unsigned int dw=bm->getWidth(); */
/* if(dw>nActualWidth) dw=nActualWidth; */
/* unsigned int dh=bm->getHeight(); */
/* if(dh>nActualHeight) dh=nActualHeight; */
#endif
  unsigned int minBlit=1;
  unsigned int maxBlit=nikdrawBytesPerPixel;
  unsigned int tx=(dx/minBlit)*maxBlit;
  unsigned int dbpl=nikdrawBytesPerPixel*nikdrawWidth;
  unsigned char *idest=nikdrawThePixels+tx+(dy*dbpl);
  unsigned int cy=0;
  for(cy=0;cy<bh;cy++) {
    idest+=dbpl;
    memset(idest,0,dbpl);
  }
}


/* ///////////////////////////////////////////////////////////////////////////////
//  End of drawing
/////////////////////////////////////////////////////////////////////////////// */


//#endif // NIK_NIKDRAWING

