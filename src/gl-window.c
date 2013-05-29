

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "calvis.h"
#define ESCAPE_KEY 27

int WindowWidth = 768;
int WindowHeight = 768;

double RotationX = 0.0;
double RotationY = 0.0;
double TranslateZ = -2.0;

static void auto_scale(double *raw_data, int N, int mem, int nm, double data_cut[2]);
static double scale(double z);
static void get_rgb(double val, GLfloat rgb[3]);
static int log_scale = 0;
static int colorbar_id = 1;
static int user_cuts = 0;

static ImagePlane *ImagePlaneObjects[16];
static int ImagePlaneObjectCount = 0;
static RenderScene *RenderSceneCurrent = NULL;

struct RenderScene
{
  int (*KeyboardFunc)(unsigned char key, int x, int y);
} ;

struct ImagePlane
{
  GLuint id;
  int complete;
  GLfloat X0[3]; // corner positions
  GLfloat X1[3];
  GLfloat X2[3];
  GLfloat X3[3];
} ;

int cv_setcurrent_scene(RenderScene *S)
{
  RenderSceneCurrent = S;
  return 0;
}

void IdleFunc()
{
  glutPostRedisplay();
}

void DisplayFunc()
{
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();

  /* Camera location */
  glTranslated(0.0,-1.0, 0.0);
  glTranslated(0.0, 0.0, TranslateZ);

  glRotated(RotationX, 1.0, 0.0, 0.0);
  glRotated(RotationY, 0.0, 1.0, 0.0);
  glRotated(-90.0, 1.0, 0.0, 0.0);

  for (int i=0; i<ImagePlaneObjectCount; ++i) {
    ImagePlane *I = ImagePlaneObjects[i];
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, I->id);
    glBegin(GL_QUADS);

    glTexCoord2f(0.f, 0.f); glVertex3fv(I->X0);
    glTexCoord2f(1.f, 0.f); glVertex3fv(I->X1);
    glTexCoord2f(1.f, 1.f); glVertex3fv(I->X2);
    glTexCoord2f(0.f, 1.f); glVertex3fv(I->X3);

    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
  }

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
  /*
  if (RenderSceneCurrent->KeyboardFunc) {
    if (RenderSceneCurrent->KeyboardFunc(key, x, y)) return;
  }
  */
  switch (key) {
  case ESCAPE_KEY:
    exit(0);
  case '-': TranslateZ *= 1.1; break;
  case '=': TranslateZ /= 1.1; break;
  }
}

void SpecialFunc(int key, int x, int y)
{
  double a = 4.0;
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
  glutCreateWindow("NYU CAL-Vis");
  glutDisplayFunc(DisplayFunc);
  glutIdleFunc(IdleFunc);
  glutReshapeFunc(ReshapeFunc);
  glutKeyboardFunc(KeyboardFunc);
  glutSpecialFunc(SpecialFunc);

  cv_build_scene();

  glutMainLoop();
  return 0;
}


RenderScene *cv_renderscene_new()
{
  RenderScene *S = (RenderScene*) malloc(sizeof(RenderScene));
  S->KeyboardFunc = NULL;
  return S;
}
int cv_renderscene_del(RenderScene *S)
{
  free(S);
  return 0;
}

ImagePlane *cv_imageplane_new()
{
  ImagePlane *I = (ImagePlane*) malloc(sizeof(ImagePlane));
  glGenTextures(1, &I->id);
  I->complete = 0;
  ImagePlaneObjects[ImagePlaneObjectCount++] = I;
  return I;
}
int cv_imageplane_del(ImagePlane *I)
{
  glDeleteTextures(1, &I->id);
  free(I);
  --ImagePlaneObjectCount;
  return 0;
}



