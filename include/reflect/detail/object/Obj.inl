#if !defined(_reflect_Obj_inl)
#define _reflect_Obj_inl

#include "reflect/detail/object/Obj.hpp"

#include "reflect/detail/field/FieldRegistration.hpp"
#include "reflect/detail/method/MethodRegistration.hpp"
#include "reflect/detail/class/ClassRegistration.hpp"
#include "reflect/detail/field/Field.hpp"
#include "reflect/detail/method/Method.hpp"
#include "reflect/detail/json/JSON_decl.hpp"

template <typename FieldType>
FieldType *reflect_Obj::access(std::string const &fieldName)
{
  if (NULL == _p_singleton_className)
  {
    return NULL;
  }

  reflect_Field f;
  if (0<=_reflect_FieldRegistry<FieldType>::get(*_p_singleton_className, fieldName, f)) // 有
    return (
        (FieldType *)(((void *)this) +
                      f.offset));
  else
    return NULL;
}

template <typename FieldType>
FieldType const *reflect_Obj::access(std::string const &fieldName) const
{
  if (NULL == _p_singleton_className)
  {
    return NULL;
  }

  reflect_Field f;
  if (0<=_reflect_FieldRegistry<FieldType>::get(*_p_singleton_className, fieldName, f)) // 有
    return (
        (FieldType const *)(((void const *)this) +
                            f.offset));
  else
    return NULL;
}

template <typename FieldType>
int reflect_Obj::get(std::string const &fieldName, FieldType &value) const
{
  if (NULL == _p_singleton_className)
  {
    return -1;
  }

  reflect_Field f;
  if (0<=_reflect_FieldRegistry<FieldType>::get(*_p_singleton_className, fieldName, f)) // 有
  {
    value = *(
        (FieldType const *)(((void *)this) +
                            f.offset));
    return 0;
  }
  else
    return -2;
}

template <typename FieldType>
int reflect_Obj::set(std::string const &fieldName, FieldType const &value)
{
  if (NULL == _p_singleton_className)
  {
    return -1;
  }

  reflect_Field f;
  if (0<=_reflect_FieldRegistry<FieldType>::get(*_p_singleton_className, fieldName, f)) // 有
  {
    *(
        (FieldType *)(((void *)this) +
                      f.offset)) = value;
    return 0;
  }
  else
    return -2;
}

template <typename ReturnType, typename... ArgTypes>
int reflect_Obj::func(
    std::string const &methodName,
    ReturnType &result,
    ArgTypes... args)
{
  if (NULL == _p_singleton_className)
  {
    return -1;
  }

  using Func = ReturnType (*)(reflect_Obj *const, ArgTypes...);
  reflect_Method m;
  if (0<=_reflect_MethodRegistry<ReturnType, ArgTypes...>::get(*_p_singleton_className, methodName, m)) // 有
  {
    auto f = (Func)m.getFunctor();
    result = f(this, std::forward<ArgTypes>(args)...);
    return 0;
  }
  else
    return -2;
}

template <typename... ArgTypes>
int reflect_Obj::pro(
    std::string const &methodName,
    ArgTypes... args)
{
  if (NULL == _p_singleton_className)
  {
    return -1;
  }

  using Func = void (*)(reflect_Obj *const, ArgTypes...);
  reflect_Method m;
  if (0<=_reflect_MethodRegistry<void, ArgTypes...>::get(*_p_singleton_className, methodName, m)) // 有
  {
    auto f = (Func)m.getFunctor();
    f(this, std::forward<ArgTypes>(args)...);
    return 0;
  }
  else
    return -2;
}

std::unordered_map<
    std::string /*域名*/,
    reflect_Field> const *
reflect_Obj::getFields() const
{
  if (NULL == _p_singleton_className)
  {
    return NULL;
  }

  return _reflect_normalRegistry::getFields(*_p_singleton_className);
}

std::unordered_map<
    std::string /*方法名*/,
    reflect_Method> const *
reflect_Obj::getMethods() const
{
  if (NULL == _p_singleton_className)
  {
    return NULL;
  }

  return _reflect_normalRegistry::getMethods(*_p_singleton_className);
}

int reflect_Obj::toJSON(reflect_JSON &json) const
{
  if (NULL == _p_singleton_className)
  {
    return -1;
  }

  int (*f)(void const *, reflect_JSON &);
  int ret;
  auto fields = getFields();
  if (NULL == fields)
  {
    return 0;
  }

  for (auto &kv : *fields)
  {
    auto &field = kv.second;
    f = field.toJSON;
    if (0<=f(((void const *)this) + field.offset, json[field.name]))
    {
    }
    else// fail
      return -2; 
  }
  return 1;
}

int reflect_Obj::fromJSON(reflect_JSON const &json)
{
  if (NULL == _p_singleton_className)
  {
    return -1;
  }

  int (*f)(reflect_JSON const &, void *);
  int ret;
  auto fields = getFields();
  if (NULL == fields)
  {
    return 0;
  }

  for (auto &kv : *fields)
  {
    auto &field = kv.second;
    if (json.contains(field.name))
    {
    }
    else //json没有这个键
      return -3;

    f = field.fromJSON;
    if (0<=f(json[field.name], ((void *)this) + field.offset))
    {
    }
    else
      return -2;
  }
  return 1;
}

#define REFLECT_ACCESS(pointer_or_sharedPtr, fieldName, type) \
  (pointer_or_sharedPtr ? pointer_or_sharedPtr->access<type>(fieldName) : ((type *)NULL))

template <typename... ConstructorArgTypes>
inline reflect_Obj *reflect_new(
    std::string const &className,
    ConstructorArgTypes... args)
{
  auto f = _reflect_ClassRegistry<ConstructorArgTypes...>::get_new(className);
  if (NULL == f)
  {
    return NULL;
  }

  reflect_Obj *obj = f(std::forward<ConstructorArgTypes>(args)...);

  // 是通过反射得到的，要带上类名
  obj->_p_singleton_className = _reflect_normalRegistry::get(className);
  if (NULL == obj->_p_singleton_className)
  {
    delete obj;
    return NULL;
  }

  return obj;
}

template <typename... ConstructorArgTypes>
inline reflect_Ptr reflect_share(
    std::string const &className,
    ConstructorArgTypes... args)
{
  auto f = _reflect_ClassRegistry<ConstructorArgTypes...>::get_make_shared(className);
  if (NULL == f)
  {
    return nullptr;
  }

  reflect_Ptr obj = f(std::forward<ConstructorArgTypes>(args)...);

  // 是通过反射得到的，要带上类名
  obj->_p_singleton_className = _reflect_normalRegistry::get(className);
  if (NULL == obj->_p_singleton_className)
  {
    obj = nullptr;
  }

  return obj;
}

#endif // _reflect_Obj_inl
