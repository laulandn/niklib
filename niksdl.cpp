

#include "niklib.h"


#include "nikdraw.h"


#ifdef NIK_SDL
#ifndef NIK_OPENGL
#ifndef NIK_SDL_DRAWING
#error "You must define either NIK_OPENGL or NIK_SDL_DRAWING!"
#endif // NIK_SDL_DRAWING
#endif // NIK_OPENGL
#endif


///////////////////////////////////////////////////////////////////////////////
//  Start of NIK_SDL section
////////////////////////////////////////////////////////////////////////////////


#ifdef NIK_SDL


#define NIK_JOYSTICK_SUPPORT 1
#define NIK_ALWAYS_SETUP_FONT 1
#define NIK_DOUBLE_BUFFER 1


#define NIK_DEFAULT_FONT "font.bmp"


#include <unistd.h>
#include <string.h>


#include "SDL.h"
#include "SDL_version.h"
#include "SDL_video.h"



#ifdef NIK_OPENGL
// NOTE: No GLUT for opengles!
#ifdef __APPLE__
#ifdef NIK_OPENGLES
#include <OpenGLES/ES1/gl.h>
#else
#include <AGL/agl.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#endif // NIK_OPENGLES
#define NIK_FOUND_GL_HEADER 1
#endif // __APPLE__
//
#ifdef _WIN32
#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <GL/glut.h>
#define NIK_FOUND_GL_HEADER 1
#endif // _WIN32
//
#ifndef NIK_FOUND_GL_HEADER
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#endif // NIK_FOUND_GL_HEADER
#endif // NIK_OPENGL


#if defined(__APPLE__) || defined(MACOSX)
#include <TargetConditionals.h>
#include <AvailabilityMacros.h>
#endif // __APPLE__



#ifdef NIK_SDL_DRAWING
unsigned int nikColorRed=0;
unsigned int nikColorGreen=0;
unsigned int nikColorBlue=0;
unsigned long nikColorLong=0;
//
NIK_BITMAP *nikTheFont=NULL;
unsigned int nikTheFontWidth=8;
unsigned int nikTheFontHeight=8;
#endif


#ifdef NIK_OPENGL
BOOL nikAppInitGLUT(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);
BOOL nikAppInitOpenGL(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);
#endif // NIK_OPENGL


BOOL nikAppInited=FALSE;
BOOL nikExitNextLoop=FALSE;
unsigned int nikActualWidth=0;
unsigned int nikActualHeight=0;
unsigned int nikActualDepth=0;
char *nikResourceFolder=(char *)NIK_DEFAULT_RESOURCEFOLDER;
NIK_BITMAP *nikBackBuffer=NULL;
//
SDL_Surface *niksdl_theSurface=NULL;
SDL_Event niksdl_theEvent;
SDL_Joystick *niksdl_theJoystick=NULL;
#if SDL_MAJOR_VERSION != 1
SDL_Window *niksdl_theWindow=NULL;
SDL_Renderer *niksdl_theRenderer=NULL;
#endif
//
unsigned int niksdl_translateKeysym(unsigned int c, unsigned int mod);
//
void (* nikTheKeyboardFunc)( unsigned char, int, int )=NULL;
void (* nikTheReshapeFunc)( int, int )=NULL;
void (* nikTheDisplayFunc)( void )=NULL;
void (* nikTheMouseFunc)( int, int, int, int )=NULL;
void (* nikTheMotionFunc)( int, int )=NULL;
void (* nikTheJoyMotionFunc)( int, int )=NULL;
void (* nikTheJoyButtonFunc)( int, int )=NULL;
void (* nikTheSpecialFunc)( int, int, int )=NULL;
void (* nikTheIdleFunc)( void )=NULL;


BOOL nikAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  int t=0;
  debug_init();
  debug_msg("nikAppInit("); debug_xyz(theWidth,theHeight,theDepth); debug_msg(")\n");
  //system("pwd");
  for(t=0;t<argc;t++) {
    debug_msg("argv["); debug_int(t); debug_msg("]="); debug_quotes(argv[t]); debug_nl();
  }
  nikResourceFolder=(char *)malloc(256);
#ifdef NIK_USE_ALIB
  DSHELPER_alib.fillInString(nikResourceFolder,NIK_DEFAULT_RESOURCEFOLDER);
