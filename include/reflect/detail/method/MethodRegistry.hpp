#if !defined(_reflect_MethodRegistry_hpp)
#define _reflect_MethodRegistry_hpp

#include <memory>
#include <unordered_map>
#include <string>

class reflect_Method;

template <typename ReturnType, typename... ArgTypes>
class _reflect_MethodRegistry
{
  static std::unordered_map<
      std::string /*类名*/,
      std::unordered_map<
          std::string /*方法名*/,
          reflect_Method>> &
  getMap()
  {
    static std::unordered_map<
        std::string /*类名*/,
        std::unordered_map<
            std::string /*方法名*/,
            reflect_Method>>
        methods;
    return methods;
  }

public:
  static void set(
      std::string const &className,
      std::string const &methodName,
      reflect_Method const &method);

  /// @brief
  /// @param className
  /// @param methodName
  /// @param method [out]
  /// @return -1 -- class unregistered;
  //          -2 -- method unregistered
  static int get(
      std::string const &className,
      std::string const &methodName,
      reflect_Method &method);
};

#endif // _reflect_MethodRegistry_hpp
