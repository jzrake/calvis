#include "cvRenderWindowGLUT.hpp"
extern "C" {
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
}
#define ESCAPE_KEY 27
static cvRenderWindowGLUT *RenderWindow = NULL;

static void displayFunc()
{
  RenderWindow->DisplayFunc();
}
static void idleFunc()
{
  RenderWindow->IdleFunc();
}
static void reshapeFunc(int width, int height)
{
  RenderWindow->ReshapeFunc(width, height);
}
static void keyboardFunc(unsigned char key, int x, int y)
{
  RenderWindow->KeyboardFunc(key, x, y);
}
static void specialFunc(int key, int x, int y)
{
  RenderWindow->SpecialFunc(key, x, y);
}

cvRenderWindowGLUT::cvRenderWindowGLUT()
  : TranslateZ(-2.0),
    RotationX(0.0),
    RotationY(0.0)
{
  int argc = 0;
  char **argv = NULL;
  RenderWindow = this;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(768, 768);
  glutInitWindowPosition(512, 0);
  glutCreateWindow("NYU CAL-Vis");
  glutDisplayFunc(displayFunc);
  glutIdleFunc(idleFunc);
  glutReshapeFunc(reshapeFunc);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(specialFunc);
}
cvRenderWindowGLUT::~cvRenderWindowGLUT() { }
int cvRenderWindowGLUT::DisplayFunc()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  glTranslated(0.0,-1.0, 0.0);
  glTranslated(0.0, 0.0, TranslateZ);

  glRotated(RotationX, 1.0, 0.0, 0.0);
  glRotated(RotationY, 0.0, 1.0, 0.0);
  glRotated(-90.0, 1.0, 0.0, 0.0);

  std::list<cvSceneElement*>::iterator it = ElementList.begin();
  while (it != ElementList.end()) {
    (*it)->render();
    ++it;
  }

  glutSwapBuffers();

  return 0;
}
int cvRenderWindowGLUT::IdleFunc()
{
  glutPostRedisplay();
  return 0;
}
int cvRenderWindowGLUT::ReshapeFunc(int width, int height)
{
  if (height == 0) {
    height = 1;
  }
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  return 0;
}
int cvRenderWindowGLUT::KeyboardFunc(unsigned char key, int x, int y)
{
  switch (key) {
  case ESCAPE_KEY:
    exit(0);
  case '-': TranslateZ *= 1.1; break;
  case '=': TranslateZ /= 1.1; break;
  }
  return 0;
}
int cvRenderWindowGLUT::SpecialFunc(int key, int x, int y)
{
  double a = 4.0;
  switch (key) {
  case GLUT_KEY_RIGHT: RotationY -= a; break;
  case GLUT_KEY_LEFT:  RotationY += a; break;
  case GLUT_KEY_UP:    RotationX -= a; break;
  case GLUT_KEY_DOWN:  RotationX += a; break;
  }
  glutPostRedisplay();
  return 0;
}
void cvRenderWindowGLUT::render()
{
  glutMainLoop();
}
void cvRenderWindowGLUT::add_element(cvSceneElement *e)
{
  ElementList.push_back(e);
}
