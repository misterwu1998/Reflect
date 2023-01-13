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
  std::string getClassName() const{return __className;}
  
  reflect_Obj(){}
  virtual ~reflect_Obj(){}

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @return 类型为FieldType、名为fieldName的成员变量的指针; NULL 反射类__className没有注册类型为FieldType、名称为fieldName的域
  template <typename FieldType>
  FieldType* access(std::string const& fieldName){
    reflect_Field f;
    if(_reflect_FieldRegistry<FieldType>::get(__className,fieldName,f))//有
      return (
        (FieldType*)
        (
          ((void*)this) +
          f.offset
        )
      );
    else return NULL;
  }

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @param value 
  /// @return 1 OK; 0 反射类__className没有注册类型为FieldType、名称为fieldName的域
  template <typename FieldType>
  int get(std::string const& fieldName, FieldType& value)
  {
    reflect_Field f;
    if(_reflect_FieldRegistry<FieldType>::get(__className,fieldName,f))//有
    {
      value = *(
        (FieldType const*)
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
    reflect_Field f;
    if(_reflect_FieldRegistry<FieldType>::get(__className,fieldName,f))//有
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
    using Func = ReturnType (*)(reflect_Obj* const, ArgTypes...);
    reflect_Method m;
    if(_reflect_MethodRegistry<ReturnType, ArgTypes...>::get(__className,methodName,m))//有
    {
      auto f = (Func)m.getFunctor();
      result = f(this, std::forward<ArgTypes>(args)...);
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
    using Func = void (*)(reflect_Obj* const, ArgTypes...);
    reflect_Method m;
    if(_reflect_MethodRegistry<void, ArgTypes...>::get(__className,methodName,m))//有
    {
      auto f = (Func)m.getFunctor();
      f(this, std::forward<ArgTypes>(args)...);
      return 1;
    }
    else return 0;
  }

  std::unordered_map<
    std::string/*域名*/,
    reflect_Field> const* getFields()
  {
    return _reflect_normalRegistry::getFields(__className);
  }
  
  std::unordered_map<
    std::string/*方法名*/,
    reflect_Method> const* getMethods()
  {
    return _reflect_normalRegistry::getMethods(__className);
  }

};

#define REFLECT_ACCESS(pointer_or_sharedPtr, fieldName, type)\
(pointer_or_sharedPtr ? pointer_or_sharedPtr->access<type>(fieldName) : ((type*)NULL))

template <typename ... ConstructorArgTypes>
inline reflect_Obj* reflect_new(
  std::string const& className,
  ConstructorArgTypes... args
){
  auto f = _reflect_ClassRegistry<ConstructorArgTypes...>::get_new(className);
  if(f){
    reflect_Obj* obj = f(std::forward<ConstructorArgTypes>(args)...);
    obj->__className = className;
    return obj;
  }
  return NULL;
}

template <typename ... ConstructorArgTypes>
inline reflect_Obj* reflect_new(
  const char* className,
  ConstructorArgTypes... args
){
  return reflect_new<ConstructorArgTypes...>(
    std::string(className), 
    std::forward<ConstructorArgTypes>(args)...);
}

using reflect_Ptr = std::shared_ptr<reflect_Obj>;

template <typename ... ConstructorArgTypes>
inline reflect_Ptr reflect_share(
  std::string const& className,
  ConstructorArgTypes... args
){
  auto f = _reflect_ClassRegistry<ConstructorArgTypes...>::get_make_shared(className);
  if(f){
    reflect_Ptr obj = f(std::forward<ConstructorArgTypes>(args)...);
    obj->__className = className;
    return obj;
  }
  return nullptr;
}

template <typename ... ConstructorArgTypes>
inline reflect_Ptr reflect_share(
  const char* className,
  ConstructorArgTypes... args
){
  return reflect_share<ConstructorArgTypes...>(
    std::string(className), 
    std::forward<ConstructorArgTypes>(args)...);
}

#endif // _reflect_Obj_hpp
