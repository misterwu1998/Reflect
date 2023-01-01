#include "./Foo.hpp"
#include "reflect/Registeraction.hpp"
#include <iostream>

Foo::Foo():id(114514){
  std::cout << "Foo::Foo()" << std::endl;
}

int Foo::getId(std::string const& name){
  std::cout << "Foo::getId(): " << name << std::endl;
  return id;
}

void Foo::shout(double d){
  std::cout << "AAAAAAAAAAAAAAAAAAAAAAhhhhhhhhh!!!! " << d << std::endl;
}

Foo::~Foo(){
  std::cout << "Foo::~Foo()" << std::endl;
}

REFLECT_REGISTER_CONSTRUCTOR(Foo)
REFLECT_REGISTER_FIELD(Foo,id)
REFLECT_REGISTER_FIELD(Foo,name)
REFLECT_REGISTER_METHOD(Foo,getId);
REFLECT_REGISTER_METHOD(Foo,shout);
