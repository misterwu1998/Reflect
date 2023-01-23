#if !defined(_reflect_dispatch_hpp)
#define _reflect_dispatch_hpp

#include "util/MemberExistenceChecker.hpp"
#include "util/EnableIf.hpp"

class reflect_Obj;
#include "reflect/detail/json/JSON_decl.hpp"

CheckMemberFunctionExistence(toJSON)
CheckMemberFunctionExistence(fromJSON)

/// @brief 
/// @tparam T 
/// @tparam  
/// @param obj 
/// @param jsonValue JSON键值对中的值
/// @return 1 OK; 0 fail
template <typename T, 
          EnableIf_ptr<HasMemberFunction_toJSON<T>::result &&
                       std::is_base_of<reflect_Obj,T>::value> p = nullptr >
inline int toJSON_basic_or_reflectObj(T const& obj, reflect_JSON& jsonValue);

/// @brief 
/// @tparam T 
/// @tparam  
/// @param obj 
/// @param jsonValue JSON键值对中的值
/// @return 1 OK; 0 fail
template <typename T,
          EnableIf_ptr<! HasMemberFunction_toJSON<T>::result> p = nullptr>
inline int toJSON_basic_or_reflectObj(T const& obj, reflect_JSON& jsonValue);

/// @brief 
/// @tparam T 
/// @tparam p 
/// @param jsonValue 
/// @param obj 
/// @return 1 OK; 0 fail
template <typename T,
          EnableIf_ptr<HasMemberFunction_fromJSON<T>::result &&
                       std::is_base_of<reflect_Obj,T>::value> p = nullptr>
inline int fromJSON_basic_or_reflectObj(reflect_JSON const& jsonValue, T& obj);

/// @brief 
/// @tparam T 
/// @tparam p 
/// @param jsonValue 
/// @param obj 
/// @return 1 OK; 0 fail
template <typename T,
          EnableIf_ptr<! HasMemberFunction_fromJSON<T>::result> p = nullptr>
inline int fromJSON_basic_or_reflectObj(reflect_JSON const& jsonValue, T& obj);

#endif // _reflect_dispatch_hpp
