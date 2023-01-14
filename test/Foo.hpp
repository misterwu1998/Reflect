#if !defined(_reflect_Foo_hpp)
#define _reflect_Foo_hpp

#include <iostream>
#include "reflect/single_include.hpp"

class Bar: public reflect_Obj{
public:
  std::vector<int> v;
  Bar(){
    v.emplace_back(114514);
    v.emplace_back(1919810);
  }
};

REFLECT_REGISTER_CONSTRUCTOR(Bar,prototype)
REFLECT_REGISTER_FIELD(Bar,v,std::vector<int>)

#include "serial/single_include.hpp"

class Foo : public reflect_Obj{
public:
  int id;
  std::string name;
  Bar bar;
  Foo():id(1919810){}
  Foo(int&& id):id(id){}
  Foo(std::string const& name):name(name){
    std::cout << "Foo::Foo(): name is " << name << std::endl;
  }
  Foo(int id, const char* name):id(id),name(name){}
  void shout(double d){std::cout << d << std::endl;}
  void nothing(){}
  int getId(){return id;}
  int* (*func(std::string s, Foo&& f))(std::unique_ptr<float>)
    {return (int* (*)(std::unique_ptr<float>))114514;}
  ~Foo(){}

  // 用于演示二进制序列化
  int serialize(serial_Archiver& a)
  {return a(id,name);}
};

REFLECT_REGISTER_CONSTRUCTOR(Foo, prototype)
REFLECT_REGISTER_CONSTRUCTOR(Foo, rvInt, int&&)
REFLECT_REGISTER_CONSTRUCTOR(Foo, crStr, std::string const&)
REFLECT_REGISTER_CONSTRUCTOR(Foo, both, int, const char*)
REFLECT_REGISTER_FIELD(Foo,id,int)
REFLECT_REGISTER_FIELD(Foo,name, std::string)
REFLECT_REGISTER_FIELD(Foo,bar,Bar)
REFLECT_REGISTER_METHOD(Foo,getId,int)
REFLECT_REGISTER_METHOD(Foo,shout,void,double)
REFLECT_REGISTER_METHOD(Foo,nothing,void)
REFLECT_REGISTER_METHOD(Foo,func, int* (*)(std::unique_ptr<float>), std::string, Foo&&)

REFLECT_REGISTER_METHOD(Foo,serialize,int,serial_Archiver&)

#if 0 //旧版，非类型安全

#include "reflect/Obj.hpp"
#include <string>

class Foo : public reflect_Obj{
public:
  int id;
  std::string name;
  Foo();
  int getId(std::string const& name);
  void shout(double d);
  ~Foo();
};

#endif //旧版，非类型安全

#endif // _reflect_Foo_hpp
