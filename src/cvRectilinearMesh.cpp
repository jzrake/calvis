#include <cstring>
#include <cstdio>
#include "cvRectilinearMesh.hpp"
extern "C" {
#include "cow.h"
}

cvRectilinearMesh::cvRectilinearMesh()
  : filename(NULL)
{
  domain = cow_domain_new();
  dfield = cow_dfield_new();
}
cvRectilinearMesh::~cvRectilinearMesh()
{
  cow_domain_del(domain);
  cow_dfield_del(dfield);
  free(filename);
}
int cvRectilinearMesh::get_number_of_components()
{
  return cow_dfield_getnmembers(dfield);
}
int cvRectilinearMesh::get_rank()
{
  return cow_domain_getndim(domain);
}
void cvRectilinearMesh::sample_data(double *position, double *data)
{

}
void cvRectilinearMesh::set_extent(int dim, double x0, double x1)
{
  cow_domain_setextent(domain, dim, x0, x1);
}
void cvRectilinearMesh::set_file_name(const char *fname)
{
  filename = (char *) realloc(filename, strlen(fname) + 1);
  strcpy(filename, fname);
}
void cvRectilinearMesh::set_group_name(const char *gname)
{
  char *tmp = (char *) malloc(strlen(gname) + 1);
  strcpy(tmp, gname);
  cow_dfield_setname(dfield, tmp);
  free(tmp);
}
void cvRectilinearMesh::add_component(const char *member)
{
  char *tmp = (char *) malloc(strlen(member) + 1);
  strcpy(tmp, member);
  cow_dfield_addmember(dfield, tmp);
  free(tmp);
}
void cvRectilinearMesh::load()
{
  char *gname = cow_dfield_getname(dfield);
  char *mname = cow_dfield_iteratemembers(dfield);
  char *tmp = (char *) malloc(strlen(gname) + strlen(mname) + 2);
  sprintf(tmp, "%s/%s", gname, mname);
  cow_domain_readsize(domain, filename, tmp);
  free(tmp);
  cow_dfield_setdomain(dfield, domain);
  cow_domain_commit(domain);
  cow_dfield_commit(dfield);
  cow_dfield_read(dfield, filename);
}
