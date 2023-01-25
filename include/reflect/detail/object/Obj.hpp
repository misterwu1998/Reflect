#if !defined(_reflect_Obj_hpp)
#define _reflect_Obj_hpp

#include <string>
#include <unordered_map>

class reflect_Field;
class reflect_Method;
#include "reflect/detail/json/JSON_decl.hpp"

class reflect_Obj
{
public:

  /// @brief 只要是借助反射机制动态获取的对象，这一字符串用于记录实际类名
  std::string __typeName;
  std::string getTypeName() const{return __typeName;}
  
  reflect_Obj(){}
  virtual ~reflect_Obj(){}

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @return 类型为FieldType、名为fieldName的成员变量的指针; NULL 反射类__typeName没有注册类型为FieldType、名称为fieldName的域
  template <typename FieldType> FieldType* access(std::string const& fieldName);

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @return 类型为FieldType、名为fieldName的成员变量的指针; NULL 反射类__typeName没有注册类型为FieldType、名称为fieldName的域
  template <typename FieldType> FieldType const* access(std::string const& fieldName) const;

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @param value 
  /// @return 1 OK; 0 反射类__typeName没有注册类型为FieldType、名称为fieldName的域
  template <typename FieldType>  int get(std::string const& fieldName, FieldType& value) const;

  /// @brief 
  /// @tparam FieldType 
  /// @param fieldName 
  /// @param value 
  /// @return 1 OK; 0 反射类__typeName没有注册类型为FieldType、名称为fieldName的域
  template <typename FieldType>  int set(std::string const& fieldName, FieldType const& value);

  /// @brief 调用有返回值的函数
  /// @tparam ReturnType 
  /// @tparam ...ArgTypes 
  /// @param methodName 
  /// @param result 
  /// @param ...args 
  /// @return 1 OK; 0 反射类__typeName没有注册返回值类型为ReturnType、参数列表为ArgTypes...的方法
  template <typename ReturnType, typename ... ArgTypes>
  int func(
    std::string const& methodName,
    ReturnType& result,
    ArgTypes... args
  );

  /// @brief 调用无返回值的过程
  /// @tparam ...ArgTypes 
  /// @param methodName 
  /// @param ...args 
  /// @return 1 OK; 0 反射类__typeName没有注册返回值为void、参数列表为ArgTypes...的方法
  template <typename ... ArgTypes>
  int pro(
    std::string const& methodName,
    ArgTypes... args
  );

  std::unordered_map<
    std::string/*域名*/,
    reflect_Field> const* getFields() const;
  
  std::unordered_map<
    std::string/*方法名*/,
    reflect_Method> const* getMethods() const;
    
  /// @brief 
  /// @param json 
  /// @return 1 OK; 0 fail
  int toJSON(reflect_JSON& json) const;

  /// @brief 
  /// @param json 
  /// @return 1 OK; 0 fail
  int fromJSON(reflect_JSON const& json);

};

using reflect_Ptr = std::shared_ptr<reflect_Obj>;

#endif // _reflect_Obj_hpp
