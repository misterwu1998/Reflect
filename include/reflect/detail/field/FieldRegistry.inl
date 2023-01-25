#if !defined(_reflect_FieldRegistry_inl)
#define _reflect_FieldRegistry_inl

#include "reflect/detail/field/FieldRegistry.inl"

#include "reflect/detail/field/Field.hpp"

template <typename FieldType>
void _reflect_FieldRegistry<FieldType>::set(
  std::string const& typeName,
  std::string const& fieldName,
  reflect_Field const& field
){
  auto& fields = getMap();
  fields[typeName][fieldName] = field;
}

template <typename FieldType>
int _reflect_FieldRegistry<FieldType>::get(
  std::string const& typeName,
  std::string const& fieldName,
  reflect_Field& field)
{
  auto& fields = getMap();
  auto itc = fields.find(typeName);
  if(fields.end()==itc)
    return 0;
  auto& c = itc->second;
  auto itf = c.find(fieldName);
  if(c.end()==itf)
    return 0;
  field = itf->second;
  return 1;
}

#endif // _reflect_FieldRegistry_inl
