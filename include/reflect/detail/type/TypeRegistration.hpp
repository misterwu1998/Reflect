#if !defined(_reflect_TypeRegistration_hpp)
#define _reflect_TypeRegistration_hpp 

#include <string>
#include <memory>

class reflect_Obj;

template <typename Type>
class _reflect_TypeNameRegistration{
public:
  _reflect_TypeNameRegistration(std::string const& name);
};

template <typename ...ConstructorArgTypes>
class _reflect_TypeRegistration
{
public:
  /// @brief 注册名为typeName的类的构造器
  /// @param typeName 
  /// @param _make_shared 
  _reflect_TypeRegistration(
    std::string const& typeName,
    reflect_Obj* (*_new)(ConstructorArgTypes...),
    std::shared_ptr<reflect_Obj> (*_make_shared)(ConstructorArgTypes...)
  );
};

#endif // _reflect_TypeRegistration_hpp 
