#if !defined(_reflect_normal_hpp)
#define _reflect_normal_hpp

#include <unordered_map>
#include <string>

class reflect_Field;
class reflect_Method;

/// @brief 每个类单独保存一份类名、reflect_Field和reflect_Method，这个模块与模板无关
class _reflect_normalRegistry
{
  static auto &getClassNamePointers()
  {
    static std::unordered_map<
        std::string /*类名*/,
        std::string const * /*reflect_ClassName<T>的那个单例*/>
        n;
    return n;
  }

  static auto &getFields()
  {
    static std::unordered_map<
        std::string /*类名*/,
        std::unordered_map<
            std::string /*域名*/,
            reflect_Field>>
        f;
    return f;
  }
  static auto &getMethods()
  {
    static std::unordered_map<
        std::string /*类名*/,
        std::unordered_map<
            std::string /*方法名*/,
            reflect_Method>>
        m;
    return m;
  }

public:
  static void set(
      std::string const &className,
      std::string const *pClassNameSingleton)
  {
    auto &m = getClassNamePointers();
    m[className] = pClassNameSingleton;
  }

  /// @return the singleton of className; NULL -- unregistered
  static std::string const *get(std::string const &className)
  {
    auto &m = getClassNamePointers();
    if (m.count(className) == 0)
    {
      return NULL;
    }
    return m[className];
  }

  static void set(
      std::string const &className,
      std::string const &fieldName,
      reflect_Field const &field);

  /// @return 1 OK; 0 fail
  static int get(
      std::string const &className,
      std::string const &fieldName,
      reflect_Field &field);

  /// @brief
  /// @param className
  /// @return std::unordered_map<std::string/*域名*/,reflect_Field> const*; NULL表示className所指的类未注册任何域
  static std::unordered_map<
      std::string /*域名*/,
      reflect_Field> const *
  getFields(std::string const &className)
  {
    auto &m = getFields();
    if (m.count(className) < 1)
      return NULL;
    return &(m[className]);
  }

  static void set(
      std::string const &className,
      std::string const &methodName,
      reflect_Method const &method);

  /// @return 1 OK; 0 fail
  static int get(
      std::string const &className,
      std::string const &methodName,
      reflect_Method &method);

  /// @brief
  /// @param className
  /// @return std::unordered_map<std::string/*域名*/,reflect_Method> const*; NULL表示className所指的类未注册任何方法
  static std::unordered_map<
      std::string /*方法名*/,
      reflect_Method> const *
  getMethods(std::string const &className)
  {
    auto &m = getMethods();
    if (m.count(className) < 1)
      return NULL;
    return &(m[className]);
  }
};

#endif // _reflect_normal_hpp
