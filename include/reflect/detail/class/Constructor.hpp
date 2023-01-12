#if !defined(_reflect_Constructor_hpp)
#define _reflect_Constructor_hpp

#include <memory>

class reflect_Obj;

template <typename Class, typename ...ArgTypes>
inline reflect_Obj* _reflect_new(ArgTypes... args)
{
  return new Class(std::forward<ArgTypes>(args)...);
}

template <typename Class, typename ...ArgTypes>
inline std::shared_ptr<reflect_Obj> _reflect_make_shared(ArgTypes... args)
{
  return std::static_pointer_cast<reflect_Obj,Class>(
    std::make_shared<Class>(
      std::forward<ArgTypes>(args)...
    )
  );
}

#endif // _reflect_Constructor_hpp

/// @note 为什么加inline: C++函数模板在.hpp中定义和实现，当有多个文件包含该头文件时编译出现重复定义的问题，该如何解决？ - 知乎用户的回答 - 知乎
/// https://www.zhihu.com/question/463903559/answer/1942405670
