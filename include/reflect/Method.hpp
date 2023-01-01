#if !defined(__reflect_Method_hpp)
#define __reflect_Method_hpp

#include <string>

class reflect_Method
{
public:
  
  void* __functor;//类成员函数的地址，统统擦除掉类型
  std::string name;
  reflect_Method() : __functor(NULL){}
  reflect_Method(void* functor, std::string const& name): __functor(functor), name(name){}

  void* getFunctor() const {return __functor;}

  // 还是不拖到这里才擦除类型了，在注册动作宏定义那里就应该早早擦除
  // /// @brief 接收任意类型的成员函数指针，擦除掉它的类型
  // /// @tparam T 成员函数属于哪个类
  // /// @tparam RetType 成员函数返回值类型
  // /// @tparam ...ArgTypes 成员函数形参类型
  // /// @param ptr 成员函数指针，类型是 RetType (T::*)(ArgTypes ...)
  // /// @param name 成员函数名
  // template <typename RetType, typename T, typename ... ArgTypes>
  // reflect_Method(RetType (T::* ptr)(ArgTypes ...), std::string const& name) : __functor((void*)ptr), name(name){}
  
};

#endif // __reflect_Method_hpp
