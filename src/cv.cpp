#include "cv.hpp"
#include "cvTeapotGLUT.hpp"
#include "cvRenderWindowGLUT.hpp"
#define ESCAPE_KEY 27

int main(int argc, char **argv)
{
  cvRenderWindow *RenderWindow = new cvRenderWindowGLUT;
  cvTeapotGLUT *teapot = new cvTeapotGLUT;
  RenderWindow->add_element(teapot);
  RenderWindow->render();
  return 0;
}