#else
  nikResourceFolder[0]='.'; nikResourceFolder[1]=0;
#endif
  debug_msg("nikResourceFolder is: "); debug_quotes(nikResourceFolder); debug_nl();
  chdir(nikResourceFolder);
  BOOL ret=TRUE;
  //
  debug_msg("Going to call SDL_Init...\n");
  Uint32 flags=SDL_INIT_VIDEO;
#ifdef NIK_JOYSTICK_SUPPORT
  flags|=SDL_INIT_JOYSTICK;
#endif // NIK_JOYSTICK_SUPPORT
  if(SDL_Init(flags)<0) {
    debug_msg("Unable to init SDL: "); debug_msg(SDL_GetError()); debug_nl();
    return FALSE;
  }
  //
  atexit(SDL_Quit);
  //
#if SDL_MAJOR_VERSION == 1
  unsigned int vflags=0;
  debug_msg("Going to call SDL_VideoInit...\n");
  // NOTE: some versions don't have vflags...
  if(SDL_VideoInit(NULL,vflags)<0) {
  //if(SDL_VideoInit(NULL)<0) {
    debug_msg("Unable to videoinit SDL: "); debug_msg(SDL_GetError()); debug_nl();
    return FALSE;
  }
#endif
  //
  flags=0;
#ifdef NIK_DOUBLE_BUFFER
  flags|=SDL_HWSURFACE|SDL_DOUBLEBUF;
#endif // NIK_DOUBLE_BUFFER
#ifdef NIK_OPENGL
  flags|=SDL_OPENGL;
#endif // NIK_OPENGL
  if(theDepth<8) theDepth=8;
  //flags=0;
  //theWidth=0; theHeight=0; theDepth=0;
  debug_msg("Going to call SDL_SetVideoMode("); debug_xyz(theWidth,theHeight,theDepth);
  debug_msg(","); debug_int(flags);
  debug_msg(")...\n");
  niksdl_theSurface=SDL_SetVideoMode(theWidth,theHeight,theDepth,flags);
  if(!niksdl_theSurface) {
    debug_msg("Unable to set ");
    debug_msg(" video: "); debug_msgnl(SDL_GetError());
    return FALSE;
  }
  debug_msg("Back from SDL_SetVideoMode.\n");
  //
  SDL_LockSurface(niksdl_theSurface);
  nikActualWidth=niksdl_theSurface->w;
  nikActualHeight=niksdl_theSurface->h;
  nikActualDepth=theDepth;  // NOTE: Assumed we got it...
  SDL_UnlockSurface(niksdl_theSurface);
  debug_msg("niksdl_theSurface is "); debug_xy(nikActualWidth,nikActualHeight); debug_nl();
  //
  SDL_WM_SetCaption("niklib window","niklib window");
  //SDL_WM_GrabInput(SDL_GRAB_ON);
  //
#ifdef NIK_OPENGL
  //SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, nikActualDepth);
  //SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
  //SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
  //SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  if(ret) ret=nikAppInitOpenGL(nikActualWidth,nikActualHeight,nikActualDepth,argc,argv);
#endif // NIK_OPENGL
  //
#ifdef NIK_JOYSTICK_SUPPORT
  unsigned int joycount=SDL_NumJoysticks();
  debug_msg("joycount is "); debug_int(joycount); debug_dotdotdotnl();
  if(joycount) {
    niksdl_theJoystick=SDL_JoystickOpen(0);
    if(niksdl_theJoystick) {
      debug_msg("Got joystick...\n");
      SDL_JoystickEventState(SDL_ENABLE);
    }
    else {
      debug_msg("Failed to open joystick!\n");
    }
  }
#endif // NIK_JOYSTICK_SUPPORT
  //
#ifdef NIK_ALWAYS_SETUP_FONT
  debug_msg("Going to nLoadFont...\n");
  //nikLoadFont((char *)NIK_DEFAULT_FONT,16,16,8,8);
#endif // NIK_ALWAYS_SETUP_FONT
  //
  debug_msg("Going to create nikBackBuffer...\n");
  // NOTE: What was I doing with this #ifndef here again???
#ifdef NIK_USE_ALIB
  nikBackBuffer=new NIK_BITMAP(nikActualWidth,nikActualHeight,nikActualDepth);
#else
  nikBackBuffer=NULL;
#endif
  //
