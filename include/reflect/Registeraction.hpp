#if !defined(_reflect_Registeraction_hpp)
#define _reflect_Registeraction_hpp

#include <string>
#include <memory>
#include "reflect/Registry.hpp"

class reflect_Obj;
class reflect_Field;
class reflect_Method;

/// @brief 这个类是C++反射库的精髓，实现了“静态代码块”。其原理就是任意源文件（注意，是源文件，不可以放进头文件）中定义的全局对象都需要在main()之前初始化，没指定构造函数的话无参构造函数就会被调用，有指定构造函数的话，我们想在main()之前干啥就都可以在那个构造函数里边完成了
/// 为什么放进源文件而非头文件：我们希望同一个注册动作只发生一次，即，代表某个具体动作的Registeraction对象只有一个。如果在头文件中使用以下宏注册，那么任何展开了头文件的源文件中都会有一批Registeraction对象。这也是为什么以下宏定义不使用static修饰Registeraction对象，这样的话，一旦出现多份，链接程序时就会报错。
class reflect_Registeraction
{
public:

  /// @brief 动作1：注册构造函数
  /// @param className 
  /// @param constructor 
  reflect_Registeraction(std::string const& className, std::shared_ptr<reflect_Obj> (*constructor)(void)){
    auto& r = reflect_Registry::get();
    r.registerConstructor(className,constructor);
  }

  /// @brief 动作2：注册类成员变量
  /// @param className 
  /// @param fieldName 
  /// @param field 
  reflect_Registeraction(std::string const& className, std::string const& fieldName, reflect_Field const& field){
    auto& r = reflect_Registry::get();
    r.registerField(className,fieldName,field);
  }

  /// @brief 动作3：注册成员函数
  /// @param className 
  /// @param methodName 
  /// @param method 
  reflect_Registeraction(std::string const& className, std::string const& methodName, reflect_Method const& method){
    auto& r = reflect_Registry::get();
    r.registerMethod(className,methodName,method);
  }

};

#define REFLECT_REGISTER_CONSTRUCTOR(className)\
std::shared_ptr<reflect_Obj> __reflect_defaultConstruct_##className (){\
  auto obj = std::static_pointer_cast<reflect_Obj,className>(\
    std::make_shared<className>()\
  );\
  obj->__className = #className;\
  return obj;\
}\
reflect_Registeraction __reflect_regi_##className (#className, __reflect_defaultConstruct_##className);\
className __reflect_temp_##className ;

#define REFLECT_REGISTER_FIELD(className,fieldName)\
reflect_Registeraction __reflect_regi_##className##_##fieldName (\
  #className , #fieldName ,\
  reflect_Field(\
    ((uint64_t)(&((__reflect_temp_##className).fieldName))) - ((uint64_t)(&(__reflect_temp_##className))),\
    sizeof(__reflect_temp_##className.fieldName),\
    #fieldName\
  )\
);

#define REFLECT_REGISTER_METHOD(className,methodName)\
reflect_Registeraction __reflect_regi_##className##_##methodName(\
  #className, #methodName,\
  reflect_Method(\
    (void*)(&className::methodName),\
    #methodName\
  )\
);

#endif // _reflect_Registeraction_hpp
