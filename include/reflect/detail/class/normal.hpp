#if !defined(_reflect_normal_hpp)
#define _reflect_normal_hpp

#include <unordered_map>
#include <string>

#include "reflect/detail/field/Field.hpp"
#include "reflect/detail/method/Method.hpp"

/// @brief 每个类单独保存一份reflect_Field和reflect_Method，这个模块与模板无关
class _reflect_normalRegistry
{
  static auto& getFields(){
    static std::unordered_map<
      std::string/*类名*/,
      std::unordered_map<
        std::string/*域名*/,
        reflect_Field>> f;
    return f;
  }
  static auto& getMethods(){
    static std::unordered_map<
      std::string/*类名*/,
      std::unordered_map<
        std::string/*方法名*/,
        reflect_Method>> m;
    return m;
  }

public:

  static void set(
    std::string const& className,
    std::string const& fieldName,
    reflect_Field const& field
  ){
    getFields()[className][fieldName] = field;
  }

  static int get(
    std::string const& className,
    std::string const& fieldName,
    reflect_Field& field
  ){
    auto& f = getFields();
    auto itc = f.find(className);
    if(f.end()==itc)
      return 0;
    auto& c = itc->second;
    auto itf = c.find(fieldName);
    if(c.end()==itf)
      return 0;
    field = itf->second;
    return 1;
  }

  /// @brief 
  /// @param className 
  /// @return std::unordered_map<std::string/*域名*/,reflect_Field> const*; NULL表示className所指的类未注册任何域
  static std::unordered_map<
    std::string/*域名*/,
    reflect_Field> const* getFields(std::string const& className){
    auto& m = getFields();
    if(m.count(className)<1)
      return NULL;
    return &(m[className]);
  }

  static void set(
    std::string const& className,
    std::string const& methodName,
    reflect_Method const& method
  ){
    getMethods()[className][methodName] = method;
  }

  static int get(
    std::string const& className,
    std::string const& methodName,
    reflect_Method& method
  ){
    auto& f = getMethods();
    auto itc = f.find(className);
    if(f.end()==itc)
      return 0;
    auto& c = itc->second;
    auto itf = c.find(methodName);
    if(c.end()==itf)
      return 0;
    method = itf->second;
    return 1;
  }

  /// @brief 
  /// @param className 
  /// @return std::unordered_map<std::string/*域名*/,reflect_Method> const*; NULL表示className所指的类未注册任何方法
  static std::unordered_map<
    std::string/*方法名*/,
    reflect_Method> const* getMethods(std::string const& className){
    auto& m = getMethods();
    if(m.count(className)<1)
      return NULL;
    return &(m[className]);
  }

};

#endif // _reflect_normal_hpp
