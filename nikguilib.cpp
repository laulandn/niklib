

#ifdef NIK_GUILIB


//#define DEBUG_OUT std::cerr
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT dBug
#define ERR_OUT std::cerr
//#define DEBUG_OUT *aNullStream
#define CONS_OUT std::cout


/*

WARNING WARNING WARNING

This code has lots of java-esqe code that needs to be removed/replaced!

*/


#include <cstdlib>
#include <iostream>
#include <unistd.h>


#include "niklib.h"


#ifdef __APPLE__
//#include <TargetConditionals.h>
#endif // __APPLE__


#include "../cpp/alib/apalette.h"
#include "../cpp/alib/abasics.h"

#include "../cpp/guilib/AApp.h"
#include "../cpp/guilib/ADisplay.h"
#include "../cpp/guilib/AWindow.h"
#include "../cpp/guilib/AEvent.h"
#include "../cpp/guilib/AApplicationLoop.h"
#include "../cpp/guilib/APrivateBitmap.h"


BOOL nikAppInited=FALSE;
//
unsigned int nikActualWidth=0;
unsigned int nikActualHeight=0;
unsigned int nikColorRed=0;
unsigned int nikColorGreen=0;
unsigned int nikColorBlue=0;
unsigned long nikColorLong=0;
//
char *nikResourceFolder=NULL;
//
NIK_BITMAP *nikBackBuffer=NULL;
//
NIK_BITMAP *nikTheFont=NULL;
unsigned int nikTheFontWidth=8;
unsigned int nikTheFontHeight=8;

AApplicationLoop *appLoop=NULL;


void (* nikTheKeyboardFunc)( unsigned char, int, int )=NULL;
void (* nikTheReshapeFunc)( int, int )=NULL;
void (* nikTheDisplayFunc)( void )=NULL;
void (* nikTheMouseFunc)( int, int, int, int )=NULL;
void (* nikTheMotionFunc)( int, int )=NULL;
void (* nikTheJoyMotionFunc)( int, int )=NULL;
void (* nikTheJoyButtonFunc)( int, int )=NULL;
void (* nikTheSpecialFunc)( int, int, int )=NULL;
void (* nikTheIdleFunc)( void )=NULL;


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

BOOL nikAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  debug_init();
  debug_msg("nikAppInit(...)\n");
  //system("pwd");
  for(int t=0;t<argc;t++) {
    debug_msg("argv["); debug_int(t); debug_msg("]="); debug_quotes(argv[t]); debug_nl();
  }
  nikResourceFolder=(char *)malloc(256);
  DSHELPER_alib.fillInString(nikResourceFolder,NIK_DEFAULT_RESOURCEFOLDER);
  debug_msg("nikResourceFolder is: "); debug_quotes(nikResourceFolder); debug_nl();
  chdir(nikResourceFolder);
/*
#ifdef __APPLE__
  nikResourceFolder=(char *)malloc(256);
  DSHELPER_alib.getPathFromFullName(nikResourceFolder,argv[0]);
#if (TARGET_OS_MAC && !(TARGET_OS_EMBEDDED || TARGET_OS_IPHONE))
  DSHELPER_alib.fillInString(nikResourceFolder,"/../Resources");
#endif
  DEBUG_OUT<<"nikResourceFolder is: "); nikResourceFolder DEBUG_OUT<<"\n";
  chdir(nikResourceFolder);
#endif // __APPLE__
*/
  DEBUG_OUT<<"About to call start...\n";
  aThisApp->start(argc,argv);
  DEBUG_OUT<<"returned from start, creating display...\n";
  ADisplay *d=new ADisplay(theWidth,theHeight,theDepth);
  DEBUG_OUT<<"creating window...\n";
  AWindow *w=new AWindow(d,argv[0],theWidth,theHeight);
  aDefaultAWindow=w;
  nikActualWidth=w->getWidth();
  nikActualHeight=w->getHeight();
  debug_msg("AWindow is "); debug_xy(nikActualWidth,nikActualHeight); debug_nl();
  appLoop=new AApplicationLoop(w);
  //
  nikAppInited=TRUE;
  debug_msg("nikAppInit done.\n");
  return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////


