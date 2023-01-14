#if !defined(_reflect_dispatch_inl)
#define _reflect_dispatch_inl

#include "reflect/detail/json/dispatch.hpp"

#include "reflect/detail/class/ClassName.hpp"

template <typename T, 
          EnableIf_ptr<HasMemberFunction_toJSON<T>::result &&
                       std::is_base_of<reflect_Obj,T>::value> p>
inline int toJSON_basic_or_reflectObj(T& obj, reflect_JSON& jsonValue)
{
  obj.__className = reflect_ClassName<T>::registerOrGet();//确保携带着类名
  return obj.toJSON(jsonValue);
}

template <typename T,
          EnableIf_ptr<! HasMemberFunction_toJSON<T>::result> p>
inline int toJSON_basic_or_reflectObj(T& obj, reflect_JSON& jsonValue)
{
  jsonValue = obj;
  return 1;
}

#endif // _reflect_dispatch_inl
