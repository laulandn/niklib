

#include "niklib.h"


#define NIK_3D_LINES_NOT_TRIANGLES 1


#ifdef NIK_OPENGL
#ifndef NIK_GLUT
#ifndef NIK_SDL
#error "You must define either NIK_GLUT or NIK_SDL!"
#endif // NIK_SDL
#endif // NIK_GLUT
#endif


#ifdef NIK_OPENGL
#ifdef NIK_OPENGLES
#ifndef NIK_OPENGL
#error "You must define either NIK_OPENGL!"
#endif // NIK_OPENGL
#endif // NIK_OPENGLES
#endif


///////////////////////////////////////////////////////////////////////////////
//  Start of NIK_OPENGL section
////////////////////////////////////////////////////////////////////////////////


#ifdef NIK_OPENGL


#include <stdio.h>
#include <stdlib.h>


#if defined(__APPLE__) || defined(MACOSX)
#include <TargetConditionals.h>
#include <AvailabilityMacros.h>
#endif // __APPLE__


#if defined(__APPLE__) || defined(MACOSX)
#ifdef ASYS_MAC
#include <AGL/agl.h>
#endif // ASYS_MAC
#define NIK_FOUND_GL_HEADER 1
#ifdef NIK_GLUT
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#ifdef NIK_OPENGLES
#include <OpenGLES/ES1/gl.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif // NIK_OPENGLES
#endif // NIK_GLUT
#endif // __APPLE__


#ifdef _WIN32
#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
#include <GL/glut.h>
#define NIK_FOUND_GL_HEADER 1
#endif // _WIN32


#ifndef NIK_FOUND_GL_HEADER
#ifdef NIK_GLUT
#include <GL/glut.h>
#endif // NIK_GLUT
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#endif // NIK_FOUND_GL_HEADER


#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
#include "../cpp/guilib/APrivateBitmap.h"
#include "../cpp/nick3d/AScene3D.h"
#endif
#endif


#ifdef NIK_OPENGL
unsigned int nikColorRed=0;
unsigned int nikColorGreen=0;
unsigned int nikColorBlue=0;
unsigned long nikColorLong=0;
float nikColorRedF=0.0;
float nikColorGreenF=0.0;
float nikColorBlueF=0.0;
//
NIK_BITMAP *nikTheFont=NULL;
unsigned int nikTheFontWidth=8;
unsigned int nikTheFontHeight=8;
//NIK_BITMAP *nikBackBuffer=NULL;
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
extern "C" BOOL nikAppInitOpenGL(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
BOOL nikAppInitOpenGL(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  debug_msg("nikAppInitOpenGL(...)\n");
  glEnable(GL_TEXTURE_2D);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0.0, 0.0, 0.0, 1.0);
#ifdef NIK_OPENGLES
  glClearDepthf(1.0);
#else
  glClearDepth(1.0);
#endif // NIK_OPENGLES
  glPolygonOffset(1.0, 1.0);
  glEnable(GL_CULL_FACE);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
#ifdef NIK_OPENGLES
#else
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
#endif // NIK_OPENGLES
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_COLOR_MATERIAL);
#ifdef NIK_OPENGLES
#else
  glColorMaterial(GL_FRONT, GL_DIFFUSE);
