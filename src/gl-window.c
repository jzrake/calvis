

#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define ESCAPE_KEY 27

int WindowWidth = 768;
int WindowHeight = 768;
int WindowId;

double RotationX = 0.0;
double RotationY = 0.0;
double TranslateZ = -5.0;

void IdleFunc()
{
  glutPostRedisplay();
}

void DisplayFunc()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  glColor3d(1.0, 0.0, 0.0);
  glTranslated(0.0, 0.0, TranslateZ);

  glRotated(RotationX, 1.0, 0.0, 0.0);
  glRotated(RotationY, 0.0, 1.0, 0.0);
  glutWireTeapot(1.0);

  glutSwapBuffers();
}

void ReshapeFunc(int Width, int Height)
{
  if (Height == 0) {
    Height = 1;
  }
  glViewport(0, 0, Width, Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
  switch (key) {
  case ESCAPE_KEY:
    glutDestroyWindow(WindowId);
    exit(0);
  case '-': TranslateZ *= 1.1; break;
  case '=': TranslateZ /= 1.1; break;
  }

}

void SpecialFunc(int key, int x, int y)
{
  double a = 2.0;
  switch (key) {
  case GLUT_KEY_RIGHT: RotationY -= a; break;
  case GLUT_KEY_LEFT:  RotationY += a; break;
  case GLUT_KEY_UP:    RotationX -= a; break;
  case GLUT_KEY_DOWN:  RotationX += a; break;
  }
  glutPostRedisplay();
}

int cv_launch(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  glutInitWindowSize(WindowWidth, WindowHeight);
  glutInitWindowPosition(512, 0);
  WindowId = glutCreateWindow("NYU CAL-Vis");

  glutDisplayFunc(DisplayFunc);
  glutIdleFunc(IdleFunc);
  glutReshapeFunc(ReshapeFunc);
  glutKeyboardFunc(KeyboardFunc);
  glutSpecialFunc(SpecialFunc);

  glutMainLoop();

  return 0;
}

