#if !defined(_reflect_Registeraction_hpp)
#define _reflect_Registeraction_hpp

#include <string>

class reflect_Obj;

/// @brief 要实现“静态代码块”，可借助静态变量的构造函数
class reflect_Registeraction
{
public:

  /// @brief 动作1：注册构造函数
  /// @param className 
  /// @param constructor 
  reflect_Registeraction(std::string const& className, reflect_Obj* (*constructor)(void));

};

#define REFLECT_REGISTER_CONSTRUCTOR(className)\
reflect_Obj* __reflect_defaultConstruct_##className (){\
  return static_cast<reflect_Obj*>(new className());\
}\
static reflect_Registeraction registerConstructor_##className (#className, __reflect_defaultConstruct_##className);

#endif // _reflect_Registeraction_hpp
