

#include "niklib.h"


#include "nikdraw.h"


///////////////////////////////////////////////////////////////////////////////
//  Start of NIK_POSIX section
////////////////////////////////////////////////////////////////////////////////


#ifdef NIK_POSIX


//#define NIK_DONT_MESS_WTH_TERM 1


#define NIK_JOYSTICK_SUPPORT 1
#define NIK_ALWAYS_SETUP_FONT 1
#define NIK_DOUBLE_BUFFER 1


#define NIK_DEFAULT_FONT "font.bmp"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>


#if defined(__APPLE__) || defined(MACOSX)
#include <TargetConditionals.h>
#include <AvailabilityMacros.h>
#endif // __APPLE__


NIK_BITMAP *nikBackBuffer=NULL;
//
NIK_BITMAP *nikTheFont=NULL;


BOOL nikAppInited=FALSE;
//
struct termios nikPosixOrigTermAttr;
struct termios nikPosixNewTermAttr;
//
void (* nikTheKeyboardFunc)( unsigned char, int, int )=NULL;
void (* nikTheReshapeFunc)( int, int )=NULL;
void (* nikTheDisplayFunc)( void )=NULL;
void (* nikTheMouseFunc)( int, int, int, int )=NULL;
void (* nikTheMotionFunc)( int, int )=NULL;
void (* nikTheSpecialFunc)( int, int, int )=NULL;
void (* nikTheIdleFunc)( void )=NULL;

unsigned int nikActualWidth=0;
unsigned int nikActualHeight=0;
unsigned int nikColorRed=0;
unsigned int nikColorGreen=0;
unsigned int nikColorBlue=0;
unsigned long nikColorLong=0;
//
char *nikResourceFolder=(char *)NIK_DEFAULT_RESOURCEFOLDER;
//
unsigned int nikTheFontWidth=8;
unsigned int nikTheFontHeight=8;


// Based on values that xterm uses
int nikPosixColors[]={
// 8 normal intensity
0,0,0,
205,0,0,
0,205,0,
205,205,0,
0,0,238,
205,0,205,
0,205,205,
229,229,229,
// 8 bright intensity
127,127,127,
255,0,0,
0,255,0,
255,255,0,
92,92,255,
255,0,255,
0,255,255,
255,255,255
};


// Forward
void nikPosixAtExit();
void nikPosixSetTerm();
void nikPosixRestoreTerm();


BOOL nikAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  debug_msg("nikAppInit(...)\n");
  //system("pwd");
  for(int t=0;t<argc;t++) {
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
  atexit(nikPosixAtExit);
  nikPosixSetTerm();
  // Hard coded for now...
  nikActualWidth=80;  nikActualHeight=24;
  nikAppInited=TRUE;
  debug_msg("nikAppInit done.\n");
  return ret;
}


void nikPosixAtExit()
{
  debug_msg("nikPosixAtExit()\n");
  nikPosixRestoreTerm();
}


void nikPosixSetTerm()
{
  debug_msg("nikPosixSetTerm()\n");
#ifndef NIK_DONT_MESS_WTH_TERM
  tcgetattr(fileno(stdin),&nikPosixOrigTermAttr);
  memcpy(&nikPosixNewTermAttr,&nikPosixOrigTermAttr,sizeof(struct termios));
  nikPosixNewTermAttr.c_lflag&=~(ECHO|ICANON);
  nikPosixNewTermAttr.c_cc[VTIME]=0;
  nikPosixNewTermAttr.c_cc[VMIN]=0;
  tcsetattr(fileno(stdin),TCSANOW,&nikPosixNewTermAttr);
#else
  debug_msg("NOTE: Not messing with term!\n");
#endif // NIK_DONT_MESS_WTH_TERM
}


void nikPosixRestoreTerm()
{
  debug_msg("nikPosixRestoreTerm()\n");
#ifndef NIK_DONT_MESS_WTH_TERM
  tcsetattr(fileno(stdin),TCSANOW,&nikPosixOrigTermAttr);
#else
  debug_msg("NOTE: Not messing with term!\n");
#endif // NIK_DONT_MESS_WTH_TERM
}


