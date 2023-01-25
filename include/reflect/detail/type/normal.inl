#if !defined(_reflect_normal_inl)
#define _reflect_normal_inl

#include "reflect/detail/type/normal.hpp"

#include "reflect/detail/field/Field.hpp"
#include "reflect/detail/method/Method.hpp"

void _reflect_normalRegistry::set(
  std::string const& typeName,
  std::string const& fieldName,
  reflect_Field const& field
){
  getFields()[typeName][fieldName] = field;
}

int _reflect_normalRegistry::get(
  std::string const& typeName,
  std::string const& fieldName,
  reflect_Field& field
){
  auto& f = getFields();
  auto itc = f.find(typeName);
  if(f.end()==itc)
    return 0;
  auto& c = itc->second;
  auto itf = c.find(fieldName);
  if(c.end()==itf)
    return 0;
  field = itf->second;
  return 1;
}

void _reflect_normalRegistry::set(
  std::string const& typeName,
  std::string const& methodName,
  reflect_Method const& method
){
  getMethods()[typeName][methodName] = method;
}

int _reflect_normalRegistry::get(
  std::string const& typeName,
  std::string const& methodName,
  reflect_Method& method
){
  auto& f = getMethods();
  auto itc = f.find(typeName);
  if(f.end()==itc)
    return 0;
  auto& c = itc->second;
  auto itf = c.find(methodName);
  if(c.end()==itf)
    return 0;
  method = itf->second;
  return 1;
}

#endif // _reflect_normal_inl
