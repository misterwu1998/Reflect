#include <iostream>
#include <string>
#include "reflect/single_include.hpp"

#include "./Foo.hpp"

// for coding
// #include "reflect/detail/class/normal.hpp" 
// #include "reflect/detail/class/ClassRegistration.inl"
// #include "reflect/detail/class/normal.inl"
// #include "reflect/detail/field/FieldRegistration.inl"
// #include "reflect/detail/field/FieldRegistry.inl"
// #include "reflect/detail/method/MethodRegistration.inl"
// #include "reflect/detail/method/MethodRegistry.inl"
// #include "reflect/detail/object/Obj.inl"
// #include "reflect/detail/json/dispatch.hpp"

#include "serial/single_include.hpp"
#define REFLECT_SERIALIZE(pointer_or_sharedPtr,serialRetCode,archiver)\
(pointer_or_sharedPtr ? \
  pointer_or_sharedPtr->func<int,serial_Archiver&>("serialize", serialRetCode,archiver) : \
  -1)

#define __test_memberFunctor 0
#define __test_templateVoidArg 0

#if __test_templateVoidArg
template <typename T, typename ... Ts> struct TTT{
  using RT = T (*)(Ts...);
};
#endif

int main(int argc, char const *argv[])
{
  int ret;
  reflect_Obj* raw;
  reflect_Ptr shared;
  int id;
  std::string name;
  int* (*ptr)(std::unique_ptr<float>);

  raw = reflect_new/*<>*/("unknown");
  if(raw) delete raw;
  raw = reflect_new<int&&>("unknown",114514);//int&&型应当注册了Foo::Foo()，而没有unknown
  if(raw) delete raw;
  raw = reflect_new<int, std::string&&>("null", 114514,"田所");//同上
  if(raw) delete raw;
  raw = reflect_new/*<>*/("Foo");
  if(raw) delete raw;
  raw = reflect_new/*<int>*/("Foo",114514);//nullptr
  if(raw) delete raw;
  raw = reflect_new<int&&>("Foo",114514);
  if(raw) delete raw;
  raw = reflect_new<std::string const&>("Foo","田所");
  if(raw) delete raw;
  raw = reflect_new/* <int, char const*> */("Foo", 114514,"foo");
  {auto p = REFLECT_ACCESS(raw,"id",int);}
  {auto p = REFLECT_ACCESS(raw,"name", std::string); std::cout << "name is " << *p << std::endl;}
  {auto p = REFLECT_ACCESS(raw,"unknown", int);}
  {auto p = REFLECT_ACCESS(raw,"unknown", std::string);}
  if(raw) delete raw;
  raw = NULL;
  {auto p = REFLECT_ACCESS(raw,"id",int);}

  shared = reflect_share/*<>*/("unknown");
  shared = reflect_share<int&&>("unknown",114514);//int&&型应当注册了Foo::Foo()，而没有unknown
  shared = reflect_share<int, std::string&&>("null", 114514,"田所");//同上
  shared = reflect_share/*<>*/("Foo");
  shared = reflect_share/*<int>*/("Foo",114514);//nullptr
  shared = reflect_share<int&&>("Foo",114514);
  shared = reflect_share<std::string const&>("Foo","田所");
  shared = reflect_share/* <int, char const*> */("Foo", 114514,"foo");

  ret = shared->get("id",id);
  ret = shared->set("id",1919810);
  ret = shared->get("id",id);
  ret = shared->get("name",name);
  // ret = shared->set/*<char const*>*/("name","王爷");//编译在赋值语句处报错：invalid array assignment
  ret = shared->set<std::string>("name","nobody");//ok
  ret = shared->get("name",name);
  ret = shared->get("unknown",id);
  ret = shared->get("unknown",name);
  ret = shared->set("unknown",123);
  ret = shared->set("unknown", std::string("nobody"));
  {auto p = REFLECT_ACCESS(shared,"id",int);}
  {auto p = REFLECT_ACCESS(shared,"name",std::string); std::cout << "name is " << *p << std::endl;}
  {auto p = REFLECT_ACCESS(shared,"unknown", int);}
  {auto p = REFLECT_ACCESS(shared,"unknown", std::string);}

  ret = shared->pro("nothing");
  ret = shared->pro/* <double> */("shout",114.514);
  ret = shared->pro("unknown");
  ret = shared->pro/* <double> */("unknown",114.514);
  ret = shared->pro<double&&>("shout", 114.514);//0

  ret = shared->func("getId",id);
  ret = shared->func
    < decltype(ptr),  std::string,  Foo&&>("unknown",   
      ptr,            "reflect!",   Foo{42,"答案"});
  ret = shared->func("unknown",id);//0
  ret = shared->func("getId", name);//0
  ret = shared->func
    < decltype(ptr),  std::string,  Foo&&>("unknown",   
      ptr,            "reflect!",   Foo{42,"答案"});
  
  auto fields = shared->getFields();
  for(auto& kv : *fields){
    std::cout << kv.first << ": "
              // << kv.second.name << "; "
              << kv.second.offset << "; "
              << kv.second.size << "; "
              << kv.second.typeName << std::endl;
  }

  auto methods = shared->getMethods();
  for(auto& kv: *methods){
    std::cout << kv.first << ": "
              // << kv.second.getName() << "; "
              << kv.second.getRetTypeName() << "; "
              << kv.second.getArgTypeNames() << "; "
              << kv.second.getFunctor() << std::endl;
  }

  // 演示二进制序列化
  int rrc;
  int src;
  Buffer b;
  serial_Archiver ao(b, serial_ArchiverType::out_binary_littleEndian);
  rrc = REFLECT_SERIALIZE(shared,src,ao);
  serial_Archiver ai(b, serial_ArchiverType::in_binary_littleEndian);
  rrc = REFLECT_SERIALIZE(shared,src,ai);

  reflect_JSON j;
  ret = shared->toJSON(j);
  auto dumpling = j.dump();
  std::cout << dumpling << std::endl;

  j["bar"]["v"] = {1919810,114514};
  j["id"] = 1024;
  j["name"] = "王爷";
  std::cout << j.dump() << std::endl;
  raw = reflect_new("Foo");
  raw->fromJSON(j);
  {auto p = REFLECT_ACCESS(raw,"id",int); std::cout << *p << '\t';}
  {auto p = REFLECT_ACCESS(raw,"name",std::string); std::cout << *p << std::endl;}
  delete raw;

  raw = NULL;
  {auto p = REFLECT_ACCESS(shared,"id",int);}

  shared = nullptr;
  {auto p = REFLECT_ACCESS(shared,"id",int);}

  return 0;

#if __test_templateVoidArg
  TTT<void>::RT f;
#endif

#if 0 //旧版，非类型安全

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
  obj->callPro("shout", 114514);//能不能猜到double？不行，最后传给成员函数的是0
  obj->callPro<double&&>("shout", 1919.81);//这个也会导致传给成员函数的实参是0。这也是为什么callFunc()和callPro()不使用万能引用，因为参数类型需要尽可能简明、准确，如果用户有指定，那指定什么就得是什么，使用万能引用的话，指定double实际仍有可能是double&&
  id = 0;
  id = obj->callFunc<int>("getId", std::string("shit"));//虽然内部的_Func的形参是(_This const, std::string)型的，但也成功把字符串传给成员函数getId()的std::string const&形参了
  
  // delete obj;已经改裸指针为shared_ptr

#endif //旧版，非类型安全

#if __test_memberFunctor
  auto f = &Foo::getId;
  auto v = (void*)f;//类成员函数指针也可以类型擦除
  Foo abc;
  typedef int (*Functor_i_s)(Foo* const, std::string const&);
  Functor_i_s fff = (Functor_i_s)v;
  int ret = fff(&abc, "abc");
#endif
}
