#ifndef __CV_cv_HEADER
#define __CV_cv_HEADER

class cvObject;
class cvRenderWindow;
class cvSceneElement;

class cvObject
{

} ;

class cvRenderWindow : public cvObject
{
public:
  virtual void add_element(cvSceneElement *e) = 0;
  virtual void render() = 0;
} ;

class cvSceneElement : public cvObject
{
public:
  virtual int respond_to_key(unsigned char key, int x, int y) = 0;
  virtual void render() = 0;
} ;

class cvDataSource : public cvObject { } ;
class cvImagePlane : public cvObject { } ;
class cvPointwiseTransform : public cvObject { } ;
class cvPlaneExtraction : public cvObject { } ;

#endif // __CV_cv_HEADER
