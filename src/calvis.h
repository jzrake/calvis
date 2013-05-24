


#ifndef CALVIS_HEADER_INCLUDED
#define CALVIS_HEADER_INCLUDED

#include "cow.h"

typedef struct ImagePlane ImagePlane;

ImagePlane *cv_imageplane_new();
int cv_imageplane_del(ImagePlane *I);
int cv_imageplane_from3d(ImagePlane *I, cow_dfield *f, int mem, int ax, int ind);

int cv_build_scene();
int cv_launch(int argc, char **argv);

#define cv_error(msg) fprintf(stderr, "[ERROR] %s: %s\n", __FUNCTION__, msg);

#endif // CALVIS_HEADER_INCLUDED
