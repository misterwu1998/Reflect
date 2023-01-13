#if !defined(_reflect_FieldRegistry_hpp)
#define _reflect_FieldRegistry_hpp

#define _REFLECT_DEBUG 0

#include <unordered_map>
#include <memory>
#include <string>
#if _REFLECT_DEBUG
  #include <iostream>
#endif
#include "reflect/detail/field/Field.hpp"

class reflect_Obj;

template <typename FieldType>
class _reflect_FieldRegistry
{
  static std::unordered_map<
    std::string/*类名*/,
    std::unordered_map<
      std::string/*域名*/,
      reflect_Field>>& getMap()
  {
    static std::unordered_map<
      std::string/*类名*/,
      std::unordered_map<
        std::string/*域名*/,
        reflect_Field>> fields;
    return fields;
  }

public:

  static void set(
    std::string const& className,
    std::string const& fieldName,
    reflect_Field const& field
  ){
    auto& fields = getMap();
    fields[className][fieldName] = field;
#if _REFLECT_DEBUG
    std::cout << "_reflect_FieldRegistry::set(): register " << className << "::" << fieldName << std::endl;
#endif
  }

  /// @brief 
  /// @param className 
  /// @param fieldName 
  /// @param field [out]
  /// @return 1 有; 0 没有
  static int get(
    std::string const& className,
    std::string const& fieldName,
    reflect_Field& field)
  {
    auto& fields = getMap();
    auto itc = fields.find(className);
    if(fields.end()==itc)
      return 0;
    auto& c = itc->second;
    auto itf = c.find(fieldName);
    if(c.end()==itf)
      return 0;
    field = itf->second;
    return 1;
  }

};

#undef _REFLECT_DEBUG

#endif // _reflect_FieldRegistry_hpp
