#if !defined(_reflect_ClassRegistration_hpp)
#define _reflect_ClassRegistration_hpp 

#include <string>
#include <memory>

class reflect_Obj;

template <typename ...ConstructorArgTypes>
class _reflect_ClassRegistration
{
public:
  /// @brief 注册名为className的类的构造器
  /// @param className 
  /// @param _make_shared 
  _reflect_ClassRegistration(
    std::string const& className,
    reflect_Obj* (*_new)(ConstructorArgTypes...),
    std::shared_ptr<reflect_Obj> (*_make_shared)(ConstructorArgTypes...)
  );
};

template <typename Class>
class _reflect_ClassNameRegistration{
public:
  _reflect_ClassNameRegistration(std::string const& name);
};

#endif // _reflect_ClassRegistration_hpp 
