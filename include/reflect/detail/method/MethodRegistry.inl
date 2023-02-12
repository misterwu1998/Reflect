#if !defined(_reflect_MethodRegistry_inl)
#define _reflect_MethodRegistry_inl

#include "reflect/detail/method/MethodRegistry.hpp"

#include "reflect/detail/method/Method.hpp"

template <typename ReturnType, typename... ArgTypes>
void _reflect_MethodRegistry<ReturnType, ArgTypes...>::set(
    std::string const &className,
    std::string const &methodName,
    reflect_Method const &method)
{
  auto &methods = getMap();
  methods[className][methodName] = method;
  // std::cout << "_reflect_MethodRegistry::set(): register " << className << "::" << methodName << "()" << std::endl;
}

template <typename ReturnType, typename... ArgTypes>
int _reflect_MethodRegistry<ReturnType, ArgTypes...>::get(
    std::string const &className,
    std::string const &methodName,
    reflect_Method &method)
{
  auto &methods = getMap();
  auto itc = methods.find(className);
  if (methods.end() == itc)
  {
    return -1;
  }

  auto &c = itc->second;
  auto itm = c.find(methodName);
  if (c.end() == itm)
  {
    return -2;
  }

  method = itm->second;
  return 0;
}

#endif // _reflect_MethodRegistry_inl