// Get rid of this...
/*
void nikRegisterEventHandlers()
{
  DEBUG_OUT<<"nikRegisterEventHandlers(...)\n";
  appLoop->regEvent(A_EVENT_REDRAW,JAppletRedrawGuilib);
  appLoop->regEvent(A_EVENT_RESIZE,JAppletResizeGuilib);
  appLoop->regEvent(A_EVENT_DESTROY,JAppletDestroyGuilib);
  appLoop->regEvent(A_EVENT_BREAK,JAppletDestroyGuilib);
  appLoop->regEvent(A_EVENT_MOUSEDOWN,JnikvmAppletMousedownGuilib);
  appLoop->regEvent(A_EVENT_MOUSEUP,JnikvmAppletMouseupGuilib);
  appLoop->regEvent(A_EVENT_MOUSEMOVE,JnikvmAppletMousemoveGuilib);
  appLoop->regEvent(A_EVENT_KEYDOWN,JnikvmAppletKeydownGuilib);
  appLoop->regEvent(A_EVENT_KEYUP,JnikvmAppletKeyupGuilib);
}
*/


void nikRunEventLoop()
{
  DEBUG_OUT<<"nikRunEventLoop(...)\n";
  DEBUG_OUT<<"Going to apploop.run...\n";
  appLoop->run();
  DEBUG_OUT<<"Back from apploop.run.\n";
}


void nikExitFromRunLoop()
{
  //nikExitNextLoop=TRUE;
  DEBUG_OUT<<"nikExitFromRunLoop not implemented!\n";
  exit(EXIT_FAILURE);
 }


