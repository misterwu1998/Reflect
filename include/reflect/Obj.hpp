#if !defined(_reflect_Obj_hpp)
#define _reflect_Obj_hpp

#include <string>
#include "reflect/Registry.hpp"
#include "reflect/Field.hpp"
#include "reflect/Method.hpp"

class reflect_Obj
{
public:

  /// @brief 只要是借助反射机制动态获取的对象，这一字符串用于记录实际类名
  std::string __className;
  
  reflect_Obj(){}
  virtual ~reflect_Obj(){}

  template <typename FieldType> FieldType get(std::string const& fieldName){
    reflect_Field field;
    auto ret = reflect_Registry::get().getField(__className, fieldName, field);
    if(1==ret){//成功获取
      auto offset = field.getOffset();
      return *((FieldType *)(((void*)this) + offset));
    }
  }

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @param value 
  /// @return 1表示设置成功，0表示未注册此Field
  template <typename FieldType> int set(std::string const& fieldName, FieldType&& value){
    reflect_Field field;
    auto ret = reflect_Registry::get().getField(__className, fieldName, field);
    if(1==ret){
      auto offset = field.getOffset();
      *((FieldType *)(((void*)this) + offset)) = value;
      return 1;
    }else return 0;
  }

  template <typename RetType, typename ... ArgTypes> RetType callFunc(std::string const& methodName, ArgTypes ... args){
    reflect_Method method;
    auto ret = reflect_Registry::get().getMethod(__className, methodName, method);
    if(1==ret){
      using _This = decltype(this);
      using _Func = RetType (*)(_This const, decltype(args) ...);
      _Func f = (_Func)(method.getFunctor());
      return (RetType)(f(this, std::forward<ArgTypes>(args)...));
    }
  }

  template <typename ... ArgTypes> void callPro(std::string const& methodName, ArgTypes ... args){
    reflect_Method method;
    auto ret = reflect_Registry::get().getMethod(__className, methodName, method);
    if(1>ret) return;
    using _This = decltype(this);
    using _Func = void (*)(_This const, decltype(args) ...);
    _Func f = (_Func)(method.getFunctor());
    f(this, std::forward<ArgTypes>(args)...);
    return;
  }

};

#endif // _reflect_Obj_hpp
