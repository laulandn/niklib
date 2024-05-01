#ifndef NIKLIB_H
#define NIKLIB_H


// Some global config...
//#define NIK_3D_LINES_NOT_TRIANGLES 1
//#define NIK_3D_FORCE_NO_TEXTURES 1


#define NIK_TEST_EVERYTHING 1


#define HAS_STDINT_H


#ifdef HAS_STDINT_H
#include <stdint.h>
#endif


#ifndef BOOL
#define BOOL int
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


#include "../../more_src/nlib/nlibdbug.h"


///////////////

#ifdef NIK_SDL
//???
#define NIK_DEFINED 1
#ifndef NIK_OPENGL
#ifndef NIK_SDL_DRAWING
#define NIK_SDL_DRAWING 1
#endif
#endif
#endif

#ifdef NIK_SDL_DRAWING
//???
#define NIK_DEFINED 1
#endif

#ifdef NIK_GUILIB
#define NIK_DEFINED 1
#define NIK_USE_ALIB 1
#endif

#ifdef NIK_NLIB
#define NIK_DEFINED 1
#endif

#ifdef NIK_OPENGL
//???
#define NIK_DEFINED 1
#endif

#ifdef NIK_OPENGLES
//???
#define NIK_DEFINED 1
#endif

#ifdef NIK_GLUT
//???
#define NIK_DEFINED 1
#endif

#ifdef NIK_BITTY
#define NIK_DEFINED 1
#endif

#ifdef NIK_COCOA
#define NIK_DEFINED 1
#endif

// Default to posix
#ifndef NIK_DEFINED
#define NIK_POSIX 1
#define NIK_DEFINED 1
#error "You need to define at least one NIK_*!"
#endif


///////////////

#ifdef NIK_COCOA
#define main(a,b) myMain(a,b)
#endif


///////////////

#ifdef NIK_USE_ALIB
#include "../cpp/alib/abasics.h"
#include "../cpp/alib/abitmap.h"
#endif


///////////////

// NOTE: Original iPhone is 320x480, 0x0 denotes max possible size
#define NIK_WINDOW_WIDTH 320
#define NIK_WINDOW_HEIGHT 480
#define NIK_WINDOW_DEPTH 24


///////////////

// Opaque
#define NIK_DRAWABLE_BITMAP void
#define NIK_BITMAP void
#define NIK_PALETTE void


///////////////

/* Useful triplet breaking macros... */
/* These are for getting 8 bit colors out of a 24 bit triplet */
#define getRGBRed24(x)   ((unsigned int)((x>>16)&0xff))
#define getRGBGreen24(x) ((unsigned int)((x>>8)&0xff))
#define getRGBBlue24(x)  ((unsigned int)(x&0xff))
/* These are for getting 4 bit colors out of a 12 bit triplet */
#define getRGBRed12(x)   ((unsigned int)((x>>8)&0xf))
#define getRGBGreen12(x) ((unsigned int)((x>>4)&0xf))
#define getRGBBlue12(x)  ((unsigned int)(x&0xf))
/* These are for getting 3 bit colors out of a 9 bit triplet */
#define getRGBRed9(x)    ((unsigned int)((x>>6)&0x7))
#define getRGBGreen9(x)  ((unsigned int)((x>>3)&0x7))
#define getRGBBlue9(x)   ((unsigned int)(x&0x7))
/* Build 24 bit triplet from 8 bit colors */
#define makeRGB24(x,y,z) ((z&0xff)+((y&0xff)<<8)+((x&0xff)<<16))
/* Build 12 bit triplet from 4 bit colors */
#define makeRGB12(x,y,z) ((z&0xf)+((y&0xf)<<4)+((x&0xf)<<8))
/* Build 9 bit triplet from 3 bit colors */
#define makeRGB9(x,y,z) ((z&0x7)+((y&0x7)<<3)+((x&0x7)<<6))


///////////////  Globals

extern BOOL nikAppInited;

