#if !defined(_reflect_dispatch_hpp)
#define _reflect_dispatch_hpp

#include "util/MemberExistenceChecker.hpp"
#include "util/EnableIf.hpp"

class reflect_Obj;
#include "reflect/detail/json/JSON_decl.hpp"

CheckMemberFunctionExistence(toJSON)

/// @brief 
/// @tparam T 
/// @tparam  
/// @param obj 
/// @param jsonValue JSON键值对中的值
/// @return 1 OK; 0 fail
template <typename T, 
          EnableIf_ptr<HasMemberFunction_toJSON<T>::result &&
                       std::is_base_of<reflect_Obj,T>::value> p = nullptr >
int toJSON_basic_or_reflectObj(T& obj, reflect_JSON& jsonValue);

/// @brief 
/// @tparam T 
/// @tparam  
/// @param obj 
/// @param jsonValue JSON键值对中的值
/// @return 1 OK; 0 fail
template <typename T,
          EnableIf_ptr<! HasMemberFunction_toJSON<T>::result> p = nullptr>
int toJSON_basic_or_reflectObj(T& obj, reflect_JSON& jsonValue);

#endif // _reflect_dispatch_hpp
