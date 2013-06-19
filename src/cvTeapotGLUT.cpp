extern "C" {
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
}
#include "cvTeapotGLUT.hpp"

int cvTeapotGLUT::respond_to_key(unsigned char key, int x, int y)
{
  return 0;
}
void cvTeapotGLUT::render()
{
  glColor3d(1.0, 1.0, 1.0);
  glutWireTeapot(1.0);
}