extern unsigned int nikActualWidth;
extern unsigned int nikActualHeight;
extern unsigned int nikActualDepth;

extern unsigned int nikTheFontWidth;
extern unsigned int nikTheFontHeight;


// For icons, bitmaps, fonts, etc etc.
extern char *nikResourceFolder;

#ifdef __APPLE__
#if (TARGET_OS_MAC && !(TARGET_OS_EMBEDDED || TARGET_OS_IPHONE))
#define NIK_DEFAULT_RESOURCEFOLDER "/../Resources"
#else
#define NIK_DEFAULT_RESOURCEFOLDER "/Users/nik/nfiles"
#endif
#else
#define NIK_DEFAULT_RESOURCEFOLDER "/home/nik/nfiles"
#endif // __APPLE__


///////////////  Functions

#ifdef __cplusplus
extern "C" {
#endif


BOOL nikAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);

void nikLockDisplay();
void nikUnlockDisplay();
void nikFlipDisplay();

void nikSetColorRGB(unsigned int r,unsigned int g,unsigned int b);
void nikSetColorLong(unsigned long c);

void nikClearFrameBuffer();
void nikWritePixel(unsigned int x,unsigned int y);
unsigned long nikReadPixel(unsigned int x,unsigned int y);
void nikDrawScaledLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2);
void nikDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2);
void nikClearBlock(unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh);

void nikCopyBlock(NIK_BITMAP *tbm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh);

void nikDrawString(const char *st,unsigned int x,unsigned int y);

void nikRunEventLoop();
void nikExitFromRunLoop();  // NOTE: Don't count on it immediately or ever...
void nikNotifyOfResize(int w, int h);

// NOTE: These are identical to GLUT's...for now...
void nikKeyboardFunc( void (* callback)( unsigned char, int, int ) );
void nikReshapeFunc( void (* callback)( int, int ) );  // NOTE: aka resize
void nikDisplayFunc( void (* callback)( void ) );  // NOTE: aka redraw
void nikMouseFunc( void (* callback)( int, int, int, int ) );
void nikMotionFunc( void (* callback)( int, int ) );
void nikJoyMotionFunc( void (* callback)( int, int ) );  // NOTE: ???
void nikJoyButtonFunc( void (* callback)( int, int ) );  // NOTE: ???
void nikSpecialFunc( void (* callback)( int, int, int ) );  // NOTE: ???
void nikIdleFunc( void (* callback)( void ) );  // NOTE: DO NOT ASSUME YOU'LL EVER GET AN IDLE EVENT!!!!


///////////////

// KLUDGE!
#ifdef NIK_USE_ALIB
NIK_BITMAP *nikCreateBitmapFromPrivate(ABitmap *theABitmap);
#endif // NIK_USE_ALIB
#ifdef NIK_NLIB
NIK_BITMAP *nikCreateBitmapFromPrivate(struct n_image *theImage);
#endif // NIK_NLIB


///////////////  Globals (C++ only) ?!?

extern NIK_BITMAP *nikBackBuffer;  // NOTE: Not necessarily valid
extern NIK_BITMAP *nikTheFont;  // NOTE: May be NULL


///////////////  Functions (C++ only) ?!?

//NIK_PALETTE *nikGetCurPalette();

BOOL nikMatchDisplayToBitmapIfPossible(NIK_DRAWABLE_BITMAP *bm);

NIK_DRAWABLE_BITMAP *nikGetDrawableBitmap(NIK_BITMAP *bm);
BOOL nikPrepareBitmap(NIK_BITMAP *bm);
void nikCopyToDisplay(NIK_DRAWABLE_BITMAP *bm);
void nikDrawImage(NIK_DRAWABLE_BITMAP *bm,unsigned int x,unsigned int y);
void nikCopyBlock(NIK_DRAWABLE_BITMAP *bm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh);
NIK_DRAWABLE_BITMAP *nikLoadImage(char *name);


#ifdef __cplusplus
};
#endif


#endif // NIKLIB_H

