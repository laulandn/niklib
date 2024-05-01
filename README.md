Very small, trivial cross platform application framework 
===============

Work in progress...

Does not use GNU autotools for configure, instead goofy shell script...sorry!

Meant to be in plain C, but still has some C++isms.

Opens a window, event handling, simple drawing, and that's about it.

Include trivial raw bitmap/framebuffer drawing routines.
Include some basic 3d rendering support.

Builds on several backends including my own guilib, SDL, OpenGL, GLUT, and plain text.
(But some are just skeletons).

Test programs...

+ nlines: Draw rainbow moire pattern.
+ nv: View an image (uses other library for image reading).
+ nwire: Draw simple 3d wireframe (uses other library for 3d support).

