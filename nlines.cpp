
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT dBug
#define ERR_OUT std::cerr
//#define DEBUG_OUT *aNullStream
#define CONS_OUT std::cout


#include <stdio.h>
#include <stdlib.h>


//#define DEBUG_VERBOSE 1


// NOTE: Original iPhone is 320x480, 0x0 denotes max possible size
#define NLINES_WINDOW_WIDTH 320
#define NLINES_WINDOW_HEIGHT 480
#define NLINES_WINDOW_DEPTH 24


#include "niklib.h"


// Forward...
void resizeLines(int w, int h);
void keyboardLines(unsigned char key, int x, int y);
void specialLines(int key, int x, int y);
void mouseLines(int button, int state, int x, int y);
void motionLines(int x, int y);
void drawLines();


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

void resizeLines(int w, int h)
{
#ifdef DEBUG_VERBOSE
  debug_msg("resizeLines!\n");
#endif
  nikNotifyOfResize(w,h);
  drawLines();
}


void keyboardLines(unsigned char key, int x, int y)
{
#ifdef DEBUG_VERBOSE
  debug_msg("keyboardLines!\n");
#endif
  drawLines();
}


void specialLines(int key, int x, int y)
{
#ifdef DEBUG_VERBOSE
  debug_msg("specialLines!\n");
#endif
  drawLines();
}


void mouseLines(int button, int state, int x, int y)
{
#ifdef DEBUG_VERBOSE
  debug_msg("mouseLines!\n");
#endif
  drawLines();
}


void motionLines(int x, int y)
{
#ifdef DEBUG_VERBOSE
  debug_msg("motionLines!\n");
#endif
  drawLines();
}


void drawLines()
{
#ifdef DEBUG_VERBOSE
  debug_msg("drawLines "); debug_xy(nikActualWidth,nikActualHeight); debug_msg("!\n");
#endif
  unsigned long t;
  unsigned long numColors=0x00ffffff;
  unsigned long inc=1;
  BOOL trueColor=TRUE;
  /*
  NIK_PALETTE *colors=nikGetCurPalette();
  // If we don't have a palette, we assume it's truecolor
  if(colors) {
    //if(colors->getNumColors()>256) { trueColor=TRUE; }
    //numColors=colors->getNumColors();
  }
  else trueColor=TRUE;
  */
  if(trueColor) inc=numColors/nikActualWidth;
#ifdef DEBUG_VERBOSE
  debug_msg("trueColor="); debug_int(trueColor); debug_msg(" numColors="); debug_long(numColors); debug_msg("\n");
#endif
  unsigned long c=0;
  nikLockDisplay();
  nikSetColorLong(0x000000);
  //nikClearDisplay();
  c=0;
  for(t=0;t<nikActualWidth-1;t++) {
    c=c+inc;
    if(c>numColors) c=0;
    nikSetColorLong(c);
    nikDrawLine(t,0,nikActualWidth-t-1,nikActualHeight-1);
  }
  c=0;
  if(trueColor) inc=numColors/nikActualHeight;
  for(t=nikActualHeight-1;t>0;t--) {
    c=c+inc;
    if(c>numColors) c=0;
    nikSetColorLong(c);
    nikDrawLine(0,t,nikActualWidth-1,nikActualHeight-t-1);
  }
  nikUnlockDisplay();
  nikFlipDisplay();
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv)
{
  debug_msg("Going to nikAppInit...\n");
  BOOL ok=nikAppInit(NLINES_WINDOW_WIDTH,NLINES_WINDOW_HEIGHT,NLINES_WINDOW_DEPTH,argc,argv);
  if(!ok) { debug_msg("nikAppInit failed!\n");exit(EXIT_FAILURE); }
  debug_msg("Back from nikAppInit.\n");
  if((!nikActualWidth)||(!nikActualHeight)) { debug_msg("Unable to open suitible display!\n");exit(EXIT_FAILURE); }
  //
  debug_msg("Setting event handlers...\n");
  nikDisplayFunc(drawLines);
  nikReshapeFunc(resizeLines);
  nikMouseFunc(mouseLines);
  nikMotionFunc(motionLines);
  nikKeyboardFunc(keyboardLines);
  nikSpecialFunc(specialLines);
  debug_msg("Done setting event handlers.\n");
  //
  debug_msg("Drawing once...\n");
  drawLines();
  //
  debug_msg("Entering event loop...\n");
  nikRunEventLoop();
  //
#ifdef DEBUG_VERBOSE
  debug_msg("End of main().\n");
#endif
  return 0;
}
