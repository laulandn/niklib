

#ifdef NIK_NLIB


#define DEBUG_OUT std::cerr
#define ERR_OUT std::cerr
//#define DEBUG_OUT *aNullStream


/*

WARNING WARNING WARNING

This code has lots of java-esqe code that needs to be removed/replaced!

*/


#include <cstdlib>
#include <iostream>
#include <unistd.h>


#include "niklib.h"


extern "C" {
#include "../../more_src/nlib/nlib.h"
};


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


struct n_display *theDisplay=NULL;
struct n_window *theWin=0;
struct n_event *theEvent=0;


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
  //DSHELPER_alib.fillInString(nikResourceFolder,NIK_DEFAULT_RESOURCEFOLDER);
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
  DEBUG_OUT<<"About to call open_nlibs...\n";
  open_nlibs(&argc,&argv);
  DEBUG_OUT<<"Calling open_window...\n";
  theWin=open_window(NULL,argv[0],theWidth,theHeight);
  theDisplay=theWin->parent;
  nikActualWidth=theWin->width;
  nikActualHeight=theWin->height;
  debug_msg("AWindow is "); debug_xy(nikActualWidth,nikActualHeight); debug_nl();
  //
  nikAppInited=TRUE;
  debug_msg("nikAppInit done.\n");
  return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

void nikRunEventLoop()
{
  DEBUG_OUT<<"nikRunEventLoop(...)\n";
  debug_msg("about to loop...\n");
  BOOL waiting=TRUE;
  while(waiting) {
    theEvent=wait_event(theWin);
    if(theEvent->type!=N_EVENT_NONE) dump_event(theEvent);
    if(theEvent->type&N_EVENT_REDRAW) nikTheDisplayFunc();
    if(theEvent->type&N_EVENT_RESIZE) nikTheDisplayFunc();
    if(theEvent->type&N_EVENT_BREAK) waiting=FALSE;
    if(theEvent->type&N_EVENT_DESTROY) waiting=FALSE;
  }
  debug_msg("done looping.\n");
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
  //if(appLoop) appLoop->regEventKeyboard(callback);
  //else {
  //  DEBUG_OUT<<"No appLoop! Can't set event callback!";
  //  exit(EXIT_SUCCESS);
  //}
}


void nikReshapeFunc( void (* callback)( int, int ) )
{
  DEBUG_OUT<<"nikReshapeFunc called...\n";
  nikTheReshapeFunc=callback;
  //if(appLoop) appLoop->regEventResize(callback);
  //else {
  //  DEBUG_OUT<<"No appLoop! Can't set event callback!";
  //  exit(EXIT_SUCCESS);
  //}
}


// NOTE: Should we set thsis handler?
void nikDisplayFunc( void (* callback)( void ) )
{
  DEBUG_OUT<<"nikDisplayFunc called...\n";
  nikTheDisplayFunc=callback;
  //if(appLoop) appLoop->regEventRedraw(callback);
  //else {
  //  DEBUG_OUT<<"No appLoop! Can't set event callback!";
  //  exit(EXIT_SUCCESS);
  //}
}


void nikMouseFunc( void (* callback)( int, int, int, int ) )
{
  DEBUG_OUT<<"nikMouseFunc called...\n";
  nikTheMouseFunc=callback;
  //if(appLoop) appLoop->regEventMouse(callback);
  //else {
  //  DEBUG_OUT<<"No appLoop! Can't set event callback!";
  //  exit(EXIT_SUCCESS);
  //}
}


void nikMotionFunc( void (* callback)( int, int ) )
{
  DEBUG_OUT<<"nikMotionFunc called...\n";
  nikTheMotionFunc=callback;
  //if(appLoop) appLoop->regEventMotion(callback);
  //else {
  //  DEBUG_OUT<<"No appLoop! Can't set event callback!";
  //  exit(EXIT_SUCCESS);
  //}
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
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return false; }
  //if(aDefaultAWindow) aDefaultAWindow->resizeWindow(bm->getWidth(),bm->getHeight());
  return FALSE;
}


void nikWritePixel(unsigned int x,unsigned int y)
{
  write_pixel(theWin,x,y);
}


unsigned long nikReadPixel(unsigned int x,unsigned int y)
{
  return read_pixelc(theWin,x,y);
}


void nikDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  draw_line(theWin,xu1,yu1,xu2,yu2);
}


NIK_DRAWABLE_BITMAP *nikGetDrawableBitmap(NIK_BITMAP *bm)
{
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return NULL; }
  // NOTE For now we just assume they're all drawable
  return bm;
}


void nikDrawImage(NIK_DRAWABLE_BITMAP *bm,unsigned int x,unsigned int y)
{
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return; }
  draw_image(theWin,(struct n_image *)bm,x,y);
}


void nikCopyBlock(NIK_DRAWABLE_BITMAP *bm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return; }
  //aDefaultAWindow->copyBlock((APrivateBitmap *)bm,sx,sy,dx,dy,bw,bh);
}


void nikCopyToFrameBuffer(NIK_DRAWABLE_BITMAP *tbm)
{
  if(!tbm) { DEBUG_OUT<<"no tbm!\n"; return; }
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
  draw_text(theWin,x,y,st);
}


/*
BOOL nikLoadFont(APrivateBitmap *bm,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh)
{
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return FALSE; }
  DEBUG_OUT<<"nikLoadFont not implemented!\n";\
  return FALSE;
}
*/


BOOL nikPrepareBitmap(NIK_BITMAP *bm)
{
  if(!bm) { DEBUG_OUT<<"no bm!\n"; return false; }
  DEBUG_OUT<<"nikPrepareBitmap not implemented!\n";
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
  clear_window(theWin);
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
  set_foregroundc(theWin,nikColorLong);
}


void nikSetColorLong(unsigned long c)
{
  nikColorLong=c;
  nikColorRed=getRGBRed24(c);
  nikColorGreen=getRGBGreen24(c);
  nikColorBlue=getRGBBlue24(c);
  set_foregroundc(theWin,c);
}


// A Kludge...if we're implementing niklib using nlib, NIK_BITMAP is already an n_image
NIK_BITMAP *nikCreateBitmapFromPrivate(n_image *theImage)
{
  DEBUG_OUT<<"nikCreateBitmapFromPrivate kludge...\n";
  NIK_BITMAP *bm=NULL;
  bm=(NIK_BITMAP *)theImage;
  return bm;
}


#endif // NIK_NLIB
