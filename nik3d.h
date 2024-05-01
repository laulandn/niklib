#ifndef NIK3D_H
#define NIK3D_H


#define N3D_TEST_EVERYTHING 1


#include "../nick3d/AWorld3D.h"
#include "../nick3d/AThing3D.h"
#include "../nick3d/AScene3D.h"
#include "../nick3d/ATexture3D.h"
#include "../nick3d/AMesh3D.h"

#include "../decode3d/ASmartMeshDecoder.h"
#include "../decode3d/ASmartSceneDecoder.h"
#include "../decode3d/AMeshDecoder.h"
#include "../decode3d/ASceneDecoder.h"


#include "../../more_src/nlib/nlibdbug.h"


void nikDrawFace3D(AMesh3D *m,unsigned short *f, int *dvs,unsigned int fnum);
void nikDrawMesh3D(AMesh3D *m,float x,float y,float z);
void nikDrawScene3D(AScene3D *s);
void *nikBindThisTexture3D(ATexture3D *tx);
BOOL nikPrepareMesh3D(AMesh3D *m);
BOOL nikPrepareScene3D(AScene3D *s);


#endif // NIK3D_H
