#if !defined(_reflect_MethodRegistry_hpp)
#define _reflect_MethodRegistry_hpp

#include <memory>
#include <unordered_map>
#include <string>

#include "reflect/detail/method/Method.hpp"

class _reflect_Method;

template <typename ReturnType, typename ... ArgTypes>
class _reflect_MethodRegistry
{
  static std::unordered_map<
    std::string/*类名*/,
    std::unordered_map<
      std::string/*方法名*/,
      _reflect_Method>>* methods;

public:

  static void set(
    std::string const& className,
    std::string const& methodName,
    _reflect_Method const& method
  ){
    if(NULL==methods)
      methods = new std::unordered_map<
        std::string/*类名*/,
        std::unordered_map<
          std::string/*方法名*/,
          _reflect_Method>>;
    (*methods)[className][methodName] = method;
  }

  /// @brief 
  /// @param className 
  /// @param methodName 
  /// @param method [out]
  /// @return 1 有; 0 无
  static int get(
    std::string const& className,
    std::string const& methodName,
    _reflect_Method& method
  ){
    if(NULL==methods)
      return 0;
    auto itc = methods->find(className);
    if(methods->end()==itc)
      return 0;
    auto& c = itc->second;
    auto itm = c.find(methodName);
    if(c.end()==itm)
      return 0;
    method = itm->second;
    return 1;
  }

};

template <typename ReturnType, typename ... ArgTypes>
std::unordered_map<
  std::string/*类名*/,
  std::unordered_map<
    std::string/*方法名*/,
    _reflect_Method>>* _reflect_MethodRegistry<ReturnType, ArgTypes...>::methods = NULL;

#endif // _reflect_MethodRegistry_hpp
