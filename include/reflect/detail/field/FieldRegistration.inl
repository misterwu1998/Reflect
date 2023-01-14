#if !defined(_reflect_FieldRegistration_inl)
#define _reflect_FieldRegistration_inl

#include "reflect/detail/field/FieldRegistration.hpp"

#include <cstddef>

#include "reflect/detail/field/FieldRegistry.hpp"
#include "reflect/detail/class/normal.hpp"

template <typename FieldType>
_reflect_FieldRegistration<FieldType>::_reflect_FieldRegistration(
  std::string const& className,
  std::string const& fieldName,
  reflect_Field const& field
){
  _reflect_FieldRegistry<FieldType>::set(className,fieldName,field);
  _reflect_normalRegistry::set(className,fieldName,field);
}

/// 【!】不允许注册引用型的成员
#define REFLECT_REGISTER_FIELD(Class,field,FieldType) \
inline int _reflect_toJSON_##Class##_##field (void* p, reflect_JSON& jsonValue) \
{ \
  if(p==NULL) return 0; \
  auto& f = *((FieldType*)p); \
  return toJSON_basic_or_reflectObj<FieldType>(f,jsonValue); \
} \
static _reflect_FieldRegistration<FieldType> _reflect_fieldRegistration_##Class##_##field( \
  #Class, #field, \
  reflect_Field( \
    offsetof(Class,field), \
    sizeof(Class::field), \
    #field, \
    #FieldType, \
    _reflect_toJSON_##Class##_##field \
  ) \
);

#endif // _reflect_FieldRegistration_inl

#if 0 //example
struct Foo{double d; Foo(){} Foo(int i){}};
// REFLECT_REGISTER_FIELD_0_INSTANTIATION(Foo)
REFLECT_REGISTER_FIELD_0_INSTANTIATION(Foo,114514)
REFLECT_REGISTER_FIELD_1(Foo,d,double)
#endif
