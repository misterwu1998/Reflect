#if !defined(_reflect_dispatch_inl)
#define _reflect_dispatch_inl

#include "reflect/detail/json/dispatch.hpp"

#include "reflect/detail/class/ClassName.hpp"

template <typename T, 
          EnableIf_ptr<HasMemberFunction_toJSON<T>::result &&
                       std::is_base_of<reflect_Obj,T>::value> p>
inline int toJSON_basic_or_reflectObj(T const& obj, reflect_JSON& jsonValue)
{
  //确保携带着类名
  auto& pName = const_cast<T&>(obj).__className;
  if(NULL==pName)
  {
    auto& name = reflect_ClassName<T>::registerOrGet();
    pName = (char*)malloc(name.length()+1);
    memcpy(pName, name.data(), name.length());
    pName[ name.length() ] = 0;
  }

  return obj.toJSON(jsonValue);
}

template <typename T,
          EnableIf_ptr<! HasMemberFunction_toJSON<T>::result> p>
inline int toJSON_basic_or_reflectObj(T const& obj, reflect_JSON& jsonValue)
{
  jsonValue = obj;
  return 1;
}

template <typename T,
          EnableIf_ptr<HasMemberFunction_fromJSON<T>::result &&
                       std::is_base_of<reflect_Obj,T>::value> p>
inline int fromJSON_basic_or_reflectObj(reflect_JSON const& jsonValue, T& obj)
{
  //确保携带着类名
  auto& pName = obj.__className;
  if(NULL==p)
  {
    auto& name = reflect_ClassName<T>::registerOrGet();
    pName = (char*)malloc(name.length()+1);
    memcpy(pName, name.data(), name.length());
    pName[ name.length() ] = 0;
  }
  
  return obj.fromJSON(jsonValue);
}

template <typename T,
          EnableIf_ptr<! HasMemberFunction_fromJSON<T>::result> p>
inline int fromJSON_basic_or_reflectObj(reflect_JSON const& jsonValue, T& obj)
{
  jsonValue.get_to(obj);
  return 1;
}

#endif // _reflect_dispatch_inl
