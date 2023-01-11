#if !defined(_reflect_FieldRegistration_hpp)
#define _reflect_FieldRegistration_hpp

#include "reflect/detail/field/FieldRegistry.hpp"

class _reflect_Field;

template <typename FieldType>
class _reflect_FieldRegistration
{
public:
  _reflect_FieldRegistration(
    std::string const& className,
    std::string const& fieldName,
    _reflect_Field const& field
  ){
    _reflect_FieldRegistry<FieldType>::set(className,fieldName,field);
  }
};

#define REFLECT_REGISTER_FIELD_PREPARE_INSTANCE(Class, ...)\
static Class _reflect_forRegistration_##Class (__VA_ARGS__);

/// 【!】不允许注册引用型的成员
#define REFLECT_REGISTER_FIELD_REGISTER(Class,field,FieldType) \
static _reflect_FieldRegistration<FieldType> _reflect_fieldRegistration_##Class##_##field( \
  #Class, #field, \
  _reflect_Field( \
    ((uint64_t)(&((_reflect_forRegistration_##Class).field))) - \
      ((uint64_t)(&(_reflect_forRegistration_##Class))), \
    sizeof((_reflect_forRegistration_##Class).field), \
    #field \
  ) \
);

#endif // _reflect_FieldRegistration_hpp

#if 0 //example
struct Foo{double d; Foo(){} Foo(int i){}};
// REFLECT_REGISTER_FIELD_0_INSTANTIATION(Foo)
REFLECT_REGISTER_FIELD_0_INSTANTIATION(Foo,114514)
REFLECT_REGISTER_FIELD_1(Foo,d,double)
#endif
