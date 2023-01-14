#if !defined(_reflect_JSON_decl_hpp)
#define _reflect_JSON_decl_hpp

#define JSON_USE_IMPLICIT_CONVERSIONS 0 //禁止隐式类型转换，从JSON值对象读取内容到变量，需要通过get<>()或get_to()
#include "nlohmann/json.hpp"

using reflect_JSON = nlohmann::json;

#endif // _reflect_JSON_decl_hpp
