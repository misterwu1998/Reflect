#if !defined(_reflect_MethodRegistration_inl)
#define _reflect_MethodRegistration_inl

#include "reflect/detail/method/MethodRegistration.hpp"

#include "reflect/detail/method/MethodRegistry.hpp"
#include "reflect/detail/class/normal.hpp"

template <typename ReturnType, typename ... ArgTypes>
_reflect_MethodRegistration<ReturnType, ArgTypes...>::_reflect_MethodRegistration(
  std::string const& className,
  std::string const& methodName,
  reflect_Method const& method
){
  _reflect_MethodRegistry<ReturnType, ArgTypes...>::set(className,methodName,method);
  _reflect_normalRegistry::set(className,methodName,method);
}

#define REFLECT_REGISTER_METHOD(Class,method,ReturnType,...) \
static _reflect_MethodRegistration<ReturnType, ##__VA_ARGS__ > \
  _reflect_methodRegistration_##Class##_##method ( \
    #Class, #method, \
    reflect_Method( \
      (void*)(&Class::method), \
      #method, \
      #ReturnType, \
      #__VA_ARGS__ \
    ) \
  );

#endif // _reflect_MethodRegistration_inl
