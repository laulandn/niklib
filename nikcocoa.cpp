

#include "niklib.h"


///////////////////////////////////////////////////////////////////////////////
//  Start of NIK_COCOA section
////////////////////////////////////////////////////////////////////////////////


#ifdef NIK_COCOA


unsigned int nikActualWidth=0;
unsigned int nikActualHeight=0;
unsigned int nikActualDepth=0;


BOOL nikAppInit(unsigned int theWidth,unsigned int theHeight,unsigned theDepth,int argc,char **argv)
{
  return FALSE;
}


///////////////////////////////////////////////////////////////////////////////
//  Start of event handling
////////////////////////////////////////////////////////////////////////////////


void nikRunEventLoop()
{
}


void nikExitFromRunLoop()
{
}


void nikKeyboardFunc( void (* callback)( unsigned char, int, int ) )
{
}


void nikReshapeFunc( void (* callback)( int, int ) )
{
}


void nikDisplayFunc( void (* callback)( void ) )
{
}


void nikMouseFunc( void (* callback)( int, int, int, int ) )
{
}


void nikMotionFunc( void (* callback)( int, int ) )
{
}


void nikJoyButtonFunc( void (* callback)( int, int ) )
{
}


void nikJoyMotionFunc( void (* callback)( int, int ) )
{
}


void nikSpecialFunc( void (* callback)( int, int, int ) )
{
}


void nikIdleFunc( void (* callback)( void ) )
{
}


///////////////////////////////////////////////////////////////////////////////
//  End of event handling
////////////////////////////////////////////////////////////////////////////////

void nikLockDisplay()
{
}


void nikUnlockDisplay()
{
}


void nikFlipDisplay()
{
}


void nikNotifyOfResize(int w, int h)
{
}


void nikDrawImage(NIK_DRAWABLE_BITMAP *tbm,unsigned int x,unsigned int y)
{
}


/*
NIK_PALETTE *nikGetCurPalette()
{
}
*/


NIK_DRAWABLE_BITMAP *nikLoadImage(char *name)
{
  return NULL;
}


void nikDrawLine(unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
}


void nikWritePixel(unsigned int x,unsigned int y)
{
}


unsigned long nikReadPixel(unsigned int x,unsigned int y)
{
  return 0;
}


void nikCopyToFrameBuffer(NIK_DRAWABLE_BITMAP *tbm)
{
}


void nikCopyBlock(NIK_DRAWABLE_BITMAP *tbm,unsigned int sx,unsigned int sy,unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
}


void nikClearBlock(unsigned int dx,unsigned int dy,unsigned int bw,unsigned int bh)
{
}


void nikDrawString(const char *st,unsigned int x,unsigned int y)
{
}


void nikClearFrameBuffer()
{
}


void nikSetColorRGB(unsigned int r,unsigned int g,unsigned int b)
{
}


void nikSetColorLong(unsigned long c)
{
}


#endif // NIK_COCOA

