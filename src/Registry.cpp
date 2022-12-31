#include "reflect/Registry.hpp"

reflect_Registry& reflect_Registry::get(){
  static reflect_Registry r;
  return r;
}

int reflect_Registry::registerConstructor(std::string const& className, reflect_Obj* (*constructor)(void)){
  auto it = defaultConstructors.find(className);
  if(defaultConstructors.end()==it){//还没有
    defaultConstructors.insert({className, constructor});
    return 1;
  }else{//已有
    it->second = constructor;
    return 0;
  }
}

reflect_Obj* (*reflect_Registry::getConstructor(std::string const& className))(void){
  auto it = defaultConstructors.find(className);
  if(defaultConstructors.end()==it) return NULL;
  else return it->second;
}
