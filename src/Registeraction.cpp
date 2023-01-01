#include "reflect/Registeraction.hpp"
#include "reflect/Registry.hpp"

reflect_Registeraction::reflect_Registeraction(std::string const& className, std::shared_ptr<reflect_Obj> (*constructor)(void)){
  auto& r = reflect_Registry::get();
  r.registerConstructor(className,constructor);
}

reflect_Registeraction::reflect_Registeraction(std::string const& className, std::string const& fieldName, reflect_Field const& field){
  auto& r = reflect_Registry::get();
  r.registerField(className,fieldName,field);
}

reflect_Registeraction::reflect_Registeraction(std::string const& className, std::string const& methodName, reflect_Method const& method){
  auto& r = reflect_Registry::get();
  r.registerMethod(className,methodName,method);
}