#ifdef NIK_NIKDRAWING
  // NOTE: Set up nikdrawing vars
  ndColorRed=nikColorRed;
  ndColorGreen=nikColorGreen;
  ndColorBlue=nikColorBlue;
  ndColorLong=nikColorLong;
  ndActualWidth=nikActualWidth;
  ndActualHeight=nikActualHeight;
  ndActualDepth=nikActualDepth;
  ndBytesPerPixel=niksdl_theSurface->format->BytesPerPixel;
  ndThePixels=(unsigned char *)niksdl_theSurface->pixels;
  ndThePixelsSize=nikActualWidth*nikActualHeight*ndBytesPerPixel;  // Is this always right?
#endif
  //
  nikAppInited=TRUE;
  debug_msg("nikAppInit done.\n");
  return ret;
}


///////////////////////////////////////////////////////////////////////////////
//  Start of event handling
////////////////////////////////////////////////////////////////////////////////


void nikRunEventLoop()
{
  debug_msg("nikRunEventLoop(...)\n");
  int x=0,y=0;
  int button=0;
  int state=0;
  int mod=0;
  int axis=0;
  int value=0;
  int which=0;
  char c=0;
  nikExitNextLoop=FALSE;
  BOOL done=FALSE;
  while(!done){
    //debug_msg("outside loop\n");
    while(SDL_PollEvent(&niksdl_theEvent)) {
      //debug_msg("inside loop\n");
      switch(niksdl_theEvent.type) {
        case SDL_QUIT:
          done=TRUE;
          break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
          x=0;  y=0;
          // NOTE: x and y aren't correct...
          c=niksdl_theEvent.key.keysym.sym;
          mod=niksdl_theEvent.key.keysym.mod;  // NOTE: not used
          x=niksdl_theEvent.key.keysym.unicode;
          y=niksdl_theEvent.key.keysym.scancode;
          value=niksdl_theEvent.key.type;
          debug_msg("SDL_KEYUP/DOWN: "); debug_int(c); debug_msg(" ");
          debug_int(mod); debug_msg(" "); debug_xy(x,y); debug_msg(" ");
          debug_int(value); debug_nl();
          if((mod==1024)&&(c='q')) done=TRUE;  // logo/apple/command-q
          c=niksdl_translateKeysym(c,mod);
          // NOTE: This next bit only works if the scancode is valid...not on all platforms...
          switch(y) {
            case 50:  // left shift
              // ignore
              break;
            case 64:  // left alt
              // ignore
              break;
            case 37:  // left ctrl
              // ignore
              break;
            case 62:  // right shift
              // ignore
              break;
            case 108:  // right alt
              // ignore
              break;
            case 105:  // right ctrl
              // ignore
              break;
            default:
              if(nikTheKeyboardFunc) nikTheKeyboardFunc(c,x,y);
              break;
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          button=niksdl_theEvent.button.button-1;  // GLUT numbers from 0, SDL from 1...
          state=niksdl_theEvent.button.state;
          x=niksdl_theEvent.button.x;
          y=niksdl_theEvent.button.y;
          debug_msg("SDL_MOUSEBUTTONUP/DOWN: "); debug_int(button); debug_msg(" ");
          debug_int(axis); debug_msg(" "); debug_xy(x,y); debug_nl();
          if(nikTheMouseFunc) nikTheMouseFunc(button,state,x,y);
          break;
        case SDL_MOUSEMOTION:
          button=niksdl_theEvent.button.button-1;  // GLUT numbers from 0, SDL from 1...
          state=niksdl_theEvent.button.state;
          x=niksdl_theEvent.motion.x;
          y=niksdl_theEvent.motion.y;
          // NOTE: xrel and yrel are not used
          debug_msg("SDL_MOUSEMOTION: "); debug_int(button); debug_msg(" ");
          debug_int(axis); debug_msg(" "); debug_xy(x,y); debug_nl();
          // NOTE: button and state aren't passed to nikTheMotionFunc...
          if(nikTheMotionFunc) nikTheMotionFunc(x,y);
          break;
        case SDL_JOYAXISMOTION:
          which=niksdl_theEvent.jaxis.which;
          axis=niksdl_theEvent.jaxis.axis;
          value=niksdl_theEvent.jaxis.value;
          debug_msg("SDL_JOYAXISMOTION: "); debug_int(which); debug_msg(" ");
          debug_int(axis); debug_msg(" "); debug_int(value); debug_nl();
          if(nikTheJoyMotionFunc) nikTheJoyMotionFunc(axis,value);
          break;
        case SDL_JOYHATMOTION:
          which=niksdl_theEvent.jhat.which;
          axis=niksdl_theEvent.jhat.hat;
          value=niksdl_theEvent.jhat.value;
          debug_msg("SDL_JOYHATMOTION: "); debug_int(which); debug_msg(" ");
          debug_int(axis); debug_msg(" "); debug_int(value); debug_nl();
          break;
        case SDL_JOYBALLMOTION:
          which=niksdl_theEvent.jball.which;
          axis=niksdl_theEvent.jball.ball;
          x=niksdl_theEvent.jball.xrel;
          y=niksdl_theEvent.jball.yrel;
          debug_msg("SDL_JOYBALLMOTION: "); debug_int(which); debug_msg(" ");
          debug_int(axis); debug_msg(" "); debug_xy(x,y); debug_nl();
          break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
          which=niksdl_theEvent.jbutton.which;
          button=niksdl_theEvent.jbutton.button;
          state=niksdl_theEvent.jbutton.state;
          debug_msg("SDL_JOYBUTTONUP/DOWN: "); debug_int(which); debug_msg(" ");
          debug_int(button); debug_msg(" "); debug_int(state); debug_nl();
          if(nikTheJoyButtonFunc) nikTheJoyButtonFunc(button,state);
          break;
        case SDL_VIDEORESIZE:
          // NOTE: Marked as deprecated in SDL
          which=niksdl_theEvent.resize.type;
          x=niksdl_theEvent.resize.w;
          y=niksdl_theEvent.resize.h;
          debug_msg("SDL_VIDEORESIZE: "); debug_int(which); debug_msg(" "); debug_xy(x,y); debug_nl();
          if(nikTheReshapeFunc) nikTheReshapeFunc(x,y);
          break;
        case SDL_VIDEOEXPOSE:
          // NOTE: Marked as deprecated in SDL
          if(nikTheDisplayFunc) nikTheDisplayFunc();
          break;
        case SDL_ACTIVEEVENT:
          // NOTE: Marked as deprecated in SDL
          state=niksdl_theEvent.active.state;
          which=niksdl_theEvent.active.gain;
          if(nikTheDisplayFunc) nikTheDisplayFunc();
          break;
#if SDL_VERSIONNUM(SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL) > 1299
         case SDL_WINDOWEVENT:
          // NOTE: Unused for now
          which=niksdl_theEvent.window.windowID;
          x=niksdl_theEvent.window.data1;
          y=niksdl_theEvent.window.data2;
          if(nikTheDisplayFunc) nikTheDisplayFunc();
          break;
#endif // SDL_VERSIONNUM
        default:
          debug_msg("! Unhandled SDL event type "); debug_int((int)niksdl_theEvent.type); debug_msg("!\n");
          break;
      }
    }
    if(nikTheIdleFunc) nikTheIdleFunc();
    //if(nikTheDisplayFunc) nikTheDisplayFunc();
    if(nikExitNextLoop) done=TRUE;
  }
  debug_msg("Done with loop.\n");
}


// NOTE: Yes, I know this is stupid and very speicific to EN-US key layout...
// I just don't know how SDL does this the right way yet...
// NOTE: Doesn't handle more than one mod at a time
unsigned int niksdl_translateKeysym(unsigned int c, unsigned int mod)
{
  BOOL done=FALSE;
  unsigned int ret=c;
  // Shift
  if(mod==1) {
    done=TRUE;
    switch(c) {
      case '`':  ret='~';  break;
      case '1':  ret='!';  break;
      case '2':  ret='@';  break;
      case '3':  ret='#';  break;
      case '4':  ret='$';  break;
      case '5':  ret='%';  break;
      case '6':  ret='^';  break;
      case '7':  ret='&';  break;
      case '8':  ret='*';  break;
      case '9':  ret='(';  break;
      case '0':  ret=')';  break;
      case '-':  ret='_';  break;
      case '=':  ret='+';  break;
      case '[':  ret='{';  break;
      case ']':  ret='}';  break;
      case '\\':  ret='|';  break;
      case ';':  ret=':';  break;
      case '\'':  ret='"';  break;
      case ',':  ret='<';  break;
      case '.':  ret='>';  break;
      case '/':  ret='?';  break;
      default:
        break;
    }
  }
  // control
  if(mod==64) {
    done=TRUE;
    switch(c) {
      default:
        ret=c-'a';
        break;
    }
  }
  // alt
  if(mod==256) {
    switch(c) {
      default:
        break;
    }
  }
  // logo/command/cloverleaf
  if(mod==1024) {
    switch(c) {
      default:
        break;
    }
  }
  if(!done) debug_msg("niksdl_translateKeysym "); debug_int(c); debug_msg(","); debug_int(mod); debug_msg(" unhandled!!!\n");
  return ret;
}


void nikExitFromRunLoop()
{
  nikExitNextLoop=TRUE;
}


void nikKeyboardFunc( void (* callback)( unsigned char, int, int ) )
{
  debug_msg("nikKeyboardFunc called...\n");
  nikTheKeyboardFunc=callback;
}


void nikReshapeFunc( void (* callback)( int, int ) )
{
  debug_msg("nikReshapeFunc called...\n");
  nikTheReshapeFunc=callback;
}


void nikDisplayFunc( void (* callback)( void ) )
{
  debug_msg("nikDisplayFunc called...\n");
  nikTheDisplayFunc=callback;
}


void nikMouseFunc( void (* callback)( int, int, int, int ) )
{
  debug_msg("nikMouseFunc called...\n");
  nikTheMouseFunc=callback;
}


void nikMotionFunc( void (* callback)( int, int ) )
{
  debug_msg("nikMotionFunc called...\n");
  nikTheMotionFunc=callback;
}


void nikJoyButtonFunc( void (* callback)( int, int ) )
{
  debug_msg("nikJoyButtonFunc called...\n");
  nikTheJoyButtonFunc=callback;
}


void nikJoyMotionFunc( void (* callback)( int, int ) )
{
  debug_msg("nicJoykMotionFunc called...\n");
  nikTheJoyMotionFunc=callback;
}


void nikSpecialFunc( void (* callback)( int, int, int ) )
{
  debug_msg("nikSpecialFunc called...\n");
  nikTheSpecialFunc=callback;
}


void nikIdleFunc( void (* callback)( void ) )
{
  debug_msg("nikIdleFunc called...\n");
  nikTheIdleFunc=callback;
}


///////////////////////////////////////////////////////////////////////////////
//  End of event handling
////////////////////////////////////////////////////////////////////////////////


void nikLockDisplay()
{
  SDL_LockSurface(niksdl_theSurface);
#ifdef NIK_OPENGL
#ifndef NIK_OPENGLES
  glBegin(GL_TRIANGLES);
#endif // NIK_OPENGLES
#endif // NIK_OPENGL
}


void nikUnlockDisplay()
{
#ifdef NIK_OPENGL
#ifndef NIK_OPENGLES
  glEnd();
#endif // NIK_OPENGLES
#endif // NIK_OPENGL
  SDL_UnlockSurface(niksdl_theSurface);
}


void nikFlipDisplay()
{
#ifdef NIK_OPENGL
  glFlush();
  SDL_GL_SwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
#else
#ifdef NIK_DOUBLE_BUFFER
  SDL_Flip(niksdl_theSurface);
#else
  SDL_UpdateRect(niksdl_theSurface,0,0,0,0);
#endif // NIK_DOUBLE_BUFFER
#endif // NIK_OPENGL
}


#endif // NIK_SDL


///////////////////////////////////////////////////////////////////////////////
//  End of NIK_SDL section
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//  Start of NIK_SDL_DRAWING section
////////////////////////////////////////////////////////////////////////////////


#ifdef NIK_SDL_DRAWING


void nikNotifyOfResize(int w, int h)
{
  debug_msg("nikNotifyOfResize("); debug_int(w); debug_msg("x"); debug_int(h); debug_msg(")\n");
  nikActualWidth=w;  nikActualHeight=h;
}


BOOL nikMatchDisplayToBitmapIfPossible(NIK_DRAWABLE_BITMAP *bm)
{
  if(!bm) { debug_msg("no bm!\n"); return FALSE; }
  // No, you need to use what you already have!
  return FALSE;
}


NIK_DRAWABLE_BITMAP *nikGetDrawableBitmap(NIK_BITMAP *bm)
{
  if(!bm) { debug_msg("no bm!\n"); return FALSE; }
  return (NIK_DRAWABLE_BITMAP *)bm;
}


BOOL nikPrepareBitmap(NIK_BITMAP *bm)
{
  if(!bm) { debug_msg("no bm!\n"); return FALSE; }
  // NOTE: What was I doing with this #ifndef here again???
//#ifndef __cplusplus
  //bm->reorderRGB();
//#endif
  return TRUE;
}


// NOTE: We don't handle neg or off screen coords
void nikDrawImage(NIK_DRAWABLE_BITMAP *tbm,unsigned int x,unsigned int y)
{
  unsigned int cy=0;
  if(!tbm) return;
#ifdef NIK_USE_ALIB
  NIK_BITMAP *bm=(NIK_BITMAP *)tbm;
  if(bm->getBitmapType()!=ALIB_BITMAP_CHUNKY) { debug_msg("Can't draw, not chunky!\n"); exit(EXIT_FAILURE); }
  if(bm->getDepth()!=24) { debug_msg("Can't draw, not 24 bit!\n"); exit(EXIT_FAILURE); }
  unsigned int dw=bm->getWidth();
  unsigned int dh=bm->getHeight();
#else
  unsigned int dw=0;
  unsigned int dh=0;
#endif
  if(dw>(unsigned int)niksdl_theSurface->w) dw=niksdl_theSurface->w;
  if(dh>(unsigned int)niksdl_theSurface->h) dh=niksdl_theSurface->h;
  if(x>dw) x=0;  // TODO: Just to get it to work (could also be negative)
  if(y>dh) y=0;  // TODO: Just to get it to work (could also be negative)
  //debug_msg("dw,dh="); dw debug_msg(","); dh debug_msg("\n");
  unsigned int minBlit=1;
  unsigned int maxBlit=niksdl_theSurface->format->BytesPerPixel;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned int dbpl=niksdl_theSurface->format->BytesPerPixel*niksdl_theSurface->w;
#ifdef NIK_USE_ALIB
  unsigned int sbpl=bm->getBytesPerLine();
#else
  unsigned int sbpl=0;
#endif
  //debug_msg("maxBlit="); maxBlit debug_msg(" tx="); tx debug_msg(" dbpl="); dbpl debug_msg(" sbpl="); sbpl debug_msg("\n");
#ifdef NIK_USE_ALIB
  unsigned char *isrc=bm->getPlane(0);
#else
  unsigned char *isrc=NULL;
#endif
  unsigned char *idest=(unsigned char *)niksdl_theSurface->pixels+tx+(y*dbpl);
  if(!isrc) { debug_msg("isrc is NULL!\n"); exit(EXIT_FAILURE); }
  if(!idest) { debug_msg("idest is NULL!\n"); exit(EXIT_FAILURE); }
  for(cy=0;cy<dh-1;cy++) {
    //debug_msg("Copying line "); cy debug_msg("\n");
    isrc+=sbpl;
    idest+=dbpl;
    memcpy(idest,isrc,sbpl);
  }
}


/*
NIK_PALETTE *nikGetCurPalette()
{
  //debug_msg("nikGetCurPalette not implemented!\n");
  return NULL;
}
*/


NIK_DRAWABLE_BITMAP *nikLoadImage(char *name)
{
  debug_msg("nikLoadImage not implemented!\n");
  exit(EXIT_FAILURE);
  return NULL;
}


void nikDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
#ifdef NIK_NIKDRAWING
  //debug_msg("Herp\n");
  ndDrawLine(xu1,yu1,xu2,yu2);
#else
  //debug_msg("Derp\n");
  int x1=xu1, x2=xu2, y1=yu1, y2=yu2;
  unsigned int x,y;
  int deltax,deltay,temp,err,i,swap,s1,s2;
  x=x1;  y=y1;
  // FIXME: The unsigned coords might mess the next 4 lines up a little...
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
    nikWritePixel(x,y);
    while(err>=0) {
      if(swap) x+=s1; else y+=s2;
      err-=2*deltax;
    }
    if(swap) y+=s2; else x+=s1;
    err+=2*deltay;
  }
