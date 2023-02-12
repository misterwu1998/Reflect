#if !defined(_reflect_FieldRegistration_inl)
#define _reflect_FieldRegistration_inl

#include "reflect/detail/field/FieldRegistration.hpp"

#include <cstddef>

#include "reflect/detail/field/FieldRegistry.hpp"
#include "reflect/detail/class/normal.hpp"

template <typename FieldType>
_reflect_FieldRegistration<FieldType>::_reflect_FieldRegistration(
    std::string const &className,
    std::string const &fieldName,
    reflect_Field const &field)
{
  _reflect_FieldRegistry<FieldType>::set(className, fieldName, field);
  _reflect_normalRegistry::set(className, fieldName, field);
}

/// 【!】不允许注册引用型的成员
#define REFLECT_REGISTER_FIELD(Class, field, FieldType)                                      \
  inline int _reflect_toJSON_##Class##_##field(void const *p, reflect_JSON &jsonValue)       \
  {                                                                                          \
    if (p == NULL)                                                                           \
      return -1;                                                                             \
    auto &f = *((FieldType const *)p);                                                       \
    return toJSON_basic_or_reflectObj<FieldType>(f, jsonValue);                              \
  }                                                                                          \
  inline int _reflect_fromJSON_##Class##_##field(reflect_JSON const &jsonValue, void *p)     \
  {                                                                                          \
    if (p == NULL)                                                                           \
      return -1;                                                                             \
    auto &f = *((FieldType *)p);                                                             \
    return fromJSON_basic_or_reflectObj<FieldType>(jsonValue, f);                            \
  }                                                                                          \
  static _reflect_FieldRegistration<FieldType> _reflect_fieldRegistration_##Class##_##field( \
      #Class, #field,                                                                        \
      reflect_Field(                                                                         \
          offsetof(Class, field),                                                            \
          sizeof(Class::field),                                                              \
          #field,                                                                            \
          #FieldType,                                                                        \
          _reflect_toJSON_##Class##_##field,                                                 \
          _reflect_fromJSON_##Class##_##field));

#endif // _reflect_FieldRegistration_inl
