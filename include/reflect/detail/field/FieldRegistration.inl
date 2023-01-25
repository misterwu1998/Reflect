#if !defined(_reflect_FieldRegistration_inl)
#define _reflect_FieldRegistration_inl

#include "reflect/detail/field/FieldRegistration.hpp"

#include <cstddef>

#include "reflect/detail/field/FieldRegistry.hpp"
#include "reflect/detail/type/normal.hpp"
#include "reflect/detail/type/TypeName.hpp"

template <typename FieldType>
_reflect_FieldRegistration<FieldType>::_reflect_FieldRegistration(
  std::string const& typeName,
  std::string const& fieldName,
  reflect_Field const& field
){
  _reflect_FieldRegistry<FieldType>::set(typeName,fieldName,field);
  _reflect_normalRegistry::set(typeName,fieldName,field);
}

/// 【!】不允许注册引用型的成员
#define REFLECT_REGISTER_FIELD_OLD(Type,field,FieldType) \
inline int _reflect_toJSON_##Type##_##field (void const* p, reflect_JSON& jsonValue) \
{ \
  if(p==NULL) return 0; \
  auto& f = *((FieldType const*)p); \
  return toJSON_basic_or_reflectObj<FieldType>(f,jsonValue); \
} \
inline int _reflect_fromJSON_##Type##_##field (reflect_JSON const& jsonValue, void* p) \
{ \
  if(p==NULL) return 0; \
  auto& f = *((FieldType*)p); \
  return fromJSON_basic_or_reflectObj<FieldType>(jsonValue,f); \
} \
static _reflect_FieldRegistration<FieldType> _reflect_fieldRegistration_##Type##_##field( \
  #Type, #field, \
  reflect_Field( \
    offsetof(Type,field), \
    sizeof(Type::field), \
    #field, \
    #FieldType, \
    _reflect_toJSON_##Type##_##field, \
    _reflect_fromJSON_##Type##_##field \
  ) \
);

#define REFLECT_REGISTER_FIELD(Type,field) \
inline int _reflect_toJSON_##Type##_##field (void const* p, reflect_JSON& jsonValue) \
{ \
  if(p==NULL) return 0; \
  auto& f = *((decltype(Type::field) const*)p); \
  return toJSON_basic_or_reflectObj<decltype(Type::field)>(f,jsonValue); \
} \
inline int _reflect_fromJSON_##Type##_##field (reflect_JSON const& jsonValue, void* p) \
{ \
  if(p==NULL) return 0; \
  auto& f = *((decltype(Type::field)*)p); \
  return fromJSON_basic_or_reflectObj<decltype(Type::field)>(jsonValue,f); \
} \
static _reflect_FieldRegistration<decltype(Type::field)> _reflect_fieldRegistration_##Type##_##field( \
  #Type, #field, \
  reflect_Field( \
    offsetof(Type,field), \
    sizeof(Type::field), \
    #field, \
    reflect_TypeName<decltype(Type::field)>::registerOrGet(), \
    _reflect_toJSON_##Type##_##field, \
    _reflect_fromJSON_##Type##_##field \
  ) \
);

#endif // _reflect_FieldRegistration_inl

#if 0 //example
struct Foo{double d; Foo(){} Foo(int i){}};
// REFLECT_REGISTER_FIELD_OLD_0_INSTANTIATION(Foo)
REFLECT_REGISTER_FIELD_OLD_0_INSTANTIATION(Foo,114514)
REFLECT_REGISTER_FIELD_OLD_1(Foo,d,double)
#endif