void nikKeyboardFunc( void (* callback)( unsigned char, int, int ) )
{
  DEBUG_OUT<<"nikKeyboardFunc called...\n";
  nikTheKeyboardFunc=callback;
  if(appLoop) appLoop->regEventKeyboard(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
}


void nikReshapeFunc( void (* callback)( int, int ) )
{
  DEBUG_OUT<<"nikReshapeFunc called...\n";
  nikTheReshapeFunc=callback;
  if(appLoop) appLoop->regEventResize(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
}


// NOTE: Should we set thsis handler?
void nikDisplayFunc( void (* callback)( void ) )
{
  DEBUG_OUT<<"nikDisplayFunc called...\n";
  nikTheDisplayFunc=callback;
  if(appLoop) appLoop->regEventRedraw(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
}


void nikMouseFunc( void (* callback)( int, int, int, int ) )
{
  DEBUG_OUT<<"nikMouseFunc called...\n";
  nikTheMouseFunc=callback;
  if(appLoop) appLoop->regEventMouse(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
}


void nikMotionFunc( void (* callback)( int, int ) )
{
  DEBUG_OUT<<"nikMotionFunc called...\n";
  nikTheMotionFunc=callback;
  if(appLoop) appLoop->regEventMotion(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
}


// Because AApplicationLoop doesn't handle this sort of event
void nikJoyButtonFunc( void (* callback)( int, int ) )
{
  /*
  DEBUG_OUT<<"nikJoyButtonFunc called...\n";
  nikTheJoyButtonFunc=callback;
  if(appLoop) appLoop->regEventMouse(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
*/
  DEBUG_OUT<<"Warning! nikJoyButtonFunc ignored, appLoop doesn't handle these!\n";
}


// Because AApplicationLoop doesn't handle this sort of event
void nikJoyMotionFunc( void (* callback)( int, int ) )
{
/*
  DEBUG_OUT<<"nicJoykMotionFunc called...\n";
  nikTheJoyMotionFunc=callback;
  if(appLoop) appLoop->regEventMotion(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
*/
  DEBUG_OUT<<"Warning! nikJoyMotionFunc ignored, appLoop doesn't handle these!\n";
}


// Because AApplicationLoop doesn't handle this sort of event
void nikSpecialFunc( void (* callback)( int, int, int ) )
{
/*
  DEBUG_OUT<<"nikSpecialFunc called...\n";
  nikTheSpecialFunc=callback;
  if(appLoop) appLoop->regEventResize(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
*/
  DEBUG_OUT<<"Warning! nikSpecialFunc ignored, appLoop doesn't handle these!\n";
}


// Because AApplicationLoop doesn't handle this sort of event
void nikIdleFunc( void (* callback)( void ) )
{
/*
  DEBUG_OUT<<"nikIdleFunc called...\n";
  nikTheIdleFunc=callback;
  if(appLoop) appLoop->regEventResize(callback);
  else {
    DEBUG_OUT<<"No appLoop! Can't set event callback!";
    exit(EXIT_SUCCESS);
  }
*/
  DEBUG_OUT<<"Warning! nikIdleFunc ignored, appLoop doesn't handle these!\n";
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

void nikNotifyOfResize(int w, int h)
{
  debug_msg("nikNotifyOfResize("); debug_int(w); debug_msg("x"); debug_int(h); debug_msg(")\n");
  nikActualWidth=w;  nikActualHeight=h;
}


BOOL nikMatchDisplayToBitmapIfPossible(NIK_DRAWABLE_BITMAP *bm)
{
  //if(aDefaultAWindow) aDefaultAWindow->resizeWindow(bm->getWidth(),bm->getHeight());
  return FALSE;
}


void nikWritePixel(unsigned int x,unsigned int y)
{
  aDefaultAWindow->writePixel(x,y);
}


unsigned long nikReadPixel(unsigned int x,unsigned int y)
{
  return aDefaultAWindow->readPixel(x,y);
}


void nikDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  aDefaultAWindow->drawLine(xu1,yu1,xu2,yu2);
}


NIK_DRAWABLE_BITMAP *nikGetDrawableBitmap(NIK_BITMAP *bm)
{
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return NULL; }
  return (NIK_DRAWABLE_BITMAP *)(new APrivateBitmap((ABitmap *)bm));
}


void nikDrawImage(NIK_DRAWABLE_BITMAP *bm,unsigned int x,unsigned int y)
{
  aDefaultAWindow->drawBitmap((APrivateBitmap *)bm,x,y);
}


void nikCopyBlock(NIK_DRAWABLE_BITMAP *bm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
  aDefaultAWindow->copyBlock((APrivateBitmap *)bm,sx,sy,dx,dy,bw,bh);
}


void nikCopyToFrameBuffer(NIK_DRAWABLE_BITMAP *tbm)
{
  /*
  unsigned int copySize=niksdl_theSurface->format->BytesPerPixel*niksdl_theSurface->w*niksdl_theSurface->h;
  unsigned char *isrc=((NIK_BITMAP *)tbm)->getPlane(0);
  unsigned char *idest=(unsigned char *)niksdl_theSurface->pixels;
  memcpy(idest,isrc,copySize);
  */
  DEBUG_OUT<<"nikCopyToFrameBuffer not implemented!\n";
  exit(EXIT_FAILURE);
}


void nikDrawString(char *st,unsigned int x,unsigned int y)
{
  DEBUG_OUT<<"nikDrawString not implemented!\n";
  exit(EXIT_FAILURE);
}


BOOL nikLoadFont(APrivateBitmap *bm,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh)
{
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return FALSE; }
  DEBUG_OUT<<"nikLoadFont not implemented!\n";\
  return FALSE;
}


BOOL nikPrepareBitmap(NIK_BITMAP *bm)
{
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return false; }
  ((ABitmap *)bm)->reorderRGBChunky2BGR();
  return TRUE;
}


/*
NIK_PALETTE *nikGetCurPalette()
{
  return aDefaultAWindow->getParent()->getColors();
}
*/


void nikClearFrameBuffer()
{
  aDefaultAWindow->setBackground(nikColorLong,true);
  aDefaultAWindow->clearWindow();
}


void nikLockDisplay()
{
  //DEBUG_OUT<<"nikLockDisplay not implemented!\n";
}


void nikUnlockDisplay()
{
  //DEBUG_OUT<<"nikUnlockDisplay not implemented!\n";
}


void nikFlipDisplay()
{
  //DEBUG_OUT<<"nikFlipDisplay not implemented!\n";
}


void nikSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
  nikColorLong=((r<<16)&0xff0000)+((b<<8)&0xff00)+(g&0xff);
  nikColorRed=r;
  nikColorGreen=g;
  nikColorBlue=b;
  //DEBUG_OUT<<"setForeground("); nikColorLong DEBUG_OUT<<",true)\n";
  aDefaultAWindow->setForeground(nikColorLong,true);
}


void nikSetColorLong(unsigned long c)
{
  nikColorLong=c;
  nikColorRed=getRGBRed24(c);
  nikColorGreen=getRGBGreen24(c);
  nikColorBlue=getRGBBlue24(c);
  //DEBUG_OUT<<"setForeground("); nikColorLong DEBUG_OUT<<",true)\n";
  aDefaultAWindow->setForeground(nikColorLong,true);
}


// A Kludge...if we're implementing niklib using guilib, NIK_BITMAP is already an ABitmap
NIK_BITMAP *nikCreateBitmapFromPrivate(ABitmap *theABitmap)
{
  DEBUG_OUT<<"nikCreateBitmapFromPrivate kludge...\n";
  NIK_BITMAP *bm=NULL;
  bm=(NIK_BITMAP *)theABitmap;
  return bm;
}


#endif // NIK_GUILIB
