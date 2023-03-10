#if !defined(__reflect_Method_hpp)
#define __reflect_Method_hpp

#include <string>

/// @brief 函数无关于类型的属性，即，无论什么类的什么方法，都会有以下属性
class reflect_Method
{
public:
  void *__functor; // 类成员函数的地址，统统擦除掉类型
  std::string name;
  std::string argTypeNames;
  std::string retTypeName;
  reflect_Method() : __functor(NULL) {}
  reflect_Method(void *functor,
                 std::string const &name,
                 std::string const &retTypeName = "",
                 std::string const &argTypeNames = "")
      : __functor(functor), name(name),
        retTypeName(retTypeName.empty() ? "void" : retTypeName),
        argTypeNames(argTypeNames.empty() ? "void" : argTypeNames) {}

  inline void *getFunctor() const { return __functor; }
  inline std::string const &getName() const { return name; }
  inline std::string const &getRetTypeName() const { return retTypeName; }
  inline std::string const &getArgTypeNames() const { return argTypeNames; }
};

#endif // __reflect_Method_hpp
