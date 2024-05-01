
//#define DEBUG_OUT std::cerr
#define DEBUG_OUT std::cerr
//#define DEBUG_OUT dBug
#define ERR_OUT std::cerr
//#define DEBUG_OUT *aNullStream
#define CONS_OUT std::cout


// View and rotate 3d model file


#ifdef NICK_USE_DECODE3D


#ifdef __linux__
#define DEFAULT_FILE "/home/nick/nfiles/3d/tris.md2"
#endif // __linux__

#ifdef __APPLE__
#define DEFAULT_FILE "/Users/nick/nfiles/3d/dinasaur.obj"
#endif // __APPLE__


#include <iostream>
#include <stdlib.h>


#include "../niklib/niklib.h"
#include "../niklib/nik3d.h"


// NOTE: iPhone (non-retina) is 320x480
#define THE_WINDOW_WIDTH 320
#define THE_WINDOW_HEIGHT 480

// For Doom use 5000
#define THE_SCALE_FACTOR 50

#define NICK_AUTO_SCALE 1
//#define NICK_IGNORE_ARGV 1

//#define NICK_DO_NOT_ANIMATE 1

// Lower is faster
#define NICK_ANIM_SPEED 5

#define A_MOVE_STEP 100

//#define USE_DOUBLE_BUFFERING 1
//#define NV_FIT_TO_SCREEN 1


AWorld3D *theWorld=NULL;
const char *theFilename=NULL;
float xfac=THE_SCALE_FACTOR;
int prevx=0,prevy=0;
//unsigned int theFrame=0;
unsigned int frameCount=0;
float dx=0,dy=0,dz=0;
float xrot=0.0,yrot=0.0;
float xtheta=0.0,ytheta=0.0;


// Forward...
void drawScene();
void resizeScene(int w, int h);
void keyboardScene(unsigned char key, int x, int y);
void mouseEvent(int button, int state, int x, int y);
void motionScene(int x, int y);
void joyMotionScene(int axis, int val);

bool handleMesh(const char *fname,AMeshDecoder *decoder);
bool handleScene(const char *fname,ASceneDecoder *decoder);


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

void drawScene()
{
  //DEBUG_OUT<<"*** drawScene "<<nikActualWidth<<"x"<<nikActualHeight<<"...\n";
  //
  nikLockDisplay();
  nikSetColorLong(0x000000);
  nikClearFrameBuffer();
  //
  //cerr<<"dx is "<<dx<<" dy is "<<dy<<" dz is "<<dz<<"\n";
#ifndef NICK_DO_NOT_ANIMATE
  unsigned int t=0;
  AThing3D *theThing=NULL;
  frameCount++;
  if(frameCount==NICK_ANIM_SPEED) {
    frameCount=0;
    //theFrame++;
    for(t=0;t<theWorld->getNumThings();t++) {
      theThing=theWorld->getThings()[t];
      theThing->setCurAnimFrame(theThing->getCurAnimFrame()+1);
    }
  }
#endif // NICK_DO_NOT_ANIMATE
  theWorld->render();
  nikDrawScene3D(theWorld->getScene());
  nikUnlockDisplay();
  nikFlipDisplay();
}


void resizeScene(int w, int h)
{
  DEBUG_OUT<<"*** resizeScene("<<w<<","<<h<<")...\n";
  nikNotifyOfResize(w,h);
  theWorld->setDimesions(w,h);
  drawScene();
}


void mouseEvent(int button, int state, int x, int y)
{
  DEBUG_OUT<<"*** mouseEvent("<<button<<","<<state<<","<<x<<","<<y<<")...\n";
  DEBUG_OUT<<"button is "<<button<<" and state is "<<state<<"\n";
  prevx=x;  prevy=y;
  DEBUG_OUT<<prevx<<","<<prevy<<"\n";
}


void motionScene(int x, int y)
{
  //DEBUG_OUT<<"*** motionScene("<<x<<","<<y<<")...\n";
  if(prevx&&prevy) {
    xtheta = (prevy - y) * 360.0f / nikActualWidth;
    ytheta = (x - prevx) * 360.0f / nikActualHeight;
    theWorld->getCurCamera()->xRotBy(xtheta);
    theWorld->getCurCamera()->yRotBy(ytheta);
    //DEBUG_OUT<<prevx<<","<<prevy<<"\n";
  }
  prevx = x;
  prevy = y;
  drawScene();
}


