#include "reflect/Obj.hpp"
#include "reflect/Registeraction.hpp"
#include "reflect/Registry.hpp"

class Foo : public reflect_Obj{
public:
  int i;
};
REFLECT_REGISTER_CONSTRUCTOR(Foo)

int main(int argc, char const *argv[])
{
  auto obj = reflect_Registry::get().getConstructor("Foo")();
  delete obj;
  
  return 0;
}