int cv_imageplane_from3d(ImagePlane *I, cow_dfield *f, int mem, int ax, int ind)
{
  cow_domain *d = cow_dfield_getdomain(f);
  int ng = cow_domain_getguard(d);
  int nd = cow_domain_getndim(d);
  int nx = cow_domain_getsize(d, 0);
  int ny = cow_domain_getsize(d, 1);
  int nz = cow_domain_getsize(d, 2);
  int nm = cow_dfield_getnmembers(f);
  int sx = cow_dfield_getstride(f, 0);
  int sy = cow_dfield_getstride(f, 1);
  int sz = cow_dfield_getstride(f, 2);
  int na[3] = {nx, ny, nz};
  int I0[3] = {ng, ng, ng};
  int I1[3] = {ng+nx, ng+ny, ng+nz};

  if (nd != 3) {
    cv_error("domain needs to be 3d");
    return 1;
  }
  if (ax < 0 || ax >= 3) {
    cv_error("axis must be 0, 1, or 2");
    return 1;
  }
  if (mem < 0 || mem >= nm) {
    cv_error("data member out of range");
    return 1;
  }
  if (ind < 0 || ind >= na[ax]+ng) {
    cv_error("index out of range");
    return 1;
  }

  I0[ax] = ng + ind;
  I1[ax] = ng + ind + 1;

  int ntotimg = (I1[0] - I0[0]) * (I1[1] - I0[1]) * (I1[2] - I0[2]);
  int ntotsrc = cow_domain_getnumlocalzonesincguard(d, COW_ALL_DIMS);
  double *raw_data = cow_dfield_getdatabuffer(f);
  GLfloat *texture_data = (GLfloat*) malloc(ntotimg * 3 * sizeof(GLfloat));
  int i,j,k,m;
  int Nx, Ny;
  int nimage = 0;
  double data_cut[2];
  double min, max, z;

  if (!user_cuts) {
    auto_scale(raw_data, ntotsrc, mem, nm, data_cut);
  }
  min = data_cut[0];
  max = data_cut[1];

  printf("data cuts [%4.3f, %4.3f] %s-scaled\n", min, max,
	 log_scale ? "log" : "linearly");

  switch (ax) {
  case 0:
    for (k=I0[2]; k<I1[2]; ++k) {
      for (j=I0[1]; j<I1[1]; ++j) {
	for (i=I0[0]; i<I1[0]; ++i) {
	  m = sx*i + sy*j + sz*k + mem;
	  z = (scale(raw_data[m]) - min) / (max - min);
	  get_rgb(z, &texture_data[3*nimage]);
	  ++nimage;
	}
      }
    }
    Nx=ny;
    Ny=nz;
    I->X0[0] = cow_domain_positionatindex(d, 0, I0[0]);
    I->X1[0] = cow_domain_positionatindex(d, 0, I0[0]);
    I->X2[0] = cow_domain_positionatindex(d, 0, I0[0]);
    I->X3[0] = cow_domain_positionatindex(d, 0, I0[0]);
    I->X0[1] = cow_domain_positionatindex(d, 1, I0[1]);
    I->X1[1] = cow_domain_positionatindex(d, 1, I1[1]);
    I->X2[1] = cow_domain_positionatindex(d, 1, I1[1]);
    I->X3[1] = cow_domain_positionatindex(d, 1, I0[1]);
    I->X0[2] = cow_domain_positionatindex(d, 2, I0[2]);
    I->X1[2] = cow_domain_positionatindex(d, 2, I0[2]);
    I->X2[2] = cow_domain_positionatindex(d, 2, I1[2]);
    I->X3[2] = cow_domain_positionatindex(d, 2, I1[2]);
    break;
  case 1:
    for (i=I0[0]; i<I1[0]; ++i) {
      for (k=I0[2]; k<I1[2]; ++k) {
	for (j=I0[1]; j<I1[1]; ++j) {
	  m = sx*i + sy*j + sz*k + mem;
	  z = (scale(raw_data[m]) - min) / (max - min);
	  get_rgb(z, &texture_data[3*nimage]);
	  ++nimage;
	}
      }
    }
    Nx=nz;
    Ny=nx;
    I->X0[0] = cow_domain_positionatindex(d, 0, I0[0]);
    I->X1[0] = cow_domain_positionatindex(d, 0, I0[0]);
    I->X2[0] = cow_domain_positionatindex(d, 0, I1[0]);
    I->X3[0] = cow_domain_positionatindex(d, 0, I1[0]);
    I->X0[1] = cow_domain_positionatindex(d, 1, I0[1]);
    I->X1[1] = cow_domain_positionatindex(d, 1, I0[1]);
    I->X2[1] = cow_domain_positionatindex(d, 1, I0[1]);
    I->X3[1] = cow_domain_positionatindex(d, 1, I0[1]);
    I->X0[2] = cow_domain_positionatindex(d, 2, I0[2]);
    I->X1[2] = cow_domain_positionatindex(d, 2, I1[2]);
    I->X2[2] = cow_domain_positionatindex(d, 2, I1[2]);
    I->X3[2] = cow_domain_positionatindex(d, 2, I0[2]);
    break;
  case 2:
    for (j=I0[1]; j<I1[1]; ++j) {
      for (i=I0[0]; i<I1[0]; ++i) {
	for (k=I0[2]; k<I1[2]; ++k) {
	  m = sx*i + sy*j + sz*k + mem;
	  z = (scale(raw_data[m]) - min) / (max - min);
	  get_rgb(z, &texture_data[3*nimage]);
	  ++nimage;
	}
      }
    }
    Nx=nx;
    Ny=ny;
    I->X0[0] = cow_domain_positionatindex(d, 0, I0[0]);
    I->X1[0] = cow_domain_positionatindex(d, 0, I1[0]);
    I->X2[0] = cow_domain_positionatindex(d, 0, I1[0]);
    I->X3[0] = cow_domain_positionatindex(d, 0, I0[0]);
    I->X0[1] = cow_domain_positionatindex(d, 1, I0[1]);
    I->X1[1] = cow_domain_positionatindex(d, 1, I0[1]);
    I->X2[1] = cow_domain_positionatindex(d, 1, I1[1]);
    I->X3[1] = cow_domain_positionatindex(d, 1, I1[1]);
    I->X0[2] = cow_domain_positionatindex(d, 2, I0[2]);
    I->X1[2] = cow_domain_positionatindex(d, 2, I0[2]);
    I->X2[2] = cow_domain_positionatindex(d, 2, I0[2]);
    I->X3[2] = cow_domain_positionatindex(d, 2, I0[2]);
    break;
  }

  glBindTexture(GL_TEXTURE_2D, I->id);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, Nx, Ny, 0, GL_RGB, GL_FLOAT, texture_data);
  glBindTexture(GL_TEXTURE_2D, 0);

  free(texture_data);
  return 0;
}


