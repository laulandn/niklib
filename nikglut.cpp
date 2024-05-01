

#include "niklib.h"


#ifdef NIK_GLUT
#ifndef NIK_OPENGL
#error "You must define NIK_OPENGL to use GLUT!"
#endif // NIK_OPENGL
#endif


#ifdef NIK_GLUT


///////////////////////////////////////////////////////////////////////////////
//  Start of NIK_GLUT section
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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


// NOTE: iPhone is 320x480
#define NIK_DEFAULT_WINDOW_WIDTH 1000
#define NIK_DEFAULT_WINDOW_HEIGHT 1000


BOOL nikAppInited=FALSE;
//
unsigned int nikActualWidth=0;
unsigned int nikActualHeight=0;
char *nikResourceFolder=NULL;


void (* nikTheKeyboardFunc)( unsigned char, int, int )=NULL;
void (* nikTheReshapeFunc)( int, int )=NULL;
void (* nikTheDisplayFunc)( void )=NULL;
void (* nikTheMouseFunc)( int, int, int, int )=NULL;
void (* nikTheMotionFunc)( int, int )=NULL;
void (* nikTheJoyMotionFunc)( int, int )=NULL;
void (* nikTheJoyButtonFunc)( int, int )=NULL;
void (* nikTheSpecialFunc)( int, int, int )=NULL;
void (* nikTheIdleFunc)( void )=NULL;


// Next two are internal use...
extern "C" {
BOOL nikAppInitGLUT(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);
BOOL nikAppInitOpenGL(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv);
};

BOOL nikAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  BOOL ret=FALSE;
  //
  // Pick a default size if we were asked to choose
  if(!theWidth) theWidth=NIK_DEFAULT_WINDOW_WIDTH;
  if(!theHeight) theHeight=NIK_DEFAULT_WINDOW_HEIGHT;
  nikActualWidth=theWidth;
  nikActualHeight=theHeight;
  //
  ret=nikAppInitGLUT(theWidth,theHeight,theDepth,argc,argv);
  if(ret) ret=nikAppInitOpenGL(theWidth,theHeight,theDepth,argc,argv);
  return ret;
}


BOOL nikAppInitGLUT(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  debug_msg("nikAppInitGLUT(...)\n");
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
  debug_msg("(Going to call glutInit...)\n");
  glutInit(&argc, argv);
  debug_msg("(Going to call glutInitDisplayMode...)\n");
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  debug_msg("(Going to call glutInitWindowSize...)\n");
  glutInitWindowSize(theWidth, theHeight);
  debug_msg("(Going to call glutCreateWindow...)\n");
  glutCreateWindow(argv[0]);
  {
    /* Check that renderer has the GL_EXT_texture_object
     extension or supports OpenGL 1.1 */
    char *version = (char *) glGetString(GL_VERSION);
    if(!version) { debug_msg("glGetString failed!\n"); return FALSE; }
    double gl_vers = strtod(version, NULL);
    if ((gl_vers < 1.1) && !glutExtensionSupported("GL_EXT_texture_object")) {
      fprintf(stderr, "GL version must be 1.1 or greater.\n");
      return FALSE;  // Just exit if the support is not there.
    }
  }
  //
  nikAppInited=TRUE;
  debug_msg("nikAppInit done.\n");
  return TRUE;
}


void nikKeyboardFunc( void (* callback)( unsigned char, int, int ) )
{
  debug_msg("nikKeyboardFunc called...\n");
  nikTheKeyboardFunc=callback;
  glutKeyboardFunc(callback);
  //extern void APIENTRY glutKeyboardUpFunc(void (*func)(unsigned char key, int x, int y));
}


void nikReshapeFunc( void (* callback)( int, int ) )
{
  debug_msg("nikReshapeFunc called...\n");
  nikTheReshapeFunc=callback;
  glutReshapeFunc(callback);
}


void nikDisplayFunc( void (* callback)( void ) )
{
  debug_msg("nikDisplayFunc called...\n");
  nikTheDisplayFunc=callback;
  glutDisplayFunc(callback);
}


void nikMouseFunc( void (* callback)( int, int, int, int ) )
{
  debug_msg("nikMouseFunc called...\n");
  nikTheMouseFunc=callback;
  glutMouseFunc(callback);
}


void nikMotionFunc( void (* callback)( int, int ) )
{
  debug_msg("nikMotionFunc called...\n");
  nikTheMotionFunc=callback;
  glutMotionFunc(callback);
}


void nikJoyButtonFunc( void (* callback)( int, int ) )
{
  debug_msg("nikJoyButtonFunc called...\n");
  nikTheJoyButtonFunc=callback;
  //extern void APIENTRY glutJoystickFunc(void (*func)(unsigned int buttonMask, int x, int y, int z), int pollInterval);
}


void nikJoyMotionFunc( void (* callback)( int, int ) )
{
  debug_msg("nicJoykMotionFunc called...\n");
  nikTheJoyMotionFunc=callback;
  //extern void APIENTRY glutJoystickFunc(void (*func)(unsigned int buttonMask, int x, int y, int z), int pollInterval);
}


void nikSpecialFunc( void (* callback)( int, int, int ) )
{
  debug_msg("nikSpecialFunc called...\n");
  nikTheSpecialFunc=callback;
  glutSpecialFunc(callback);
}


void nikIdleFunc( void (* callback)( void ) )
{
  debug_msg("nikIdleFunc called...\n");
  nikTheIdleFunc=callback;
  glutIdleFunc(callback);
}


void nikRunEventLoop()
{
  debug_msg("nikRunEventLoop(...)\n");
  debug_msg("Going to call glutMainLoop...\n");
  glutMainLoop();
  debug_msg("Back from glutMainLoop.\n");
}


void nikExitFromRunLoop()
{
  //nikExitNextLoop=TRUE;
  debug_msg("nikExitFromRunLoop not implemented!\n");
  exit(EXIT_FAILURE);
}


void nikLockDisplay()
{
  //debug_msg("nikLockDisplay()...\n");
#ifdef NIK_3D_LINES_NOT_TRIANGLES
  glBegin(GL_LINES);
#else
  glBegin(GL_TRIANGLES);
#endif
}


void nikUnlockDisplay()
{
  //debug_msg("nikUnlockDisplay()...\n");
  glEnd();
}


void nikFlipDisplay()
{
  //debug_msg("nikFlipDisplay()...\n");
  glFlush();
  glutSwapBuffers();
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


/*
void nikNotifyOfResize(int w, int h)
{
  debug_msg("nikNotifyOfResize("); w debug_msg(","); h debug_msg(")\n");
  nikActualWidth=w;  nikActualHeight=h;
}
*/


///////////////////////////////////////////////////////////////////////////////
//  End of NIK_GLUT section
////////////////////////////////////////////////////////////////////////////////


#endif // NIK_GLUT