// TODO: This just makes it jump in a direction for now...
void joyMotionScene(int axis, int val)
{
  DEBUG_OUT<<"*** joyMotionScene("<<axis<<","<<val<<")...\n";
  if(axis==1) {
    xrot=xrot+10*(val/32767);
    theWorld->getCurCamera()->xRotBy(xrot);
  }
  if(axis==0) {
    yrot=yrot+10*(val/32767);
    theWorld->getCurCamera()->yRotBy(yrot);
  }
}


void keyboardScene(unsigned char key, int x, int y)
{
  DEBUG_OUT<<"*** keyboardScene("<<key<<","<<x<<","<<y<<")...\n";
  switch(key) {
    case 'w':
      dx=A_MOVE_STEP;  dz=0;
      break;
    case 's':
      dx=-A_MOVE_STEP;  dz=0;
      break;
    case 'a':
      dx=0;  dz=-A_MOVE_STEP;
      break;
    case 'd':
      dx=0;  dz=A_MOVE_STEP;
      break;
    default:
      //dx=0;  dy=0;  dz=0;
      break;
  }
  //cerr<<"dx is "<<dx<<" dy is "<<dy<<" dz is "<<dz<<"\n";
  //transMat.translate(dz,0,dx);
  //drawScene();
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

bool handleScene(const char *fname,ASceneDecoder *decoder)
{
  if(!fname) return false;
  if(!decoder) return false;
  bool ret=true;
  DEBUG_OUT<<"*** handleScene...\n";
  //
  AScene3D *theScene=decoder->createNewSceneObject();
  DEBUG_OUT<<"Scene contains "<<theScene->getNumMeshes()<<" meshes and "<<theScene->getNumCameras()<<" cameras...\n";
  theWorld=new AWorld3D(theScene);
  /*
  AMesh3D *theMesh=NULL;
  for(unsigned int t=0;t<theScene->getNumMeshes();t++) {
    theMesh=theScene->getMeshes()[t];
    theMesh->translateAllVerts(-(theMesh->getBMin()->x+theMesh->getBMax()->x)/2,-(theMesh->getBMin()->y+theMesh->getBMax()->y)/2,-(theMesh->getBMin()->z+theMesh->getBMax()->z)/2);
    theMesh->scaleAllVerts(xfac,-xfac,16*xfac/nikActualWidth);
    theMesh->translateAllVerts(nikActualWidth/2,nikActualHeight/2,8);
  }
  */
  // TODO: Check for error?
  prevx=0;  prevy=0;
  //
  return ret;
}


bool handleMesh(const char *fname,AMeshDecoder *decoder)
{
  if(!fname) return false;
  if(!decoder) return false;
  bool ret=true;
  DEBUG_OUT<<"*** handleMesh...\n";
  //
  // Create empty scene in which to place the sole object
  AMesh3D *theMesh=decoder->createNewMeshObject();
  theMesh->isValid();
  AScene3D *theScene=new AScene3D();
  theScene->resizeToFit(theMesh);
  theScene->addMesh(theMesh);
  theWorld=new AWorld3D(theScene);
  /*
  theMesh->translateAllVerts(-(theMesh->getBMin()->x+theMesh->getBMax()->x)/2,-(theMesh->getBMin()->y+theMesh->getBMax()->y)/2,-(theMesh->getBMin()->z+theMesh->getBMax()->z)/2);
  theMesh->scaleAllVerts(xfac,-xfac,16*xfac/nikActualWidth);
  theMesh->translateAllVerts(nikActualWidth/2,nikActualHeight/2,8);
  */
  // TODO: check for error?
  prevx=0;  prevy=0;
  //
  return ret;
}


////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  char msg[256];
  //
  BOOL ok=nikAppInit(THE_WINDOW_WIDTH,THE_WINDOW_HEIGHT,24,argc,argv);
  if(!ok) { debug_msg("nikAppInit failed!\n"); exit(EXIT_FAILURE); }
  //
  if(argc!=2) {
#ifdef DEFAULT_FILE
    theFilename=(char *)DEFAULT_FILE;
#else
    std::cout<<argv[0]<<": Usage: "<<argv[0]<<" 3d_model_file\n";
    return 0;
#endif // DEFAULT_FILE
  }
  else {
    theFilename=argv[1];
  }
  //
  /*
#ifdef NIK_GUILIB
  if(!theFilename) {
    AFileRequester *rf=new AFileRequester();
    theFilename=rf->show(argv[0],"Choose an image file to display");
  }
#endif // NIK_GUILIB
*/
  //
  if(!theFilename) {
    char msg[256];
    sprintf(msg,(char *)"usage: %s {model filename}",argv[0]);
    std::cerr<<"Error! "<<msg<<"\n";
    return 5;
  }
  DEBUG_OUT<<"main() theFilename is: "<<theFilename<<"\n";
  //
  bool working=true;
  //while(working) {
    //DEBUG_OUT<<"main() Going to create pick a decoder...\n";
    ADecoderAbstract *dec=(ADecoderAbstract *)NULL;
    if(!dec) {
      DEBUG_OUT<<"main() going to try ASmartMeshDecoder::pickMeshDecoder...\n";
      dec=ASmartMeshDecoder::pickMeshDecoder(theFilename);
    }
    if(!dec) {
      DEBUG_OUT<<"main() going to try ASmartSceneDecoder::pickSceneDecoder...\n";
      dec=ASmartSceneDecoder::pickSceneDecoder(theFilename);
    }
    //DEBUG_OUT<<"main() (past the picking decoders part of the code)\n";
    if(!dec) {
      sprintf(msg,(char *)"Couldn't open file %s!\n",theFilename);
      std::cerr<<"Error! "<<msg<<"\n";
      exit(EXIT_FAILURE);
    }
    if(dec->err.getError()) {
        sprintf(msg,(char *)"Problem reading file %s!\n",theFilename);
        std::cerr<<"Error! "<<msg<<"\n";
        exit(EXIT_FAILURE);
    }
    bool handled=false;
    DEBUG_OUT<<"main() Going to call appropriate handle function for what we got...\n";
    if(dec->isMesh()) {
      DEBUG_OUT<<"main() isMesh is true.\n";
      working=handleMesh(theFilename,(AMeshDecoder *)dec);
      handled=true;
    }
    if(dec->isScene()) {
      DEBUG_OUT<<"main() isScene is true.\n";
      working=handleScene(theFilename,(ASceneDecoder *)dec);
      handled=true;
    }
    if(!handled) {
      std::cerr<<"main() Something went wrong...file was neither a mesh nor a scene!\n";
      return 5;
    }
    DEBUG_OUT<<"main() Going to see if we have theWorld...\n";
    if(!theWorld) {
    std::cerr<<"Didn't get theWorld...giving up!\n";
    exit(EXIT_FAILURE);
    }
  theWorld->setDimesions(nikActualWidth,nikActualHeight);
  theWorld->prepareForRendering();
  nikPrepareScene3D(theWorld->getScene());
  theWorld->debugDump();
  //
#ifdef NICK_AUTO_SCALE
  bool scaling=true;
  bool grew=false,shrank=false;
  DEBUG_OUT<<"Picking best scale to fit...\n";
  unsigned int whichDim=nikActualWidth;
  if(whichDim>nikActualHeight) whichDim=nikActualHeight;
  if(!whichDim) {
    std::cerr<<"Didn't get window dimensions for some reason, giving up!";
    exit(EXIT_FAILURE);
  }
  unsigned int maxw=whichDim/2+12;
  unsigned int minw=whichDim/2+20;
  unsigned xw=theWorld->getXWidth(),yw=theWorld->getYWidth(),zw=theWorld->getZWidth();
  while(scaling) {
    if((xw*xfac)<minw) { xfac++; grew=true; }
    if((yw*xfac)<minw) { xfac++; grew=true; }
    if((zw*xfac)<minw) { xfac++; grew=true; }
    if((xw*xfac)>maxw) { xfac--; shrank=true; }
    if((yw*xfac)>maxw) { xfac--; shrank=true; }
    if((zw*xfac)>maxw) { xfac--; shrank=true; }
    if(grew&&shrank) scaling=false;
    //DEBUG_OUT<<theScene->getXWidth()*xfac<<","<<theScene->getYWidth()*xfac<<","<<theScene->getZWidth()*xfac<<": ";
    //DEBUG_OUT<<xfac<<" "<<grew<<" "<<shrank<<"\n";
  }
  theWorld->setScaling(xfac,xfac);
  DEBUG_OUT<<"picked scale of "<<xfac<<"\n";
#endif // NICK_AUTO_SCALE
  //
  nikDisplayFunc(drawScene);
  //nikIdleFunc(drawScene);
  nikReshapeFunc(resizeScene);
  nikMouseFunc(mouseEvent);
  nikMotionFunc(motionScene);
  nikJoyMotionFunc(joyMotionScene);
  nikKeyboardFunc(keyboardScene);
  //
  drawScene();
  nikRunEventLoop();
  //
  DEBUG_OUT<<"End of main().\n";
  return 0;
}


#else
#error nwire requires NICK_USE_DECODE3D!
#include <stdio.h>
int main(int argc,char **argv)
{
  printf("nwire requires NICK_USE_DECODE3D!\n");
  return 5;
}


#endif // NICK_USE_DECODE3D


