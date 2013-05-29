
extern "C" {
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
}
#include "cv.hpp"
#define ESCAPE_KEY 27

static cvRenderWindow *RenderWindow = NULL;


static void DisplayFunc()
{
  RenderWindow->DisplayFunc();
}
static void IdleFunc()
{
  RenderWindow->IdleFunc();
}
static void ReshapeFunc(int width, int height)
{
  RenderWindow->ReshapeFunc(width, height);
}
static void KeyboardFunc(unsigned char key, int x, int y)
{
  RenderWindow->KeyboardFunc(key, x, y);
}
static void SpecialFunc(int key, int x, int y)
{
  RenderWindow->SpecialFunc(key, x, y);
}

int main(int argc, char **argv)
{
  RenderWindow = new cvRenderWindow;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(768, 768);
  glutInitWindowPosition(512, 0);
  glutCreateWindow("NYU CAL-Vis");
  glutDisplayFunc(DisplayFunc);
  glutIdleFunc(IdleFunc);
  glutReshapeFunc(ReshapeFunc);
  glutKeyboardFunc(KeyboardFunc);
  glutSpecialFunc(SpecialFunc);

  cvTeapot *teapot = RenderWindow->new_object<cvTeapot>();

  glutMainLoop();
  return 0;
}

cvRenderWindow::cvRenderWindow()
  : TranslateZ(-2.0),
    RotationX(0.0),
    RotationY(0.0)
{ }

cvRenderWindow::~cvRenderWindow()
{
  std::list<cvRenderableObject*>::iterator it = ObjectList.begin();
  while (it != ObjectList.end()) {
    delete *it;
    ++it;
  }
}

int cvRenderWindow::DisplayFunc()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  glTranslated(0.0,-1.0, 0.0);
  glTranslated(0.0, 0.0, TranslateZ);

  glRotated(RotationX, 1.0, 0.0, 0.0);
  glRotated(RotationY, 0.0, 1.0, 0.0);
  glRotated(-90.0, 1.0, 0.0, 0.0);

  std::list<cvRenderableObject*>::iterator it = ObjectList.begin();
  while (it != ObjectList.end()) {
    cvKeyboardResponsiveObject *keyb = dynamic_cast<cvKeyboardResponsiveObject*>(*it);
    if (keyb) {
      //      printf("it's responsive\n");
    }
    (*it)->render();
    ++it;
  }

  glutSwapBuffers();

  return 0;
}

int cvRenderWindow::IdleFunc()
{
  glutPostRedisplay();
  return 0;
}

int cvRenderWindow::ReshapeFunc(int width, int height)
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

int cvRenderWindow::KeyboardFunc(unsigned char key, int x, int y)
{
  switch (key) {
  case ESCAPE_KEY:
    exit(0);
  case '-': TranslateZ *= 1.1; break;
  case '=': TranslateZ /= 1.1; break;
  }
  return 0;
}

int cvRenderWindow::SpecialFunc(int key, int x, int y)
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


int cvTeapot::respond_to_key(unsigned char key, int x, int y)
{
  return 0;
}

void cvTeapot::render()
{
  glColor3d(1.0, 1.0, 1.0);
  glutWireTeapot(1.0);
}
