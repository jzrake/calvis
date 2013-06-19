#ifndef __CV_cvRenderWindowGLUT_HEADER
#define __CV_cvRenderWindowGLUT_HEADER
#include <list>
#include "cv.hpp"

class cvRenderWindowGLUT : public cvRenderWindow
{
private:
  std::list<cvSceneElement*> ElementList;
  double TranslateZ;
  double RotationX;
  double RotationY;
public:
  cvRenderWindowGLUT();
  virtual ~cvRenderWindowGLUT();
  int DisplayFunc();
  int IdleFunc();
  int ReshapeFunc(int Width, int Height);
  int KeyboardFunc(unsigned char key, int x, int y);
  int SpecialFunc(int key, int x, int y);
  void add_element(cvSceneElement *e);
  void render();
} ;

#endif // __CV_cvRenderWindowGLUT_HEADER
