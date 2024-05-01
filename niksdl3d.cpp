

#include "niklib.h"


#ifdef NIK_USE_NIK3D


#ifdef NIK_SDL
#ifndef NIK_OPENGL
#ifndef NIK_SDL_DRAWING
#error "You must define either NIK_OPENGL or NIK_SDL_DRAWING!"
#endif // NIK_SDL_DRAWING
#endif // NIK_OPENGL
#endif


#ifdef NIK_SDL


#define NIK_JOYSTICK_SUPPORT 1
#define NIK_ALWAYS_SETUP_FONT 1
#define NIK_DOUBLE_BUFFER 1


#define NIK_DEFAULT_FONT "font.bmp"


#include "nikdraw.h"
#include "nik3d.h"


#include <SDL.h>


#ifdef NIK_OPENGL
// NOET: No GLUT for opengles!
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


void *nikBindThisTexture3D(ATexture3D *tx)
{
  debug_msg("nikBindThisTexture3D not implemented!\n");
  return NULL;
}


BOOL nikPrepareMesh3D(AMesh3D *m)
{
  debug_msg("nikPrepareMesh3D(...)\n");
  for(unsigned int t=0;t<m->getNumTextures();t++) {
    ATexture3D *tx=m->getTextures()[t];
    nikBindThisTexture3D(tx);
    tx->setBinding(nikBindThisTexture3D(tx));
  }
  return TRUE;
}


BOOL nikPrepareScene3D(AScene3D *s)
{
  debug_msg("nikPrepareScene3D(...)\n");
  for(unsigned int t=0;t<s->getNumMeshes();t++) {
    nikPrepareMesh3D(s->getMeshes()[t]);
  }
  return TRUE;
}


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


void nikDrawScene3D(AScene3D *s)
{
  unsigned int nshapes=s->getNumMeshes();
  //debug_msg("nikDrawScene3D Scene has "); nshapes debug_msg(" meshes...\n");
  for(unsigned int t=0;t<nshapes;t++) {
    AMesh3D *m=s->getMeshes()[t];
    nikDrawMesh3D(m,0.0,0.0,0.0);
  }
}


#endif // NIK_SDL


#endif // NIK_USE_NIK3D

