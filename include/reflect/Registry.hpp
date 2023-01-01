#if !defined(_reflect_Registry_hpp)
#define _reflect_Registry_hpp

#include "util/Singleton.hpp"
#include <unordered_map>
#include <string>
#include <memory>
#include "reflect/Field.hpp"
#include "reflect/Method.hpp"

class reflect_Obj;

QuickSingleton_prologue(reflect_Registry)

private:

  /// <类名, 类的无参构造函数>
  std::unordered_map<std::string, std::shared_ptr<reflect_Obj> (*)(void)> defaultConstructors;

  std::unordered_map<
    std::string,
    std::unordered_map<std::string, reflect_Field>
  > fields;

  std::unordered_map<
    std::string,
    std::unordered_map<std::string, reflect_Method>
  > methods;

public:

  /// @return 反射注册中心全局单例
  static reflect_Registry& get();

  /// @brief 注册一个类的无参构造函数
  /// @param className 
  /// @param constructor 无参数、返回std::shared_ptr<reflect_Obj>型的函数
  /// @return 1表示新增，0表示覆盖，负数表示出错
  int registerConstructor(std::string const& className, std::shared_ptr<reflect_Obj> (*constructor)(void));

  /// @param className 
  /// @return 类的无参构造函数，或NULL表示这个类还未注册无参构造函数
  std::shared_ptr<reflect_Obj> (*getConstructor(std::string const& className))(void);

  /// @brief 
  /// @param className 
  /// @param fieldName 
  /// @param field 
  /// @return 1表示新增，0表示覆盖，负数表示出错
  int registerField(std::string const& className, std::string const& fieldName, reflect_Field const& field);
  
  /// @brief 
  /// @param className 
  /// @param fieldName 
  /// @param field 
  /// @return 1表示成功获取，0表示未注册，负数表示出错
  int getField(std::string const& className, std::string const& fieldName, reflect_Field& field);

  /// @brief 
  /// @param className 
  /// @param methodName 
  /// @param method 
  /// @return 1表示新增，0表示覆盖，负数表示出错
  int registerMethod(std::string const& className, std::string const& methodName, reflect_Method const& method);

  /// @brief 
  /// @param className 
  /// @param methodName 
  /// @param method
  /// @return 1表示成功获取，0表示未注册，负数表示出错
  int getMethod(std::string const& className, std::string const& methodName, reflect_Method& method);

QuickSingleton_epilogue

#endif // _reflect_Registry_hpp
