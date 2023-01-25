#if !defined(_reflect_TypeRegistry_hpp)
#define _reflect_TypeRegistry_hpp

#define _REFLECT_DEBUG 0

#include <memory>
#include <unordered_map>
#include <string>
#if _REFLECT_DEBUG
  #include <iostream>    
#endif

class reflect_Obj;

/// @note 仅用于在main()之前注册参数列表为Ts...的类构造函数，没有考虑资源竞争。
/// 用户非要在main()内继续使用，属于未定义行为
/// @tparam ...ConstructorArgTypes 
template <typename ... ConstructorArgTypes>
class _reflect_TypeRegistry
{
  static std::unordered_map<
    std::string/*类名*/,
    std::shared_ptr<reflect_Obj> 
      (*)(ConstructorArgTypes...)/*任意参数构造函数*/>& getMap_make_shared()
  {
    /// @note 是否需要、是否可以上锁呢？目前我觉得没必要，但也不知道在main()之前使用锁会不会出问题
    /// 我觉得没必要的理由是
    /// 1. 没有见到任何记载表明全局对象的初始化会有资源竞争
    /// 2. 注册完成后，private的typees应当是只读的，
    ///     用户不应操作任何reflect_TypeRegistry<ConstructorArgTypes...>类的任何方法，
    ///     如果用户在main()内还想使用，就得自己保护资源
    static std::unordered_map<
      std::string/*类名*/,
      std::shared_ptr<reflect_Obj> 
        (*)(ConstructorArgTypes...)/*任意参数构造函数*/> typees;
    return typees;
  }

  static std::unordered_map<
    std::string/*类名*/,
    reflect_Obj* (*)(ConstructorArgTypes...)/*任意参数构造函数*/>& getMap_new()
  {
    static std::unordered_map<
      std::string/*类名*/,
      reflect_Obj* (*)(ConstructorArgTypes...)/*任意参数构造函数*/> typees;
    return typees;
  }

public:

  static void set(
    std::string const& typeName,
    reflect_Obj* (*_func)(ConstructorArgTypes...)
  ){
    getMap_new()[typeName] = _func;
  }

  /// @brief 注册类的某个构造函数；已经注册过，则覆盖
  /// @param typeName 
  /// @param _func 借助宏展开的回调函数
  static void set(
    std::string const& typeName,
    std::shared_ptr<reflect_Obj>
      (*_func)(ConstructorArgTypes...)  )
  {
    getMap_make_shared()[typeName] = _func;
#if _REFLECT_DEBUG
    std::cout << "_reflect_TypeRegistry::set(): register " << typeName << std::endl;
#endif
  }

  static reflect_Obj* (*get_new(std::string const& typeName))(ConstructorArgTypes...){
    auto& typees = getMap_new();
    auto i = typees.find(typeName);
    if(typees.end()==i)
      return NULL;
    return i->second;
  }

  /// @param typeName 
  /// @return 参数列表为Ts...的构造函数的指针；或者NULL表示未注册
  static std::shared_ptr<reflect_Obj> (*get_make_shared(std::string const& typeName))(ConstructorArgTypes...)
  {
    auto& typees = getMap_make_shared();
    auto i = typees.find(typeName);
    if(typees.end()==i)
      return NULL;
    return i->second;
  }
  
};

#undef _REFLECT_DEBUG

#endif // _reflect_TypeRegistry_hpp
