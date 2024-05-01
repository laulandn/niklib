
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT dBug
#define ERR_OUT std::cerr
//#define DEBUG_OUT *aNullStream
#define CONS_OUT std::cout


// View an image


#define NV_USE_DECODERLIB 1

#define DEBUG_VERBOSE 1


#ifdef __linux__
#define DEFAULT_FILE "/home/nick/nfiles/pcx/8/venus.pcx"
#endif // __linux__

#ifdef __APPLE__
//#define DEFAULT_FILE "/Users/nick/nfiles/pcx/8/venus.pcx"
//#define DEFAULT_FILE "/Users/nick/nfiles/pcx/24/pac24.pcx"
#define DEFAULT_FILE "/Users/nick/nfiles/iff/24/venus24.iff"
#endif // __APPLE__


// NOTE: Original iPhone is 320x480, 0x0 denotes max possible size
#define THE_WINDOW_WIDTH 320
#define THE_WINDOW_HEIGHT 480
#define THE_WINDOW_DEPTH 24


//#define NV_FIT_TO_SCREEN 1


#include <iostream>
#include <stdio.h>
#include <stdlib.h>


#include "../niklib/niklib.h"


#ifdef NV_USE_DECODERLIB
#include "../cpp/decodeimg/ASmartImageDecoder.h"
#else
extern "C" {
#include "../../more_src/nlib/nlib.h"
}
#endif


NIK_DRAWABLE_BITMAP *theDrawableBitmap=(NIK_DRAWABLE_BITMAP *)NULL;
int theArgc=0;
char **theArgv=NULL;
//
NIK_BITMAP *theBitmap=(NIK_BITMAP *)NULL;
const char *theFilename=NULL;
unsigned int x=0;
unsigned int y=0;


// Forward...
void resizeImage(int w, int h);
void keyboardImage(unsigned char key, int x, int y);
void specialImage(int key, int x, int y);
void mouseImage(int button, int state, int x, int y);
void motionImage(int x, int y);
void drawImage();


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

void resizeImage(int w, int h)
{
  DEBUG_OUT<<"resizeImage!\n";
  nikNotifyOfResize(w,h);
  drawImage();
}


void keyboardImage(unsigned char key, int x, int y)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"keyboardImage!\n";
#endif
  // TODO: Actually use the event info to do something...
  drawImage();
}


void specialImage(int key, int x, int y)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"specialImage!\n";
#endif
  // TODO: Actually use the event info to do something...
  drawImage();
}


void mouseImage(int button, int state, int x, int y)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"mouseImage!\n";
#endif
  // TODO: Actually use the event info to do something...
  drawImage();
}


void motionImage(int x, int y)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"motionImage!\n";
#endif
  // TODO: Actually use the event info to do something...
  drawImage();
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

