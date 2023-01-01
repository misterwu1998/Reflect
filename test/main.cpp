#include "reflect/Obj.hpp"
#include "reflect/Registeraction.hpp"
#include "reflect/Registry.hpp"
#include "reflect/Field.hpp"
#include "./Foo.hpp"
#include <iostream>

#define __test_memberFunctor 0

int main(int argc, char const *argv[])
{
#if __test_memberFunctor
  auto f = &Foo::getId;
  auto v = (void*)f;//类成员函数指针也可以类型擦除
  Foo abc;
  typedef int (*Functor_i_s)(Foo* const, std::string const&);
  Functor_i_s fff = (Functor_i_s)v;
  int ret = fff(&abc, "abc");
#endif

  auto obj = reflect_Registry::get().getConstructor("Foo")();
  
  // 读写对象的成员
  auto id = obj->get<int>("id");
  obj->set("name", std::string("Jack"));
  auto name = obj->get<std::string>("name");

  // 调用对象的方法
  id = 0;
  id = obj->callFunc<int, std::string const&>("getId", std::string("abc"));//有返回值
  obj->callPro<double>("shout", 114.514);//无返回值
  obj->callPro("shout", 114.514);//不那么复杂的形参就不必指明类型了，让编译器猜
  
  delete obj;

  return 0;
}
