#if !defined(_reflect_TypeName_hpp)
#define _reflect_TypeName_hpp

#include <string>
#include <cstdint>

// 没有注册的类不会有
template <typename Type> struct reflect_TypeName
{
  /// @param other 如果不想重置类名，就不用赋值
  /// @return 
  static std::string const& registerOrGet(std::string const& other = ""){
    static std::string n;
    if(! other.empty())
      n = other;
    return n;
  }
};

#define BASIC_TYPE_NAME_(type) \
template <> struct reflect_TypeName<type>\
{\
  static std::string const& registerOrGet(std::string const& other = "")\
  {\
    static std::string n(#type);\
    if(! other.empty())\
      n = other;\
    return n;\
  }\
};
BASIC_TYPE_NAME_(char)
BASIC_TYPE_NAME_(unsigned char)
BASIC_TYPE_NAME_(int16_t)
BASIC_TYPE_NAME_(uint16_t)
BASIC_TYPE_NAME_(int32_t)
BASIC_TYPE_NAME_(uint32_t)
BASIC_TYPE_NAME_(int64_t)
BASIC_TYPE_NAME_(uint64_t)
BASIC_TYPE_NAME_(float)
BASIC_TYPE_NAME_(double)
BASIC_TYPE_NAME_(long double)
#undef BASIC_TYPE_NAME_

#endif // _reflect_TypeName_hpp
