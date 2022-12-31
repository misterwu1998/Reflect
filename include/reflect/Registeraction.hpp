#if !defined(_reflect_Registeraction_hpp)
#define _reflect_Registeraction_hpp

#include <string>

class reflect_Obj;
class reflect_Field;

/// @brief 要实现“静态代码块”，可借助静态变量的构造函数
class reflect_Registeraction
{
public:

  /// @brief 动作1：注册构造函数
  /// @param className 
  /// @param constructor 
  reflect_Registeraction(std::string const& className, reflect_Obj* (*constructor)(void));

  /// @brief 动作2：注册类成员变量
  /// @param className 
  /// @param fieldName 
  /// @param field 
  reflect_Registeraction(std::string const& className, std::string const& fieldName, reflect_Field const& field);

};

#define REFLECT_REGISTER_CONSTRUCTOR(className)\
reflect_Obj* __reflect_defaultConstruct_##className (){\
  auto obj = static_cast<reflect_Obj*>(new className());\
  obj->__className = #className;\
  return obj;\
}\
static reflect_Registeraction __reflect_regi_##className (#className, __reflect_defaultConstruct_##className);\
static className __reflect_temp_##className ;

#define REFLECT_REGISTER_FIELD(className,fieldName)\
reflect_Registeraction __reflect_regi_##className##_##fieldName (\
  #className , #fieldName ,\
  reflect_Field(\
    ((uint64_t)(&((__reflect_temp_##className).fieldName))) - ((uint64_t)(&(__reflect_temp_##className))),\
    sizeof(__reflect_temp_##className.fieldName),\
    #fieldName\
  )\
);

#endif // _reflect_Registeraction_hpp