void drawImage()
{
#ifdef DEBUG_VERBOSE
  //DEBUG_OUT<<"drawImage...\n";
#endif
  nikLockDisplay();
  unsigned int dx=(nikActualWidth/2)+(0);
  unsigned int dy=(nikActualHeight/2)+(0);
  dx=x; dy=y;  // Until we can calc the above and center the image
  nikSetColorRGB(0,0,0);
  //nikClearDisplay();
  nikDrawImage(theDrawableBitmap,dx,dy);
  nikUnlockDisplay();
  nikFlipDisplay();
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

bool handleImage(const char *fname)
{
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"handleImage('"<<fname<<"') called...\n";
#endif
  if(!fname) { DEBUG_OUT<<"no fname!\n"; return false; }
#ifdef NV_USE_DECODERLIB
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to call ASmartImageDecoder::pickBestImageDecoder...\n";
#endif
  AImageDecoder *theDecoder=ASmartImageDecoder::pickBestImageDecoder(theFilename);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"(back from pickBestImageDecoder)\n";
#endif
  //
  if(!theDecoder) {
    DEBUG_OUT<<"Couldn't recognize file type of that file!\n";
    return false;
  }
  if(theDecoder->err.getError()) {
    DEBUG_OUT<<"Problem decoding file!\n";
    return false;
  }
  //
  if(!theDecoder->isImage()) {
#ifdef DEBUG_VERBOSE
    DEBUG_OUT<<"isImage isn't true!\n";
#endif
    return false;
  }
  //
  //bool rgbReorder=theDecoder->getRGBReorder();
  //APalette *colors=theDecoder->getColors();
  //uint8_t *lineBuffer=theDecoder->getLineBuffer();
  //unsigned int lineBufferSize=theDecoder->getLineBufferSize();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to call decoder->createNewBitmapObject()...\n";
#endif
  ABitmap *theABitmap=theDecoder->createNewBitmapObject();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"(back from createNewBitmapObject)\n";
#endif
  if(!theABitmap) { DEBUG_OUT<<"no theABitmap!\n"; return false; }
  if(theABitmap->err.getError()) { DEBUG_OUT<<"theABitmap error!\n"; return false; }
  //
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"theDecoder is "<<theDecoder->getWidth()<<"x"<<theDecoder->getHeight()<<"x"<<theDecoder->getDepth()<<"x"<<theDecoder->getNPlanes()<<"\n";
  if(theDecoder->getColors()) { DEBUG_OUT<<"theDecoder has colors\n"; } else { DEBUG_OUT<<"theDecoder does not have colors\n"; }
  DEBUG_OUT<<"theABitmap is "<<theABitmap->getWidth()<<"x"<<theABitmap->getHeight()<<"x"<<theABitmap->getDepth()<<"x"<<theABitmap->getNPlanes()<<"\n";
  if(theABitmap->getColors()) { DEBUG_OUT<<"theABitmap has colors\n"; } else { DEBUG_OUT<<"theABitmap does not have colors\n"; }
#endif
  //
  //theABitmap->convertBitmap(24,(ABitmapType_alib)ALIB_BITMAP_CHUNKY,NULL);
  //
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to call nikCreateBitmapFromPrivate...\n";
#endif
  theBitmap=nikCreateBitmapFromPrivate(theABitmap);
  //
#else
  //
  uint16_t width,height;
  uint8_t depth,nPlanes;
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to call get_image_size...\n";
#endif
  get_image_size(fname,&width,&height,&depth,&nPlanes);
  /* */
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to call read_image...\n";
#endif
  struct n_image *im=read_image(NULL,fname,FALSE);
  if(!im) { DEBUG_OUT<<"no im!\n"; return false; }
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to call nikCreateBitmapFromPrivate...\n";
#endif
  theBitmap=nikCreateBitmapFromPrivate(im);
#endif
  //
  if(!theBitmap) { DEBUG_OUT<<"no theBitmap!\n"; return false; }
  theDrawableBitmap=nikGetDrawableBitmap(theBitmap);
  if(!theDrawableBitmap) { DEBUG_OUT<<"no theDrawableBitmap!\n"; return false; }
  nikPrepareBitmap(theDrawableBitmap);
  //
  //nikMatchDisplayToBitmapIfPossible(theDrawableBitmap);
  //
  return true;
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv)
{
  BOOL ok=nikAppInit(THE_WINDOW_WIDTH,THE_WINDOW_HEIGHT,THE_WINDOW_DEPTH,argc,argv);
  if(!ok) { debug_msg("nikAppInit failed!\n");exit(EXIT_FAILURE); }
  if((!nikActualWidth)||(!nikActualHeight)) {
    DEBUG_OUT<<"Unable to open suitible display!\n";
    exit(EXIT_FAILURE);
  }
  if(argc!=2) {
#ifdef DEFAULT_FILE
    theFilename=(char *)DEFAULT_FILE;
#else
    std::cout<<argv[0]<<": Usage: "<<argv[0]<<" image\n";
    exit(EXIT_FAILURE);
#endif // DEFAULT_FILE
  }
  else {
    theFilename=argv[1];
  }
  //
  if(!theFilename) {
    DEBUG_OUT<<"usage: "<<argv[0]<<" {image}\n";
    exit(EXIT_FAILURE);
  }
  //
  if(!handleImage(theFilename)) {
    DEBUG_OUT<<"handleImage() failed!\n";
    exit(EXIT_FAILURE);
  }
  //
  if(!theDrawableBitmap) {
    DEBUG_OUT<<"Didn't get theDrawableBitmap!\n";
    exit(EXIT_FAILURE);
  }
  // Center image on whatever we ended up getting
  //x=(nikActualWidth/2)-(theDrawableBitmap->getWidth()/2);
  //y=(nikActualHeight/2)-(theDrawableBitmap->getHeight()/2);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Attempting to center image: x="<<x<<" y="<<y<<"\n";
#endif
  //
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to register event handlers...\n";
#endif
  nikDisplayFunc(drawImage);
  nikReshapeFunc(resizeImage);
  nikMouseFunc(mouseImage);
  nikMotionFunc(motionImage);
  nikKeyboardFunc(keyboardImage);
  nikSpecialFunc(specialImage);
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Done registering event handlers\n";
#endif
  //
  drawImage();
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"Going to nikRunEventLoop()!\n";
#endif
  nikRunEventLoop();
  //
#ifdef DEBUG_VERBOSE
  DEBUG_OUT<<"End of main().\n";
#endif
  return 0;
}

