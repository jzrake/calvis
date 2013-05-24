
#include <stdio.h>
#include "calvis.h"

int main(int argc, char **argv)
{
  cv_launch(argc, argv);
  return 0;
}


int cv_build_scene()
{
  cow_domain *domain = cow_domain_new();
  cow_dfield *dfield = cow_dfield_new();

  int err = cow_domain_readsize(domain, "chkpt.0500.h5", "prim/rho");
  if (err) {
    fprintf(stderr, "[calvis:error] input file was not found or invalid\n");
    return 1;
  }
  cow_domain_setguard(domain, 1);
  cow_domain_commit(domain);

  cow_dfield_setname(dfield, "prim");
  cow_dfield_addmember(dfield, "rho");
  cow_dfield_setdomain(dfield, domain);
  cow_dfield_commit(dfield);

  int nx = cow_domain_getsize(domain, 0);
  int ny = cow_domain_getsize(domain, 1);
  int nz = cow_domain_getsize(domain, 2);
  printf("[calvis] read data file with size (%d %d %d)\n", nx, ny, nz);

  cow_dfield_read(dfield, "chkpt.0500.h5");
  cow_dfield_syncguard(dfield);

  double sample_point[3] = {0.5, 0.5, 0.5};
  double *sample_result;

  cow_dfield_setsamplecoords(dfield, sample_point, 1, 3);
  cow_dfield_sampleexecute(dfield);
  cow_dfield_getsampleresult(dfield, &sample_result, NULL, NULL);

  ImagePlane *img1 = cv_imageplane_new();
  ImagePlane *img2 = cv_imageplane_new();
  ImagePlane *img3 = cv_imageplane_new();

  cv_imageplane_from3d(img1, dfield, 0, 0, nx/2);
  cv_imageplane_from3d(img2, dfield, 0, 1, ny/2);
  cv_imageplane_from3d(img3, dfield, 0, 2, 0);

  cow_dfield_del(dfield);
  cow_domain_del(domain);

  return 0;
}
