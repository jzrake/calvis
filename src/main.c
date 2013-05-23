
#include <stdio.h>
#include "../cow/cow.h"


int cv_launch(int argc, char **argv);

int main(int argc, char **argv)
{
  cow_domain *domain = cow_domain_new();
  cow_dfield *dfield = cow_dfield_new();

  cow_domain_setndim(domain, 3);
  cow_domain_setguard(domain, 1);
  cow_domain_setsize(domain, 0, 32);
  cow_domain_setsize(domain, 1, 32);
  cow_domain_setsize(domain, 2, 32);
  cow_domain_commit(domain);

  cow_dfield_setname(dfield, "prim");
  cow_dfield_addmember(dfield, "vx");
  cow_dfield_addmember(dfield, "vy");
  cow_dfield_addmember(dfield, "vz");
  cow_dfield_addmember(dfield, "rho");
  cow_dfield_addmember(dfield, "pre");
  cow_dfield_setdomain(dfield, domain);
  cow_dfield_commit(dfield);

  cow_dfield_read(dfield, "chkpt.0500.h5");
  cow_dfield_syncguard(dfield);

  double sample_point[3] = {0.5, 0.5, 0.5};
  double *sample_result;

  cow_dfield_setsamplecoords(dfield, sample_point, 1, 3);
  cow_dfield_sampleexecute(dfield);
  cow_dfield_getsampleresult(dfield, &sample_result, NULL, NULL);

  printf("%f %f %f %f %f\n",
	 sample_result[0],
	 sample_result[1],
	 sample_result[2],
	 sample_result[3],
	 sample_result[4]);

  cow_dfield_del(dfield);
  cow_domain_del(domain);

  cv_launch(argc, argv);

  return 0;
}
