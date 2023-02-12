#if !defined(_reflect_normal_inl)
#define _reflect_normal_inl

#include "reflect/detail/class/normal.hpp"

#include "reflect/detail/field/Field.hpp"
#include "reflect/detail/method/Method.hpp"

void _reflect_normalRegistry::set(
    std::string const &className,
    std::string const &fieldName,
    reflect_Field const &field)
{
  getFields()[className][fieldName] = field;
}

int _reflect_normalRegistry::get(
    std::string const &className,
    std::string const &fieldName,
    reflect_Field &field)
{
  auto &f = getFields();
  auto itc = f.find(className);
  if (f.end() == itc)
  {
    return -1;
  }

  auto &c = itc->second;
  auto itf = c.find(fieldName);
  if (c.end() == itf)
  {
    return -2;
  }

  field = itf->second;
  return 0;
}

void _reflect_normalRegistry::set(
    std::string const &className,
    std::string const &methodName,
    reflect_Method const &method)
{
  getMethods()[className][methodName] = method;
}

int _reflect_normalRegistry::get(
    std::string const &className,
    std::string const &methodName,
    reflect_Method &method)
{
  auto &f = getMethods();
  auto itc = f.find(className);
  if (f.end() == itc)
  {
    return -1;
  }
  auto &c = itc->second;
  auto itf = c.find(methodName);
  if (c.end() == itf)
  {
    return -2;
  }

  method = itf->second;
  return 0;
}

#endif // _reflect_normal_inl
