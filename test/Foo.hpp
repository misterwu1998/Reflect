#if !defined(_reflect_Foo_hpp)
#define _reflect_Foo_hpp

#include "reflect/Obj.hpp"
#include <string>

class Foo : public reflect_Obj{
public:
  int id;
  std::string name;
  Foo();
  int getId(std::string const& name);
  void shout(double d);
};

#endif // _reflect_Foo_hpp
