#if !defined(_reflect_FieldRegistry_hpp)
#define _reflect_FieldRegistry_hpp

#include <unordered_map>
#include <memory>
#include <string>
#include "reflect/detail/field/Field.hpp"

class reflect_Obj;

template <typename FieldType>
class _reflect_FieldRegistry
{
  static std::unordered_map<
    std::string/*类名*/,
    std::unordered_map<
      std::string/*域名*/,
      _reflect_Field>>* fields;

public:

  static void set(
    std::string const& className,
    std::string const& fieldName,
    _reflect_Field const& field
  ){
    if(NULL==fields)
      fields = new std::unordered_map<
        std::string/*类名*/,
        std::unordered_map<
          std::string/*域名*/,
          _reflect_Field>>;
    (*fields)[className][fieldName] = field;
  }

  /// @brief 
  /// @param className 
  /// @param fieldName 
  /// @param field [out]
  /// @return 1 有; 0 没有
  static int get(
    std::string const& className,
    std::string const& fieldName,
    _reflect_Field& field)
  {
    if(NULL==fields)
      return 0;
    auto itc = fields->find(className);
    if(fields->end()==itc)
      return 0;
    auto& c = itc->second;
    auto itf = c.find(fieldName);
    if(c.end()==itf)
      return 0;
    field = itf->second;
    return 1;
  }

};

template <typename FieldType>
std::unordered_map<
  std::string/*类名*/,
  std::unordered_map<
    std::string/*域名*/,
    _reflect_Field>>* _reflect_FieldRegistry<FieldType>::fields = NULL;

#endif // _reflect_FieldRegistry_hpp
