#ifndef __CV_cvTeapotGLUT_HEADER
#define __CV_cvTeapotGLUT_HEADER
#include "cv.hpp"

class cvTeapotGLUT : public cvSceneElement
{
public:
  virtual ~cvTeapotGLUT() { }
  int respond_to_key(unsigned char key, int x, int y);
  void render();
} ;

#endif // __CV_cvTeapotGLUT_HEADER
