#if !defined(_reflect_MethodRegistration_hpp)
#define _reflect_MethodRegistration_hpp

class reflect_Method;

template <typename ReturnType, typename ... ArgTypes>
class _reflect_MethodRegistration
{
public:
  _reflect_MethodRegistration(
    std::string const& className,
    std::string const& methodName,
    reflect_Method const& method
  );
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
  reflect_Method(
    (void*)(&Foo::get),
    "get"
  )
);

static _reflect_MethodRegistration<void> _reflect_methodRegistration_Foo_nothing(
  "Foo", "nothing",
  reflect_Method(
    (void*)(&Foo::nothing)
  )
);

static _reflect_MethodRegistration<void, double&&, const char*> _reflect_methodRegistration_Foo_pro(
  "Foo", "pro",
  reflect_Method(
    (void*)(&Foo::pro),
    "pro"
  )
);

static _reflect_MethodRegistration<
  int* (*)(std::unique_ptr<float>),
  std::string, Foo&&
> _reflect_methodRegistration_Foo_func(
  "Foo", "func",
  reflect_Method(
    (void*)(&Foo::func),
    "func"
  )
);
#endif

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
