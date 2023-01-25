#if !defined(_reflect_MethodRegistration_inl)
#define _reflect_MethodRegistration_inl

#include "reflect/detail/method/MethodRegistration.hpp"

#include "reflect/detail/method/MethodRegistry.hpp"
#include "reflect/detail/type/normal.hpp"

template <typename ReturnType, typename ... ArgTypes>
_reflect_MethodRegistration<ReturnType, ArgTypes...>::_reflect_MethodRegistration(
  std::string const& typeName,
  std::string const& methodName,
  reflect_Method const& method
){
  _reflect_MethodRegistry<ReturnType, ArgTypes...>::set(typeName,methodName,method);
  _reflect_normalRegistry::set(typeName,methodName,method);
}

#define REFLECT_REGISTER_METHOD(Type,method,ReturnType,...) \
static _reflect_MethodRegistration<ReturnType, ##__VA_ARGS__ > \
  _reflect_methodRegistration_##Type##_##method ( \
    #Type, #method, \
    reflect_Method( \
      (void*)(&Type::method), \
      #method, \
      #ReturnType, \
      #__VA_ARGS__ \
    ) \
  );

#endif // _reflect_MethodRegistration_inl
