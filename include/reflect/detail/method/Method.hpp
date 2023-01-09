#if !defined(__reflect_Method_hpp)
#define __reflect_Method_hpp

#include <string>

/// @brief 函数无关于类型的属性，即，无论什么类的什么方法，都会有以下属性
class _reflect_Method
{
public:
  
  void* __functor;//类成员函数的地址，统统擦除掉类型
  std::string name;
  _reflect_Method() : __functor(NULL){}
  _reflect_Method(void* functor, std::string const& name): __functor(functor), name(name){}

  inline void* getFunctor() const {return __functor;}
  inline std::string getName() const{ return name;}

};

#endif // __reflect_Method_hpp
