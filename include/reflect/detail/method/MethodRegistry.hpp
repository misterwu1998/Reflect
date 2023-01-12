#if !defined(_reflect_MethodRegistry_hpp)
#define _reflect_MethodRegistry_hpp

#define _REFLECT_DEBUG 1

#include <memory>
#include <unordered_map>
#include <string>
#if _REFLECT_DEBUG
  #include <iostream>
#endif

#include "reflect/detail/method/Method.hpp"

class reflect_Method;

template <typename ReturnType, typename ... ArgTypes>
class _reflect_MethodRegistry
{
  static std::unordered_map<
    std::string/*类名*/,
    std::unordered_map<
      std::string/*方法名*/,
      reflect_Method>>& getMap()
  {
    static std::unordered_map<
      std::string/*类名*/,
      std::unordered_map<
        std::string/*方法名*/,
        reflect_Method>> methods;
    return methods;
  }

public:

  static void set(
    std::string const& className,
    std::string const& methodName,
    reflect_Method const& method
  ){
    auto& methods = getMap();
    methods[className][methodName] = method;
#if _REFLECT_DEBUG
    std::cout << "_reflect_MethodRegistry::set(): register " << className << "::" << methodName << "()" << std::endl;
#endif
  }

  /// @brief 
  /// @param className 
  /// @param methodName 
  /// @param method [out]
  /// @return 1 有; 0 无
  static int get(
    std::string const& className,
    std::string const& methodName,
    reflect_Method& method
  ){
    auto& methods = getMap();
    auto itc = methods.find(className);
    if(methods.end()==itc)
      return 0;
    auto& c = itc->second;
    auto itm = c.find(methodName);
    if(c.end()==itm)
      return 0;
    method = itm->second;
    return 1;
  }

};

#endif // _reflect_MethodRegistry_hpp