#endif
}


// NOTE: 24 bit pixel format is assumed...bad!
void nikWritePixel(unsigned int x,unsigned int y)
{
#ifdef NIK_NIKDRAWING
  ndWritePixel(x,y);
#else
  if(x>(unsigned int)niksdl_theSurface->w) x=niksdl_theSurface->w-1;
  if(y>(unsigned int)niksdl_theSurface->h) y=niksdl_theSurface->h-1;
  unsigned int minBlit=1;
  unsigned int maxBlit=niksdl_theSurface->format->BytesPerPixel;
  unsigned int bytesPerLine=niksdl_theSurface->format->BytesPerPixel*niksdl_theSurface->w;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned char *dest=(unsigned char *)niksdl_theSurface->pixels+tx+(y*bytesPerLine);
  dest[2]=nikColorRed;
  dest[1]=nikColorGreen;
  dest[0]=nikColorBlue;
#endif
}


// NOTE: 24 bit pixel format is assumed...bad!
unsigned long nikReadPixel(unsigned int x,unsigned int y)
{
#ifdef NIK_NIKDRAWING
 return ndReadPixel(x,y);
#else
  if(x>(unsigned int)niksdl_theSurface->w) x=niksdl_theSurface->w-1;
  if(y>(unsigned int)niksdl_theSurface->h) y=niksdl_theSurface->h-1;
  unsigned int minBlit=1;
  unsigned int maxBlit=niksdl_theSurface->format->BytesPerPixel;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned int bytesPerLine=niksdl_theSurface->format->BytesPerPixel*niksdl_theSurface->w;
  unsigned char *src=(unsigned char *)niksdl_theSurface->pixels+tx+(y*bytesPerLine);
  return makeRGB24(src[2],src[1],src[0]);
#endif
  return 0;
}


