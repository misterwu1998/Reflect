#if !defined(_reflect_Obj_inl)
#define _reflect_Obj_inl

#include "reflect/detail/object/Obj.hpp"

#include "reflect/detail/field/FieldRegistration.hpp"
#include "reflect/detail/method/MethodRegistration.hpp"
#include "reflect/detail/type/TypeRegistration.hpp"
#include "reflect/detail/field/Field.hpp"
#include "reflect/detail/method/Method.hpp"
#include "reflect/detail/json/JSON_decl.hpp"

template <typename FieldType>
FieldType* reflect_Obj::access(std::string const& fieldName){
  reflect_Field f;
  if(_reflect_FieldRegistry<FieldType>::get(__typeName,fieldName,f))//有
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
FieldType const* reflect_Obj::access(std::string const& fieldName) const{
  reflect_Field f;
  if(_reflect_FieldRegistry<FieldType>::get(__typeName,fieldName,f))//有
    return (
      (FieldType const*)
      (
        ((void const*)this) +
        f.offset
      )
    );
  else return NULL;
}

template <typename FieldType>
int reflect_Obj::get(std::string const& fieldName, FieldType& value) const
{
  reflect_Field f;
  if(_reflect_FieldRegistry<FieldType>::get(__typeName,fieldName,f))//有
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
  if(_reflect_FieldRegistry<FieldType>::get(__typeName,fieldName,f))//有
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
  if(_reflect_MethodRegistry<ReturnType, ArgTypes...>::get(__typeName,methodName,m))//有
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
  if(_reflect_MethodRegistry<void, ArgTypes...>::get(__typeName,methodName,m))//有
  {
    auto f = (Func)m.getFunctor();
    f(this, std::forward<ArgTypes>(args)...);
    return 1;
  }
  else return 0;
}

std::unordered_map<
  std::string/*域名*/,
  reflect_Field> const* reflect_Obj::getFields() const
{
  return _reflect_normalRegistry::getFields(__typeName);
}

std::unordered_map<
  std::string/*方法名*/,
  reflect_Method> const* reflect_Obj::getMethods() const
{
  return _reflect_normalRegistry::getMethods(__typeName);
}

int reflect_Obj::toJSON(reflect_JSON& json) const{
  int (*f)(void const*, reflect_JSON&);
  int ret;
  auto fields = getFields();
  for(auto& kv: *fields){
    auto& field = kv.second;
    f = field.toJSON;
    if(f(((void const*)this) + field.offset, json[field.name])){}
    else return 0;// fail
  }
  return 1;
}

int reflect_Obj::fromJSON(reflect_JSON const& json){
  int (*f)(reflect_JSON const&, void*);
  int ret;
  auto fields = getFields();
  for(auto& kv: *fields){
    auto& field = kv.second;
    if(json.contains(field.name)){}
    else return 0;
    f = field.fromJSON;
    if(f(json[field.name], ((void*)this) + field.offset)){}
    else return 0;
  }
  return 1;
}

#define REFLECT_ACCESS(pointer_or_sharedPtr, fieldName, type)\
(pointer_or_sharedPtr ? pointer_or_sharedPtr->access<type>(fieldName) : ((type*)NULL))

template <typename ... ConstructorArgTypes>
inline reflect_Obj* reflect_new(
  std::string const& typeName,
  ConstructorArgTypes... args
){
  auto f = _reflect_TypeRegistry<ConstructorArgTypes...>::get_new(typeName);
  if(f){
    reflect_Obj* obj = f(std::forward<ConstructorArgTypes>(args)...);
    obj->__typeName = typeName;
    return obj;
  }
  return NULL;
}

template <typename ... ConstructorArgTypes>
inline reflect_Obj* reflect_new(
  const char* typeName,
  ConstructorArgTypes... args
){
  return reflect_new<ConstructorArgTypes...>(
    std::string(typeName), 
    std::forward<ConstructorArgTypes>(args)...);
}

template <typename ... ConstructorArgTypes>
inline reflect_Ptr reflect_share(
  std::string const& typeName,
  ConstructorArgTypes... args
){
  auto f = _reflect_TypeRegistry<ConstructorArgTypes...>::get_make_shared(typeName);
  if(f){
    reflect_Ptr obj = f(std::forward<ConstructorArgTypes>(args)...);
    obj->__typeName = typeName;
    return obj;
  }
  return nullptr;
}

template <typename ... ConstructorArgTypes>
inline reflect_Ptr reflect_share(
  const char* typeName,
  ConstructorArgTypes... args
){
  return reflect_share<ConstructorArgTypes...>(
    std::string(typeName), 
    std::forward<ConstructorArgTypes>(args)...);
}

#endif // _reflect_Obj_inl
