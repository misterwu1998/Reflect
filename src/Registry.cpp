#include "reflect/Registry.hpp"
#include "reflect/Field.hpp"

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

int reflect_Registry::registerField(std::string const& className, std::string const& fieldName, reflect_Field const& field){
  if(fields.count(className)<1){
    fields.insert({
      className,
      std::unordered_map<std::string, reflect_Field>()
    });
  }
  if(fields[className].count(fieldName)<1){
    fields[className].insert({fieldName,field});
    return 1;
  }else{
    fields[className][fieldName] = field;
    return 0;
  }
}

int reflect_Registry::getField(std::string const& className, std::string const& fieldName, reflect_Field& field){
  if(fields.count(className)<1 || fields[className].count(fieldName)<1) return 0;
  field = fields[className][fieldName];
  return 1;
}