// WARNING!  We assume you know what you're doing and tbm already matches the frame buffer!
void nikCopyToFrameBuffer(NIK_DRAWABLE_BITMAP *tbm)
{
#ifdef NIK_NIKDRAWING
  ndCopyToFrameBuffer(tbm);
#else
  unsigned int copySize=niksdl_theSurface->format->BytesPerPixel*niksdl_theSurface->w*niksdl_theSurface->h;
#ifdef NIK_USE_ALIB
  unsigned char *isrc=((NIK_BITMAP *)tbm)->getPlane(0);
#else
  unsigned char *isrc=NULL;
#endif
  unsigned char *idest=(unsigned char *)niksdl_theSurface->pixels;
  if(!isrc) { debug_msg("isrc is NULL!\n"); exit(EXIT_FAILURE); }
  if(!idest) { debug_msg("idest is NULL!\n"); exit(EXIT_FAILURE); }
  memcpy(idest,isrc,copySize);
#endif
}


void nikCopyBlock(NIK_BITMAP *tbm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
#ifdef NIK_NIKDRAWING
  ndCopyBlockToFrameBuffer(tbm,sx,sy,dx,dy,bw,bh);
#else
#ifdef NIK_USE_ALIB
  NIK_BITMAP *bm=(NIK_BITMAP *)tbm;
  if(bm->getBitmapType()!=ALIB_BITMAP_CHUNKY) { debug_msg("Can't draw, not chunky!\n"); exit(EXIT_FAILURE); }
  if(bm->getDepth()!=24) { debug_msg("Can't draw, not 24 bit!\n"); exit(EXIT_FAILURE); }
#endif
  //unsigned int dw=bm->getWidth();
  //if(dw>niksdl_theSurface->w) dw=niksdl_theSurface->w;
  //unsigned int dh=bm->getHeight();
  //if(dh>niksdl_theSurface->h) dh=niksdl_theSurface->h;
  //debug_msg("dw,dh="); dw debug_msg(","); dh debug_msg("\n");
  unsigned int cy=0;
  unsigned int minBlit=1;
  unsigned int maxBlit=niksdl_theSurface->format->BytesPerPixel;
#ifdef NIK_USE_ALIB
  unsigned int smaxBlit=bm->getMaxBlit();
#else
  unsigned int smaxBlit=0;
#endif
  unsigned int tx=(dx/minBlit)*maxBlit;
  unsigned int dbpl=niksdl_theSurface->format->BytesPerPixel*niksdl_theSurface->w;
  unsigned int sdw=smaxBlit*bw;
#ifdef NIK_USE_ALIB
  unsigned int sbpl=bm->getBytesPerLine();
#else
  unsigned int sbpl=0;
#endif
  //debug_msg("maxBlit="); maxBlit debug_msg(" tx="); tx debug_msg(" dbpl="); dbpl debug_msg(" sbpl="); sbpl debug_msg("\n");
#ifdef NIK_USE_ALIB
  unsigned char *src=bm->getPlane(0)+(sbpl*sy)+(smaxBlit*sx);
#else
  unsigned char *src=NULL;
#endif
  unsigned char *dest=(unsigned char *)niksdl_theSurface->pixels+tx+(dy*dbpl);
  if(!src) { debug_msg("src is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dest) { debug_msg("dest is NULL!\n"); exit(EXIT_FAILURE); }
  for(cy=0;cy<bh;cy++) {
    //debug_msg("Copying line "); cy debug_msg("\n");
    src+=sbpl;
    dest+=dbpl;
    memcpy(dest,src,sdw);
  }
#endif
}


void nikClearBlock(unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
#ifdef NIK_NIKDRAWING
  ndClearBlockFrameBuffer(dx,dy,bw,bh);
#else
  //unsigned int dw=bm->getWidth();
  //if(dw>niksdl_theSurface->w) dw=niksdl_theSurface->w;
  //unsigned int dh=bm->getHeight();
  //if(dh>niksdl_theSurface->h) dh=niksdl_theSurface->h;
  //debug_msg("dw,dh="); dw debug_msg(","); dh debug_msg("\n");
  unsigned int cy=0;
  unsigned int minBlit=1;
  unsigned int maxBlit=niksdl_theSurface->format->BytesPerPixel;
  unsigned int tx=(dx/minBlit)*maxBlit;
  unsigned int dbpl=niksdl_theSurface->format->BytesPerPixel*niksdl_theSurface->w;
  //debug_msg("maxBlit="); maxBlit debug_msg(" tx="); tx debug_msg(" dbpl="); dbpl debug_msg(" sbpl="); sbpl debug_msg("\n");
  unsigned char *dest=(unsigned char *)niksdl_theSurface->pixels+tx+(dy*dbpl);
  for(cy=0;cy<bh;cy++) {
    //debug_msg("Copying line "); cy debug_msg("\n");
    dest+=dbpl;
    memset(dest,0,dbpl);
  }
#endif
}


void nikDrawString(const char *st,unsigned int x,unsigned int y)
{
#ifdef NIK_NIKDRAWING
  ntDrawString(st,x,y);  // NOTE "nt" stands for N Text
#else
  char c;
  unsigned int sx=0,sy=0;
  unsigned int t=0;
  if(!st) return;
  if(nikTheFont) {
    for(t=0;t<strlen(st);t++) {
      c=st[t];
      sy=(c/16)*nikTheFontHeight;
      sx=(c%16)*nikTheFontWidth;
      nikCopyBlock((NIK_DRAWABLE_BITMAP *)nikTheFont,sx,sy,x,y,nikTheFontWidth,nikTheFontHeight);
    }
  }
  else debug_msg("nikDrawString no nikTheFont!\n");
#endif
}


void nikClearFrameBuffer()
{
#ifdef NIK_NIKDRAWING
  ndClearFrameBuffer();
#else
  SDL_FillRect(niksdl_theSurface,0,SDL_MapRGB(niksdl_theSurface->format,nikColorRed,nikColorGreen,nikColorBlue));
#endif
}


void nikSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
#ifdef NIK_NIKDRAWING
  ndSetColorRGB(r,g,b);
#else
  nikColorLong=((r<<16)&0xff0000)+((b<<8)&0xff00)+(g&0xff);
  nikColorRed=r;
  nikColorGreen=g;
  nikColorBlue=b;
#endif
}


void nikSetColorLong(unsigned long c)
{
#ifdef NIK_NIKDRAWING
  ndSetColorLong(c);
#else
  nikColorLong=c;
  nikColorRed=getRGBRed24(c);
  nikColorGreen=getRGBGreen24(c);
  nikColorBlue=getRGBBlue24(c);
#endif
}


///////////////////////////////////////////////////////////////////////////////
//  End of NIK_SDL_DRAWING section
////////////////////////////////////////////////////////////////////////////////


#endif // NIK_SDL_DRAWING
