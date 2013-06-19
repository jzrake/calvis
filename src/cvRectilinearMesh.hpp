#ifndef __CV_cvRectilinearMesh_HEADER
#define __CV_cvRectilinearMesh_HEADER
#include "cv.hpp"

struct cow_domain;
struct cow_dfield;

class cvRectilinearMesh : public cvDataSource
{
private:
  char *filename;
  cow_domain *domain;
  cow_dfield *dfield;
public:
  cvRectilinearMesh();
  virtual ~cvRectilinearMesh();
  virtual int get_number_of_components();
  virtual int get_rank();
  virtual void sample_data(double *position, double *data);
  void set_extent(int dim, double x0, double x1);
  void set_file_name(const char *fname);
  void set_group_name(const char *gname);
  void add_component(const char *member);
  void load();
} ;

#endif // __CV_cvRectilinearMesh_HEADER
