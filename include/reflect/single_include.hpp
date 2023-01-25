#if !defined(_reflect_single_include_hpp)
#define _reflect_single_include_hpp

#include "reflect/detail/type/TypeName.hpp"
#include "reflect/detail/type/TypeRegistry.hpp"
#include "reflect/detail/type/Constructor.hpp"
#include "reflect/detail/field/Field.hpp"
#include "reflect/detail/json/JSON_decl.hpp"
#include "reflect/detail/method/Method.hpp"

#include "reflect/detail/type/TypeRegistration.inl"
#include "reflect/detail/type/normal.inl"
#include "reflect/detail/field/FieldRegistration.inl"
#include "reflect/detail/field/FieldRegistry.inl"
#include "reflect/detail/json/dispatch.inl"
#include "reflect/detail/method/MethodRegistration.inl"
#include "reflect/detail/method/MethodRegistry.inl"
#include "reflect/detail/object/Obj.inl"

#endif // _reflect_single_include_hpp
