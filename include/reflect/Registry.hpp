#if !defined(_reflect_Registry_hpp)
#define _reflect_Registry_hpp

#include "util/Singleton.hpp"
#include <unordered_map>
#include <string>

class reflect_Obj;

QuickSingleton_prologue(reflect_Registry)

private:

  /// <类名, 类的无参构造函数>
  std::unordered_map<std::string, reflect_Obj* (*)(void)> defaultConstructors;

public:

  /// @return 反射注册中心全局单例
  static reflect_Registry& get();

  /// @brief 注册一个类的无参构造函数
  /// @param className 
  /// @param constructor 无参数、返回 reflect_Obj* 型的函数
  /// @return 1表示新增，0表示覆盖，负数表示出错
  int registerConstructor(std::string const& className, reflect_Obj* (*constructor)(void));

  /// @param className 
  /// @return 类的无参构造函数，或NULL表示这个类还未注册无参构造函数
  reflect_Obj* (*getConstructor(std::string const& className))(void);

QuickSingleton_epilogue

#endif // _reflect_Registry_hpp