void nikRunEventLoop()
{
  debug_msg("nikRunEventLoop(...)\n");
  int x=0,y=0;
  BOOL done=FALSE;
  char c=0;
  nikPosixSetTerm();
  while(!done){
    c=fgetc(stdin);
    if(c==EOF) {
      //
    }
    else {
      if(c<32) {
        debug_msg("keypress was "); debug_int(c); debug_msg("\n");
        if(c==10) c=13;
      }
      if(nikTheKeyboardFunc) nikTheKeyboardFunc(c,x,y);
    }
    //if(nikTheDisplayFunc) nikTheDisplayFunc();
    if(nikTheIdleFunc) nikTheIdleFunc();
  }
  nikPosixRestoreTerm();
  debug_msg("Done with loop.\n");
}


void nikExitFromRunLoop()
{
  //nikExitNextLoop=TRUE;
  debug_msg("nikExitFromRunLoop not implemented!\n");
  exit(EXIT_FAILURE);
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


void nikNotifyOfResize(int w, int h)
{
  debug_msg("nikNotifyOfResize("); debug_xy(w,h); debug_msg(")\n");
  nikActualWidth=w;  nikActualHeight=h;
}


void nikDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  int x1=xu1, x2=xu2, y1=yu1, y2=yu2;
  int x,y,deltax,deltay,temp,err,i,swap,s1,s2;
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
}


// NOTE: pixel format is assumed...bad!
void nikWritePixel(unsigned int x,unsigned int y)
{
  printf("@");
}


// NOTE: pixel format is assumed...bad!
unsigned long nikReadPixel(unsigned int x,unsigned int y)
{
  return 0;
}


void nikDrawString(const char *st,unsigned int x,unsigned int y)
{
  unsigned int xc=x/8,yc=y/8;
  xc++; yc++;
  printf("\x1b[%d;%dH%s",yc,xc,st);
}


void nikClearFrameBuffer()
{
  printf("\x1b[2J");
  printf("\x1b[1;1H");
}


void nikLockDisplay()
{
}


void nikUnlockDisplay()
{
}


void nikFlipDisplay()
{
}


void nikSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
  nikColorLong=((r<<16)&0xff0000)+((b<<8)&0xff00)+(g&0xff);
  nikColorRed=r;
  nikColorGreen=g;
  nikColorBlue=b;
}


void nikSetColorLong(unsigned long c)
{
  nikColorLong=c;
  //nikColorRed=getRGBRed24(c);
  //nikColorGreen=getRGBGreen24(c);
  //nikColorBlue=getRGBBlue24(c);
}


BOOL nikPrepareBitmap(NIK_BITMAP *bm)
{
  if(!bm) { debug_msg("no bm!\n"); return FALSE; }
  //((ABitmap *)bm)->reorderRGBChunky2BGR();
  return TRUE;
}


BOOL nikMatchDisplayToBitmapIfPossible(NIK_DRAWABLE_BITMAP *bm)
{
  // No, you need to use what you already have!
  return FALSE;
}


NIK_DRAWABLE_BITMAP *nikGetDrawableBitmap(NIK_BITMAP *bm)
{
  return (NIK_DRAWABLE_BITMAP *)bm;
}


void nikDrawImage(NIK_DRAWABLE_BITMAP *tbm,unsigned int x,unsigned int y)
{
}


void nikCopyBlock(NIK_DRAWABLE_BITMAP *tbm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
}


void nikCopyToFrameBuffer(NIK_DRAWABLE_BITMAP *tbm)
{
  /*
  unsigned int copySize=niksdl_theSurface->format->BytesPerPixel*niksdl_theSurface->w*niksdl_theSurface->h;
  unsigned char *isrc=((NIK_BITMAP *)tbm)->getPlane(0);
  unsigned char *idest=(unsigned char *)niksdl_theSurface->pixels;
  memcpy(idest,isrc,copySize);
  */
  debug_msg("nikCopyToFrameBuffer not implemented!\n");
  exit(EXIT_FAILURE);
}

void nikLoadFont(NIK_BITMAP *bm,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh)
{
  debug_msg("nikLoadFont not implemented!\n");
}


