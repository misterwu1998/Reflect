#if !defined(_reflect_FieldRegistration_hpp)
#define _reflect_FieldRegistration_hpp

#include <cstddef>

#include "reflect/detail/field/FieldRegistry.hpp"
#include "reflect/detail/class/normal.hpp"

class reflect_Field;

template <typename FieldType>
class _reflect_FieldRegistration
{
public:
  _reflect_FieldRegistration(
    std::string const& className,
    std::string const& fieldName,
    reflect_Field const& field
  ){
    _reflect_FieldRegistry<FieldType>::set(className,fieldName,field);
    _reflect_normalRegistry::set(className,fieldName,field);
  }
};

/// 【!】不允许注册引用型的成员
#define REFLECT_REGISTER_FIELD(Class,field,FieldType) \
static _reflect_FieldRegistration<FieldType> _reflect_fieldRegistration_##Class##_##field( \
  #Class, #field, \
  reflect_Field( \
    offsetof(Class,field), \
    sizeof(Class::field), \
    #field, \
    #FieldType \
  ) \
);

#endif // _reflect_FieldRegistration_hpp

#if 0 //example
struct Foo{double d; Foo(){} Foo(int i){}};
// REFLECT_REGISTER_FIELD_0_INSTANTIATION(Foo)
REFLECT_REGISTER_FIELD_0_INSTANTIATION(Foo,114514)
REFLECT_REGISTER_FIELD_1(Foo,d,double)
#endif
