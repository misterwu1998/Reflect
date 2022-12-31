#include "reflect/Obj.hpp"
#include "reflect/Registeraction.hpp"
#include "reflect/Registry.hpp"
#include "reflect/Field.hpp"

class Foo : public reflect_Obj{
public:
  int id;
  std::string name;
  Foo():id(114514){}
};
REFLECT_REGISTER_CONSTRUCTOR(Foo)
REFLECT_REGISTER_FIELD(Foo,id)
REFLECT_REGISTER_FIELD(Foo,name)

int main(int argc, char const *argv[])
{
  auto obj = reflect_Registry::get().getConstructor("Foo")();
  
  // 读写对象的成员
  auto id = obj->get<int>("id");
  obj->set("name", std::string("Jack"));
  auto name = obj->get<std::string>("name");

  delete obj;

  return 0;
}
