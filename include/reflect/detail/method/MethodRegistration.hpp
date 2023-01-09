#if !defined(_reflect_MethodRegistration_hpp)
#define _reflect_MethodRegistration_hpp

#include "reflect/detail/method/MethodRegistry.hpp"

template <typename ReturnType, typename ... ArgTypes>
class _reflect_MethodRegistration
{
public:
  _reflect_MethodRegistration(
    std::string const& className,
    std::string const& methodName,
    _reflect_Method const& method
  ){
    _reflect_MethodRegistry<ReturnType, ArgTypes...>::set(className,methodName,method);
  }
};

#if 0 // for coding
struct Foo{
  char get(){}
  void nothing(){}
  void pro(double&& d, const char* s){}
  int* (*func(std::string s, Foo&& f))(std::unique_ptr<float>){}//func(std::string s, Foo&& f)返回函数指针，该函数指针接收std::unique_ptr<float>，返回int*
};

static _reflect_MethodRegistration<char> _reflect_methodRegistration_Foo_get(
  "Foo", "get",
  _reflect_Method(
    (void*)(&Foo::get),
    "get"
  )
);

static _reflect_MethodRegistration<void> _reflect_methodRegistration_Foo_nothing(
  "Foo", "nothing",
  _reflect_Method(
    (void*)(&Foo::nothing)
  )
);

static _reflect_MethodRegistration<void, double&&, const char*> _reflect_methodRegistration_Foo_pro(
  "Foo", "pro",
  _reflect_Method(
    (void*)(&Foo::pro),
    "pro"
  )
);

static _reflect_MethodRegistration<
  int* (*)(std::unique_ptr<float>),
  std::string, Foo&&
> _reflect_methodRegistration_Foo_func(
  "Foo", "func",
  _reflect_Method(
    (void*)(&Foo::func),
    "func"
  )
);
#endif

#define REFLECT_REGISTER_METHOD(Class,method,ReturnType,...) \
static _reflect_MethodRegistration<ReturnType, ##__VA_ARGS__ > \
  _reflect_methodRegistration_##Class##_##method ( \
    #Class, #method, \
    _reflect_Method( \
      (void*)(&Class::method), \
      #method \
    ) \
  );

#endif // _reflect_MethodRegistration_hpp

#if 0 //example
struct Foo{
  char get(){}
  void nothing(){}
  void pro(double&& d, const char* s){}
  int* (*func(std::string s, Foo&& f))(std::unique_ptr<float>){}//func(std::string s, Foo&& f)返回函数指针，该函数指针接收std::unique_ptr<float>，返回int*
};
REFLECT_REGISTER_METHOD(Foo,func, int* (*)(std::unique_ptr<float>), std::string, Foo&&)
#endif