NIK_PALETTE *nikGetCurPalette()
{
  //debug_msg("nikGetCurPalette not implemented!\n");
  return NULL;
}


/*
void nikDrawFace3D(AMesh3D *m,unsigned short *f,int *dvs,unsigned int fnum)
{
  int x1,y1,x2,y2,x3,y3;
  //float z1,z2,z3;
  float c1,c2,c3;
  unsigned int clr=0xffffff;
  float fact=255.0;
  AFaceInfo3D **fis=m->getFaceInfos();
  AFaceInfo3D *fi=NULL;
  if(fis) fi=fis[fnum];
  //c1=(z1+z2)/2;  c2=(z2+z3)/2;  c3=(z3+z1)/2;
  c1=1.0;  c2=1.0;  c3=1.0;
  //
  //z1=dvs[f->a].z;
  //z2=dvs[f->b].z;
  //z3=dvs[f->c].z;
  //
  //x1=dvs[f->a].x;
  //if(x1<0.0) x1=-x1;
  //y1=dvs[f->a].y;
  //if(y1<0.0) y1=-y1;
  //x2=dvs[f->b].x;
  //if(x2<0.0) x2=-x2;
  //y2=dvs[f->b].y;
  //if(y2<0.0) y2=-y2;
  //x3=dvs[f->c].x;
  //if(x3<0.0) x3=-x3;
  //y3=dvs[f->c].y;
  //if(y3<0.0) y3=-y3;
  x1=dvs[f[fnum*3]*3];
  y1=dvs[f[fnum*3]*3+1];
  x2=dvs[f[fnum*3+1]*3];
  y2=dvs[f[fnum*3+1]*3+1];
  x3=dvs[f[fnum*3+2]*3];
  y3=dvs[f[fnum*3+2]*3+1];
  //
  //debug_msg("drawFace3D: ("); x1 debug_msg(","); y1 debug_msg(") ("); x2 debug_msg(","); y2 debug_msg(") ("); x3 debug_msg(","); y3 debug_msg(")\n");
  //debug_msg("face colors: "); f->fRed debug_msg(","); f->fGreen debug_msg(","); f->fBlue debug_msg("\n");
  //
  //if((z1>0)&&(z2>0)) {
  if(fi) clr=makeRGB24((int)(c1*fact*fi->fRed),(int)(c1*fact*fi->fGreen),(int)(c1*fact*fi->fBlue));
    nikSetColorLong(clr);
    nikDrawLine(x1,y1,x2,y2);
  //}
  //if((z2>0)&&(z3>0)) {
    nikDrawLine(x2,y2,x3,y3);
  //}
  //if((z3>0)&&(z1>0)) {
    nikDrawLine(x3,y3,x1,y1);
  //}
}
*/


/*
void nikDrawMesh3D(AMesh3D *m,float x,float y,float z)
{
  debug_msg("nikDrawMesh3D Mesh has "); m->getNumFaces() debug_msg(" faces...\n");
  //nikLockDisplay();
  //nikSetColorLong(0x404040);
  //nikClearFrameBuffer();
  int *dvs=m->getDrawableMesh()->getDrawVerts();
  for(unsigned int t=0;t<m->getNumFaces();t++) {
    unsigned short *f=m->getFaces();
    //debug_msg("face: "); f->a debug_msg(","); f->b debug_msg(","); f->c debug_msg("\n");
    nikDrawFace3D(m,f,dvs,t);
  }
  //nikUnlockDisplay();
  //nikFlipDisplay();
}
*/


/*
void nikDrawScene3D(AScene3D *s)
{
  unsigned int nshapes=s->getNumMeshes();
  //debug_msg("nikDrawScene3D Scene has "); nshapes debug_msg(" meshes...\n");
  for(unsigned int t=0;t<nshapes;t++) {
    AMesh3D *m=s->getMeshes()[t];
    nikDrawMesh3D(m,0.0,0.0,0.0);
  }
}
*/


///////////////////////////////////////////////////////////////////////////////
//  End of NIK_POSIX section
////////////////////////////////////////////////////////////////////////////////


#endif // NIK_POSIX
