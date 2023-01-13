#if !defined(_reflect_Obj_inl)
#define _reflect_Obj_inl

#include "reflect/detail/object/Obj.hpp"

#include "reflect/detail/field/FieldRegistration.hpp"
#include "reflect/detail/method/MethodRegistration.hpp"
#include "reflect/detail/class/ClassRegistration.hpp"
#include "reflect/detail/field/Field.hpp"
#include "reflect/detail/method/Method.hpp"

template <typename FieldType>
FieldType* reflect_Obj::access(std::string const& fieldName){
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

template <typename FieldType>
int reflect_Obj::get(std::string const& fieldName, FieldType& value)
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

template <typename FieldType>
int reflect_Obj::set(std::string const& fieldName, FieldType const& value)
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

template <typename ReturnType, typename ... ArgTypes>
int reflect_Obj::func(
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

template <typename ... ArgTypes>
int reflect_Obj::pro(
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
  reflect_Field> const* reflect_Obj::getFields()
{
  return _reflect_normalRegistry::getFields(__className);
}

std::unordered_map<
  std::string/*方法名*/,
  reflect_Method> const* reflect_Obj::getMethods()
{
  return _reflect_normalRegistry::getMethods(__className);
}

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

#endif // _reflect_Obj_inl