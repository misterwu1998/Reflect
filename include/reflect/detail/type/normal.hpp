#if !defined(_reflect_normal_hpp)
#define _reflect_normal_hpp

#include <unordered_map>
#include <string>

class reflect_Field;
class reflect_Method;

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
    std::string const& typeName,
    std::string const& fieldName,
    reflect_Field const& field
  );

  static int get(
    std::string const& typeName,
    std::string const& fieldName,
    reflect_Field& field
  );

  /// @brief 
  /// @param typeName 
  /// @return std::unordered_map<std::string/*域名*/,reflect_Field> const*; NULL表示typeName所指的类未注册任何域
  static std::unordered_map<
    std::string/*域名*/,
    reflect_Field> const* getFields(std::string const& typeName){
    auto& m = getFields();
    if(m.count(typeName)<1)
      return NULL;
    return &(m[typeName]);
  }

  static void set(
    std::string const& typeName,
    std::string const& methodName,
    reflect_Method const& method
  );

  static int get(
    std::string const& typeName,
    std::string const& methodName,
    reflect_Method& method
  );

  /// @brief 
  /// @param typeName 
  /// @return std::unordered_map<std::string/*域名*/,reflect_Method> const*; NULL表示typeName所指的类未注册任何方法
  static std::unordered_map<
    std::string/*方法名*/,
    reflect_Method> const* getMethods(std::string const& typeName){
    auto& m = getMethods();
    if(m.count(typeName)<1)
      return NULL;
    return &(m[typeName]);
  }

};

#endif // _reflect_normal_hpp
