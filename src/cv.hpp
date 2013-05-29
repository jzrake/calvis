
#include <list>

class cvObject
{

} ;

class cvKeyboardResponsiveObject : public cvObject
{
public:
  virtual ~cvKeyboardResponsiveObject() { }
  virtual int respond_to_key(unsigned char key, int x, int y) = 0;
} ;

class cvRenderableObject : public cvObject
{
public:
  virtual ~cvRenderableObject() { }
  virtual void render() = 0;
} ;

class cvRenderableKeyboardResponsiveObject : public cvRenderableObject,
					     public cvKeyboardResponsiveObject
{
public:
  virtual ~cvRenderableKeyboardResponsiveObject() { }
} ;

class cvRenderWindow : public cvObject
{
private:
  std::list<cvRenderableObject*> ObjectList;
  double TranslateZ;
  double RotationX;
  double RotationY;
public:
  cvRenderWindow();
  virtual ~cvRenderWindow();
  int DisplayFunc();
  int IdleFunc();
  int ReshapeFunc(int Width, int Height);
  int KeyboardFunc(unsigned char key, int x, int y);
  int SpecialFunc(int key, int x, int y);
  template <class T> T *new_object()
  {
    T *object = new T;
    ObjectList.push_back(object);
    return object;
  }
} ;

class cvTeapot : public cvRenderableKeyboardResponsiveObject
{
public:
  virtual ~cvTeapot() { }
  int respond_to_key(unsigned char key, int x, int y);
  void render();
} ;
