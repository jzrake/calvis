#include "cv.hpp"
#include "cvTeapotGLUT.hpp"
#include "cvRenderWindowGLUT.hpp"
#include "cvRectilinearMesh.hpp"
#define ESCAPE_KEY 27

int main(int argc, char **argv)
{
  cvRenderWindow *RenderWindow = new cvRenderWindowGLUT;
  cvTeapotGLUT *teapot = new cvTeapotGLUT;
  cvRectilinearMesh *mesh = new cvRectilinearMesh;

  mesh->set_file_name("chkpt.0001.h5");
  mesh->set_group_name("prim");
  mesh->add_component("rho");
  mesh->add_component("pre");
  mesh->add_component("vx");
  mesh->load();

  RenderWindow->add_element(teapot);
  RenderWindow->render();
  return 0;
}
