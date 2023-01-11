#if !defined(_reflect_ClassRegistration_hpp)
#define _reflect_ClassRegistration_hpp 

#include <string>
#include <memory>

#include "reflect/detail/class/ClassRegistry.hpp"
#include "reflect/detail/class/Constructor.hpp"

class reflect_Obj;

template <typename ...ConstructorArgTypes>
class _reflect_ClassRegistration
{
public:
  /// @brief 注册名为className的类的构造器
  /// @param className 
  /// @param _constructor 
  _reflect_ClassRegistration(
    std::string const& className,
    std::shared_ptr<reflect_Obj> (*_constructor)(ConstructorArgTypes...)
  ){
    _reflect_ClassRegistry<ConstructorArgTypes...>::set(className,_constructor);
  }
};

/// 为了同时注册多个构造函数，用户需要随便提供一个唯一的_constructorNickname作区分
#define REFLECT_REGISTER_CONSTRUCTOR(Class, _constructorNickname, ...) \
static _reflect_ClassRegistration<__VA_ARGS__> \
  _reflect_classRegistration_##Class##_##_constructorNickname( \
    #Class, \
    _reflect_Constructor<Class, ##__VA_ARGS__> \
  );

#endif // _reflect_ClassRegistration_hpp 
