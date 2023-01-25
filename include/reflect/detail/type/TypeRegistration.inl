#if !defined(_reflect_TypeRegistration_inl)
#define _reflect_TypeRegistration_inl

#include "reflect/detail/type/TypeRegistration.hpp"

#include "reflect/detail/type/TypeRegistry.hpp"
#include "reflect/detail/type/Constructor.hpp"
#include "reflect/detail/type/TypeName.hpp"

template <typename ...ConstructorArgTypes>
_reflect_TypeRegistration<ConstructorArgTypes...>::_reflect_TypeRegistration(
  std::string const& typeName,
  reflect_Obj* (*_new)(ConstructorArgTypes...),
  std::shared_ptr<reflect_Obj> (*_make_shared)(ConstructorArgTypes...)
){
  _reflect_TypeRegistry<ConstructorArgTypes...>::set(typeName,_new);
  _reflect_TypeRegistry<ConstructorArgTypes...>::set(typeName,_make_shared);
}

template <typename Type>
_reflect_TypeNameRegistration<Type>::_reflect_TypeNameRegistration(std::string const& name){
  reflect_TypeName<Type>::registerOrGet(name);
}

/// @param _registrationNickname 无意义，仅用于多次注册动作之间的区分
#define REFLECT_REGISTER_TYPENAME(Type,_registrationNickname)\
static _reflect_TypeNameRegistration<Type> _reflect_typeNameRegi_##Type##_##_registrationNickname(#Type);

/// 为了同时注册多个构造函数，用户需要随便提供一个唯一的_constructorNickname作区分
#define REFLECT_REGISTER_CONSTRUCTOR(Type, _constructorNickname, ...) \
REFLECT_REGISTER_TYPENAME(Type, _constructorNickname)\
static _reflect_TypeRegistration<__VA_ARGS__> \
  _reflect_typeRegistration_##Type##_##_constructorNickname( \
    #Type, \
    _reflect_new<Type, ##__VA_ARGS__>, \
    _reflect_make_shared<Type, ##__VA_ARGS__> \
  );

#endif // _reflect_TypeRegistration_inl
