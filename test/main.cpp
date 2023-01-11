#include <iostream>
#include <string>
#include "reflect/single_include.hpp"

#include "./Foo.hpp"

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
  reflect_Ptr obj;
  int id;
  std::string name;
  int* (*ptr)(std::unique_ptr<float>);

  obj = reflect_new/*<>*/("unknown");
  obj = reflect_new<int&&>("unknown",114514);//int&&型应当注册了Foo::Foo()，而没有unknown
  obj = reflect_new<int, std::string&&>("null", 114514,"田所");//同上
  obj = reflect_new/*<>*/("Foo");
  obj = reflect_new/*<int>*/("Foo",114514);//nullptr
  obj = reflect_new<int&&>("Foo",114514);
  obj = reflect_new<std::string const&>("Foo","田所");
  obj = reflect_new/* <int, char const*> */("Foo", 114514,"foo");

  ret = obj->get("id",id);
  ret = obj->set("id",1919810);
  ret = obj->get("id",id);
  ret = obj->get("name",name);
  // ret = obj->set/*<char const*>*/("name","王爷");//编译在赋值语句处报错：invalid array assignment
  ret = obj->set<std::string>("name","王爷");//ok
  ret = obj->get("name",name);
  ret = obj->get("unknown",id);
  ret = obj->get("unknown",name);
  ret = obj->set("unknown",123);
  ret = obj->set("unknown", std::string("nobody"));

  ret = obj->pro("nothing");
  ret = obj->pro/* <double> */("shout",114.514);
  ret = obj->pro("unknown");
  ret = obj->pro/* <double> */("unknown",114.514);
  ret = obj->pro<double&&>("shout", 114.514);//0

  ret = obj->func("getId",id);
  ret = obj->func
    < decltype(ptr),  std::string, 
        Foo&&>("unknown",   
      ptr,            "reflect!",  
        Foo{42,"答案"});
  ret = obj->func("unknown",id);//0
  ret = obj->func("getId", name);//0
  ret = obj->func
    < decltype(ptr),  std::string, 
        Foo&&>("unknown",   
      ptr,            "reflect!",  
        Foo{42,"答案"});

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
