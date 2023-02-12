#if !defined(_reflect_ClassRegistration_inl)
#define _reflect_ClassRegistration_inl

#include "reflect/detail/class/ClassRegistration.hpp"

#include "reflect/detail/class/ClassRegistry.hpp"
#include "reflect/detail/class/Constructor.hpp"
#include "reflect/detail/class/ClassName.hpp"
#include "reflect/detail/class/normal.hpp"

template <typename... ConstructorArgTypes>
_reflect_ClassRegistration<ConstructorArgTypes...>::_reflect_ClassRegistration(
    std::string const &className,
    reflect_Obj *(*_new)(ConstructorArgTypes...),
    std::shared_ptr<reflect_Obj> (*_make_shared)(ConstructorArgTypes...))
{
  _reflect_ClassRegistry<ConstructorArgTypes...>::set(className, _new);
  _reflect_ClassRegistry<ConstructorArgTypes...>::set(className, _make_shared);
}

template <typename Class>
_reflect_ClassNameRegistration<Class>::_reflect_ClassNameRegistration(std::string const &name)
{
  auto &classNameSingleton = reflect_ClassName<Class>::registerOrGet(name);
  _reflect_normalRegistry::set(name, &classNameSingleton);
}

/// 为了同时注册多个构造函数，用户需要随便提供一个唯一的_constructorNickname作区分
#define REFLECT_REGISTER_CONSTRUCTOR(Class, _constructorNickname, ...) \
  static _reflect_ClassRegistration<__VA_ARGS__>                       \
      _reflect_classRegistration_##Class##_##_constructorNickname(     \
          #Class,                                                      \
          _reflect_new<Class, ##__VA_ARGS__>,                          \
          _reflect_make_shared<Class, ##__VA_ARGS__>);                 \
  static _reflect_ClassNameRegistration<Class> _reflect_classNameRegi_##Class##_##_constructorNickname(#Class);

#endif // _reflect_ClassRegistration_inl
