#include "reflect/Registry.hpp"

reflect_Registry& reflect_Registry::get(){
  static reflect_Registry r;
  return r;
}

int reflect_Registry::registerConstructor(std::string const& className, std::shared_ptr<reflect_Obj> (*constructor)(void)){
  auto it = defaultConstructors.find(className);
  if(defaultConstructors.end()==it){//还没有
    defaultConstructors.insert({className, constructor});
    return 1;
  }else{//已有
    it->second = constructor;
    return 0;
  }
}

std::shared_ptr<reflect_Obj> (*reflect_Registry::getConstructor(std::string const& className))(void){
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

int reflect_Registry::registerMethod(std::string const& className, std::string const& methodName, reflect_Method const& method){
  if(methods.count(className)<1){
    methods.insert({
      className,
      std::unordered_map<std::string, reflect_Method>()
    });
  }
  if(methods[className].count(methodName)<1){
    methods[className].insert({methodName,method});
    return 1;
  }else{
    methods[className][methodName] = method;
    return 0;
  }
}

int reflect_Registry::getMethod(std::string const& className, std::string const& methodName, reflect_Method& method){
  if(methods.count(className)<1 || methods[className].count(methodName)<1) return 0;
  method = methods[className][methodName];
  return 1;
}