#endif // NIK_OPENGLES
  glViewport(0, 0, theWidth, theHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(65.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0,0.0,0.0);  /* viewing transform  */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#ifdef NIK_OPENGLES
  glOrthof(0.0,theWidth,theHeight,0.0,0.0,1.0);
#else
  glOrtho(0,theWidth,theHeight,0,0,1);
#endif // NIK_OPENGLES
  glMatrixMode (GL_MODELVIEW);
  debug_msg("nikAppInitOpenGL done.\n");
  nikActualWidth=theWidth;
  nikActualHeight=theHeight;
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  return TRUE;
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
void nikNotifyOfResize(int w, int h)
{
  debug_msg("nikNotifyOfResize("); debug_int(w); debug_msg("x"); debug_int(h); debug_msg(")\n");
  nikActualWidth=w;  nikActualHeight=h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#ifdef NIK_OPENGLES
  glOrthof(0.0,nikActualWidth,nikActualHeight,0.0,0.0,1.0);
#else
  glOrtho(0,nikActualWidth,nikActualHeight,0,0,1);
#endif // NIK_OPENGLES
  glMatrixMode(GL_MODELVIEW);
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
void nikWritePixel(unsigned int x,unsigned int y)
{
  debug_msg("nikWritePixel not implemented!\n");
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
unsigned long nikReadPixel(unsigned int x,unsigned int y)
{
  debug_msg("nikReadPixel not implemented!\n");
  return 0;
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
void nikDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  /*
  if(xu1>nikActualWidth) xu1=nikActualWidth-1;
  if(xu2>nikActualWidth) xu2=nikActualWidth-1;
  if(yu1>nikActualHeight) yu1=nikActualHeight-1;
  if(yu2>nikActualHeight) yu2=nikActualHeight-1;
   */
#ifdef NIK_OPENGLES
#else
/*
  glVertex3f(xu1,yu1,0.0);
  glVertex3f(xu2,yu2,0.0);
  glVertex3f(xu2+1,yu2,0.0);
  glVertex3f(xu2+1,yu2,0.0);
  glVertex3f(xu1+1,yu1,0.0);
  glVertex3f(xu1,yu1,0.0);
  */
#ifdef NIK_3D_LINES_NOT_TRIANGLES
  glVertex3f(xu1,yu1,0.0);
  glVertex3f(xu2,yu2,0.0);
#else
  glVertex3f(xu1,yu1,0.0);
  glVertex3f(xu2,yu2,0.0);
  glVertex3f(xu1,yu1,0.0);
#endif
#endif // NIK_OPENGLES
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
NIK_DRAWABLE_BITMAP *nikGetDrawableBitmap(NIK_BITMAP *bm)
{
  if(!bm) { debug_msg("no bm!\n"); return FALSE; }
  return (NIK_DRAWABLE_BITMAP *)bm;
}
#endif


#ifdef NIK_OPENGL
BOOL nikPrepareBitmap(NIK_BITMAP *bm)
{
  if(!bm) { debug_msg("no bm!\n"); return FALSE; }
  // NOTE: What was I doing with this #ifndef here again???
//#ifndef __cplusplus
  //bm->reorderRGB();
//#endif
  return TRUE;
}
#endif


#ifdef NIK_OPENGL
BOOL nikMatchDisplayToBitmapIfPossible(NIK_DRAWABLE_BITMAP *bm)
{
  if(!bm) { debug_msg("no bm!\n"); return FALSE; }
  // No, you need to use what you already have!
  return FALSE;
}
#endif


#ifdef NIK_OPENGL
void nikDrawImage(NIK_DRAWABLE_BITMAP *bm,unsigned int x,unsigned int y)
{
  debug_msg("nikDrawImage not implemented!\n");
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
void nikCopyBlock(NIK_DRAWABLE_BITMAP *bm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
  debug_msg("nikCopyBlock not implemented!\n");
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
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
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
void nikDrawString(const char *st,unsigned int x,unsigned int y)
{
  debug_msg("nikDrawString not implemented!\n");
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
BOOL nikLoadFont(char *theName,unsigned int cw,unsigned int ch,unsigned bw,unsigned int bh)
{
  debug_msg("nikLoadFont not implemented!\n");
  return FALSE;
}
#endif // NIK_OPENGL


/*
#ifdef NIK_OPENGL
NIK_PALETTE *nikGetCurPalette()
{
  //debug_msg("nikGetCurPalette not implemented!\n");
  return NULL;
}
#endif // NIK_OPENGL
*/


#ifdef NIK_OPENGL
// TODO: Should the alpha be something other than 0.0?
void nikClearFrameBuffer()
{
  //debug_msg("nikClearFrameBuffer: "); nikColorRedF debug_msg(","); nikColorGreenF debug_msg(","); nikColorBlueF debug_msg("\n");
  glClearColor(nikColorRedF,nikColorGreenF,nikColorBlueF,1.0f);
  //glClearColor(1.0f,0.0f,0.0f,1.0f);
#ifdef NIK_OPENGLES
  glClearDepthf(1.0);
#else
  glClearDepth(1);
#endif // NIK_OPENGLES
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glFlush();
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
void nikSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
  nikColorLong=((r<<16)&0xff0000)+((b<<8)&0xff00)+(g&0xff);
  nikColorRed=r;
  nikColorGreen=g;
  nikColorBlue=b;
  nikColorRedF=(float)nikColorRed/255.0;
  nikColorGreenF=(float)nikColorGreen/255.0;
  nikColorBlueF=(float)nikColorBlue/255.0;
  //debug_msg("nikSetColorRGB: "); nikColorRedF debug_msg(","); nikColorGreenF debug_msg(","); nikColorBlueF debug_msg("\n");
#ifdef NIK_OPENGLES
  glColor4f(nikColorRedF,nikColorGreenF,nikColorBlueF,1.0);
#else
  glColor3f(nikColorRedF,nikColorGreenF,nikColorBlueF);
  //glColor3b(nikColorRed,nikColorBlue,nikColorGreen);
#endif // NIK_OPENGLES
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
void nikSetColorLong(unsigned long c)
{
  nikColorLong=c;
  nikColorRed=getRGBRed24(c);
  nikColorGreen=getRGBGreen24(c);
  nikColorBlue=getRGBBlue24(c);
  nikColorRedF=(float)nikColorRed/255.0;
  nikColorGreenF=(float)nikColorGreen/255.0;
  //nikColorBlueF=(float)nikColorBlue/255.0;
  //debug_msg("nikSetColorLong: "); nikColorRedF debug_msg(","); nikColorGreenF debug_msg(","); nikColorBlueF debug_msg("\n");
#ifdef NIK_OPENGLES
  glColor4f(nikColorRedF,nikColorGreenF,nikColorBlueF,1.0);
#else
  glColor3f(nikColorRedF,nikColorGreenF,nikColorBlueF);
  //glColor3b(nikColorRed,nikColorBlue,nikColorGreen);
#endif // NIK_OPENGLES
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
NIK_DRAWABLE_BITMAP *nikLoadImage(char *name)
{
  debug_msg("nikLoadImage not implemented!\n");
  exit(EXIT_FAILURE);
  return NULL;
}
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
void *nikBindThisTexture3D(ATexture3D *tx)
{
  debug_msg("nikBindThisTexture3D(...)\n");
  GLuint tex;
  glGenTextures(1,&tex);
  glBindTexture(GL_TEXTURE_2D,tex);
  GLuint *nikglTexture=&tex;
  // select modulate to mix texture with color for shading
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  // when texture area is small, bilinear filter the closest mipmap
  //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
  // when texture area is large, bilinear filter the original
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  // the texture wraps over at the edges (repeat)
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  return (void *)nikglTexture;
}
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
BOOL nikPrepareMesh3D(AMesh3D *m)
{
  debug_msg("nikPrepareMesh3D(...)\n");
  unsigned char *rawdata=NULL;
  for(unsigned int t=0;t<m->getNumTextures();t++) {
    ATexture3D *tx=m->getTextures()[t];
    GLuint *tex=(GLuint *)nikBindThisTexture3D(tx);
    tx->setBinding(tex);
    ABitmap *bm=tx->getBitmap();
    if(!bm) { debug_msg("No bm!\n"); exit(EXIT_FAILURE); }
    rawdata=bm->getPlane(0);
    if(!rawdata) { debug_msg("No rawdata!\n"); exit(EXIT_FAILURE); }
    unsigned int w=bm->getWidth();
    unsigned int h=bm->getHeight();
//#ifdef NIK_OPENGLES
    glTexImage2D(GL_TEXTURE_2D,0,3,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,rawdata);
//#else
    // build our texture mipmaps
//    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, w, h,GL_RGB, GL_UNSIGNED_BYTE, rawdata );
//#endif // NIK_OPENGLES
  }
  return TRUE;
}
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
BOOL nikPrepareScene3D(AScene3D *s)
{
  debug_msg("nikPrepareScene3D(...)\n");
  for(unsigned int t=0;t<s->getNumMeshes();t++) {
    nikPrepareMesh3D(s->getMeshes()[t]);
  }
  return TRUE;
}
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


// Line only version (no triangles, nor textures)
#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
#ifdef NIK_3D_LINES_NOT_TRIANGLES
void nikDrawFace3D(AMesh3D *m,unsigned short *f,int *dvs, unsigned int fnum)
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
  //debug_msg("nikDrawFace3D: ("); x1 debug_msg(","); y1 debug_msg(") ("); x2 debug_msg(","); y2 debug_msg(") ("); x3 debug_msg(","); y3 debug_msg(")\n");
  //debug_msg("face colors: "); f->fRed debug_msg(","); f->fGreen debug_msg(","); f->fBlue debug_msg("\n");
  //
  //if((z1>0)&&(z2>0)) {
  if(fi) clr=makeRGB24((int)(c1*fact*fi->fRed),(int)(c1*fact*fi->fGreen),(int)(c1*fact*fi->fBlue));
  nikSetColorLong(clr);
  //nikSetColorRGB(c1*fact*f->fRed,c1*fact*f->fGreen,c1*fact*f->fBlue);
  nikDrawLine(x1,y1,x2,y2);
  //}
  //if((z2>0)&&(z3>0)) {
  //nikSetColorRGB(c2*fact*f->fRed,c2*fact*f->fGreen,c2*fact*f->fBlue);
  nikDrawLine(x2,y2,x3,y3);
  //}
  //if((z3>0)&&(z1>0)) {
  //nikSetColorRGB(c3*fact*f->fRed,c3*fact*f->fGreen,c3*fact*f->fBlue);
  nikDrawLine(x3,y3,x1,y1);
  //}
}
#endif // NIK_3D_LINES_NOT_TRIANGLES
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


// Triangles, but no textures, version
#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
#ifndef NIK_3D_LINES_NOT_TRIANGLES
#ifdef NIK_3D_FORCE_NO_TEXTURES
void nikDrawFace3D(AMesh3D *m,unsigned short *f,int *dvs, unsigned int fnum)
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
  //debug_msg("nikDrawFace3D: ("); x1 debug_msg(","); y1 debug_msg(") ("); x2 debug_msg(","); y2 debug_msg(") ("); x3 debug_msg(","); y3 debug_msg(")\n");
  //debug_msg("face colors: "); f->fRed debug_msg(","); f->fGreen debug_msg(","); f->fBlue debug_msg("\n");
  //
  if(fi) clr=makeRGB24((int)(c1*fact*fi->fRed),(int)(c1*fact*fi->fGreen),(int)(c1*fact*fi->fBlue));
  nikSetColorLong(clr);
#ifdef NIK_OPENGLES
#else
  glVertex3f(x1,y1,0);
  glVertex3f(x2,y2,0);
  glVertex3f(x3,y3,0);
#endif // NIK_OPENGLES
}
#endif // NIK_3D_FORCE_NO_TEXTURES
#endif // NIK_3D_LINES_NOT_TRIANGLES
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


// Triangles with textures version
#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
#ifndef NIK_3D_LINES_NOT_TRIANGLES
#ifndef NIK_3D_FORCE_NO_TEXTURES
void nikDrawFace3D(AMesh3D *m,unsigned short *f,int *dvs, unsigned int fnum)
{
  float *tc=m->getTextCoords();
  //AFace3D **ftc=m->getFaceTextCoords();
  unsigned short *ftc=NULL;
  ftc=m->getFaceTextCoords();
  int x1,y1,x2,y2,x3,y3;
  float c1,c2,c3;
  //unsigned int clr;
  //float fact=255.0;
  c1=1.0;  c2=1.0;  c3=1.0;
  //x1=dvs[f->a].x;
  //y1=dvs[f->a].y;
  //x2=dvs[f->b].x;
  //y2=dvs[f->b].y;
  //x3=dvs[f->c].x;
  //y3=dvs[f->c].y;
  x1=dvs[f[fnum*3]*3];
  y1=dvs[f[fnum*3]*3+1];
  x2=dvs[f[fnum*3+1]*3];
  y2=dvs[f[fnum*3+1]*3+1];
  x3=dvs[f[fnum*3+2]*3];
  y3=dvs[f[fnum*3+2]*3+1];
  //
  //debug_msg("nikDrawFace3D: ("); x1 debug_msg(","); y1 debug_msg(") ("); x2 debug_msg(","); y2 debug_msg(") ("); x3 debug_msg(","); y3 debug_msg(")\n");
  //debug_msg("face colors: "); f->fRed debug_msg(","); f->fGreen debug_msg(","); f->fBlue debug_msg("\n");
  //
  unsigned int tcoff=0;
  nikSetColorRGB(255,255,255);
#ifdef NIK_OPENGLES
#else
  //if(ftc) { tcoff=ftc[fnum]->a; } else { tcoff=f->a; }
  if(ftc) { tcoff=ftc[fnum*3]; } else { tcoff=f[fnum*3]; }
  glTexCoord2f(tc[tcoff*2],tc[tcoff*2+1]);
  //glVertex3f(dvs[f->a].x,dvs[f->a].y,0);
  glVertex3f(x1,y1,0);
  //if(ftc) { tcoff=ftc[fnum]->b; } else { tcoff=f->b; }
  if(ftc) { tcoff=ftc[fnum*3+1]; } else { tcoff=f[fnum*3+1]; }
  glTexCoord2f(tc[tcoff*2],tc[tcoff*2+1]);
  //glVertex3f(dvs[f->b].x,dvs[f->b].y,0);
  glVertex3f(x2,y2,0);
  //if(ftc) { tcoff=ftc[fnum]->c; } else { tcoff=f->c; }
  if(ftc) { tcoff=ftc[fnum*3+2]; } else { tcoff=f[fnum*3+2]; }
  glTexCoord2f(tc[tcoff*2],tc[tcoff*2+1]);
  //glVertex3f(dvs[f->c].x,dvs[f->c].y,0);
  glVertex3f(x3,y3,0);
#endif // NIK_OPENGLES
}
#endif // NIK_3D_FORCE_NO_TEXTURES
#endif // NIK_3D_LINES_NOT_TRIANGLES
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


// No texture version
#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
#ifdef NIK_3D_FORCE_NO_TEXTURES
void nikDrawMesh3D(AMesh3D *m,float x,float y,float z)
{
  //debug_msg("nikDrawMesh3D Mesh has "); m->getNumFaces() debug_msg(" faces...\n");
  int *dvs=m->getDrawableMesh()->getDrawVerts();
  for(unsigned int t=0;t<m->getNumFaces();t++) {
    unsigned short *f=m->getFaces();
    //debug_msg("face: "); f->a debug_msg(","); f->b debug_msg(","); f->c debug_msg("\n");
    nikDrawFace3D(m,f,dvs,t);
  }
}
#endif // NIK_3D_FORCE_NO_TEXTURES
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


// Texture version
#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
#ifndef NIK_3D_FORCE_NO_TEXTURES
void nikDrawMesh3D(AMesh3D *m,float x,float y,float z)
{
  //debug_msg("nikDrawMesh3D Mesh has "); m->getNumFaces() debug_msg(" faces...\n");
  int *dvs=m->getDrawableMesh()->getDrawVerts();
  if(m->getNumTextures()) {
    //unsigned char *rawdata=NULL;
    //ATexture3D *tx=m->getTextures()[m->getCurTexture()];
    //if(!tx) { debug_msg("No tx!\n"); exit(EXIT_FAILURE); }
    //NIK_BITMAP *bm=tx->getBitmap();
    //if(!bm) { debug_msg("No bm!\n"); exit(EXIT_FAILURE); }
    //rawdata=bm->getPlane(0);
    //if(!rawdata) { debug_msg("No rawdata!\n"); exit(EXIT_FAILURE); }
    //glTexImage2D(GL_TEXTURE_2D,0,3,bm->getWidth(),bm->getHeight(),0,GL_RGB,GL_UNSIGNED_BYTE,rawdata);
    for(unsigned int t=0;t<m->getNumFaces();t++) {
      unsigned short *f=m->getFaces();
      //debug_msg("face: "); f->a debug_msg(","); f->b debug_msg(","); f->c debug_msg("\n");
      nikDrawFace3D(m,f,dvs,t);
    }
    //glBindTexture(GL_TEXTURE_2D,0);
  }
}
#endif // NIK_3D_FORCE_NO_TEXTURES
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


#ifdef NIK_OPENGL
#ifdef NICK_USE_NICK3D
void nikDrawScene3D(AScene3D *s)
{
  unsigned int nshapes=s->getNumMeshes();
  //debug_msg("nikDrawScene3D Scene has "); nshapes debug_msg(" meshes...\n");
  for(unsigned int t=0;t<nshapes;t++) {
    AMesh3D *m=s->getMeshes()[t];
    nikDrawMesh3D(m,0.0,0.0,0.0);
  }
}
#endif // NICK_USE_NICK3D
#endif // NIK_OPENGL


///////////////////////////////////////////////////////////////////////////////
//  Start of NIK_OPENGL section
////////////////////////////////////////////////////////////////////////////////


#endif // NIK_OPENGL

