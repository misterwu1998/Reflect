#include "reflect/Registeraction.hpp"
#include "reflect/Registry.hpp"

reflect_Registeraction::reflect_Registeraction(std::string const& className, reflect_Obj* (*constructor)(void)){
  auto& r = reflect_Registry::get();
  r.registerConstructor(className,constructor);
}
