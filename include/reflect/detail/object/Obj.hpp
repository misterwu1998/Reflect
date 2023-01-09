#if !defined(_reflect_Obj_hpp)
#define _reflect_Obj_hpp

#include <string>
#include "reflect/detail/field/FieldRegistration.hpp"
#include "reflect/detail/method/MethodRegistration.hpp"
#include "reflect/detail/class/ClassRegistration.hpp"

class reflect_Obj
{
public:

  /// @brief 只要是借助反射机制动态获取的对象，这一字符串用于记录实际类名
  std::string __className;
  
  reflect_Obj(){}
  virtual ~reflect_Obj(){}

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @param value 
  /// @return 1 OK; 0 反射类__className没有注册类型为FieldType、名称为fieldName的域
  template <typename FieldType>
  int get(std::string const& fieldName, FieldType& value)
  {
    _reflect_Field f;
    if(_reflect_FieldRegistration<FieldType>::get(__className,fieldName,f))//有
    {
      value = *(
        (FieldType*)
        (
          ((void*)this) +
          f.offset
        )
      );
      return 1;
    }
    else return 0;
  }

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @param value 
  /// @return 1 OK; 0 反射类__className没有注册类型为FieldType、名称为fieldName的域
  template <typename FieldType>
  int set(std::string const& fieldName, FieldType const& value)
  {
    _reflect_Field f;
    if(_reflect_FieldRegistration<FieldType>::get(__className,fieldName,f))//有
    {
      *(
        (FieldType*)
        (
          ((void*)this) +
          f.offset
        )
      ) = value;
      return 1;
    }
    else return 0;
  }

  /// @brief 调用有返回值的函数
  /// @tparam ReturnType 
  /// @tparam ...ArgTypes 
  /// @param methodName 
  /// @param result 
  /// @param ...args 
  /// @return 1 OK; 0 反射类__className没有注册返回值类型为ReturnType、参数列表为ArgTypes...的方法
  template <typename ReturnType, typename ... ArgTypes>
  int func(
    std::string const& methodName,
    ReturnType& result,
    ArgTypes... args
  ){
    using Func = ReturnType (*)(ArgTypes...);
    _reflect_Method m;
    if(_reflect_MethodRegistry<ReturnType, ArgTypes...>::get(__className,methodName,m))//有
    {
      auto f = (Func)m.getFunctor();
      result = f(std::forward<ArgTypes>(args)...);
      return 1;
    }
    else return 0;
  }

  /// @brief 调用无返回值的过程
  /// @tparam ...ArgTypes 
  /// @param methodName 
  /// @param ...args 
  /// @return 1 OK; 0 反射类__className没有注册返回值为void、参数列表为ArgTypes...的方法
  template <typename ... ArgTypes>
  int pro(
    std::string const& methodName,
    ArgTypes... args
  ){
    using Func = void (*)(ArgTypes...);
    _reflect_Method m;
    if(_reflect_MethodRegistry<void, ArgTypes...>::get(__className,methodName,m))//有
    {
      auto f = (Func)m.getFunctor();
      f(std::forward<ArgTypes>(args)...);
      return 1;
    }
    else return 0;
  }

};

using reflect_Ptr = std::shared_ptr<reflect_Obj>;

template <typename ... ConstructorArgTypes>
inline reflect_Ptr reflect_new(
  std::string const& className,
  ConstructorArgTypes... args
){
  auto f = _reflect_ClassRegistry<ConstructorArgTypes...>::get(className);
  if(NULL==f)
    return nullptr;
  return f(std::forward<ConstructorArgTypes>(args)...);
}

template <typename ... ConstructorArgTypes>
inline reflect_Ptr reflect_new(
  const char* className,
  ConstructorArgTypes... args
){
  auto f = _reflect_ClassRegistry<ConstructorArgTypes...>::get(std::string(className));
  if(NULL==f)
    return nullptr;
  return f(std::forward<ConstructorArgTypes>(args)...);
}

#endif // _reflect_Obj_hpp
