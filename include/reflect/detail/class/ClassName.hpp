#if !defined(_reflect_ClassName_hpp)
#define _reflect_ClassName_hpp

#include <string>

// 没有注册的类不会有
template <typename Class>
class reflect_ClassName
{
public:
  /// @brief 首次调用——指定类名；后续调用——获取类名的const引用
  /// @param name
  /// @return
  static std::string const &registerOrGet(std::string const &name = "")
  {
    static std::string n(name);
    return n;
  }
};

#endif // _reflect_ClassName_hpp