void auto_scale(double *raw_data, int N, int mem, int nm, double data_cut[2])
{
  int m;
  double z;

  data_cut[0] = scale(raw_data[mem]);
  data_cut[1] = scale(raw_data[mem]);

  for (m=0; m<N; ++m) {
    z = scale(raw_data[nm*m + mem]);
    if (z < data_cut[0]) data_cut[0] = z;
    if (z > data_cut[1]) data_cut[1] = z;
  }
}



double scale(double z)
{
  if (log_scale) {
    return log10(z);
  }
  else {
    return z;
  }
}

void get_rgb(double val, GLfloat rgb[3])
{
  double rrr, ggg, bbb;

  if( colorbar_id == 0 ){
    double nexp = 8.0;
    rrr = exp(-nexp*pow(val-5./6.,2.0)) + .25*exp(-nexp*pow(val+1./6.,2.0));
    ggg = exp(-nexp*pow(val-3./6.,2.0));
    bbb = exp(-nexp*pow(val-1./6.,2.0)) + .25*exp(-nexp*pow(val-7./6.,2.0));
  }else if(colorbar_id == 1){
    if( val < .1 ){
      bbb = 4.*(val+.15);
      ggg = 0.0;
      rrr = 0.0;
    }else if( val < .35){
      bbb = 1.0;
      ggg = 4.*(val-.1);
      rrr = 0.0;
    }else if( val < .6 ){
      bbb = 4.*(.6-val);
      ggg = 1.;
      rrr = 4.*(val-.35);
    }else if( val < .85){
      bbb = 0.0;
      ggg = 4.*(.85-val);
      rrr = 1.;
    }else{
      bbb = 0.0;
      ggg = 0.0;
      rrr = 4.*(1.1-val);
    }
  }else if(colorbar_id == 2){
    rrr = 2.*val;
    ggg = 1.2*val;
    bbb = .8*val;
  }else if(colorbar_id == 3){
    double gam = .8;
    double Amp;
    double r0,g0,b0;
    double hi,lo,x1,x2,x3,x4;
    hi = .8;
    lo = .1;
    if( val > hi ) Amp = .3 + .7*(1.-val)/(1.-hi);
    else if( val < lo ) Amp = .3 + .7*(val)/(lo);
    else Amp = 1.0;

    x1 = .5;
    x2 = .325;
    x3 = .15;
    x4 = 0.;

    if( val > x1 )      r0 = 1.;
    else if( val > x2 ) r0 = (val-x2)/(x1-x2);
    else if( val > x3 ) r0 = 0.;
    else if( val > x4 ) r0 = (val-x3)/(x4-x3);
    else                r0 = 1.;

    x1 = .6625;
    x2 = .5;
    x3 = .275;
    x4 = .15;

    if( val > x1 )      g0 = 0.;
    else if( val > x2 ) g0 = (val-x1)/(x2-x1);
    else if( val > x3 ) g0 = 1.;
    else if( val > x4 ) g0 = (val-x4)/(x3-x4);
    else                g0 = 0.;

    x1 = .325;
    x2 = .275;

    if( val > x1 )      b0 = 0.;
    else if( val > x2 ) b0 = (val-x1)/(x2-x1);
    else                b0 = 1.;

    rrr = pow(Amp*r0,gam);
    ggg = pow(Amp*g0,gam);
    bbb = pow(Amp*b0,gam);
  }else if(colorbar_id == 4){
    if( val < .1 ){
      bbb = 4.*(val+.125);
      ggg = 0.0;
      rrr = 0.0;
    }else if( val < .375){
      bbb = 1.0;
      ggg = 4.*(val-.125);
      rrr = 0.0;
    }else if( val < .625 ){
      bbb = 4.*(.625-val);
      rrr = 4.*(val-.375);
      ggg = bbb;
      if( rrr > bbb ) ggg = rrr;
    }else if( val < .875){
      bbb = 0.0;
      ggg = 4.*(.875-val);
      rrr = 1.;
    }else{
      bbb = 0.0;
      ggg = 0.0;
      rrr = 4.*(1.125-val);
    }
  }else if(colorbar_id == 5){
    rrr = val;
    ggg = val;
    bbb = val;
  }else{
    rrr = 1.0;
    ggg = 1.0;
    bbb = 1.0;
  }

  rgb[0] = rrr;
  rgb[1] = ggg;
  rgb[2] = bbb;
}
