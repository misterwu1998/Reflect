#if !defined(_reflect_Field_hpp)
#define _reflect_Field_hpp

#include <string>

#include "reflect/detail/json/JSON_decl.hpp"

/// @brief 域的类型无关的属性，即，无论什么类的什么类型的域，都会有这里定义的属性。
class reflect_Field
{
public:
  unsigned int offset;
  unsigned int size;
  std::string name;
  std::string typeName;
  int (*toJSON)(void const *, reflect_JSON &);   /// @return negative -- fail; non-negative -- OK
  int (*fromJSON)(reflect_JSON const &, void *); /// @return negative -- fail; non-negative -- OK

  reflect_Field(unsigned int offset,
                unsigned int size,
                std::string const &name,
                std::string const &typeName,
                int (*toJSON)(void const *, reflect_JSON &),
                int (*fromJSON)(reflect_JSON const &, void *))
      : offset(offset), size(size), name(name), typeName(typeName), toJSON(toJSON), fromJSON(fromJSON) {}
  reflect_Field() {}

  inline unsigned int getOffset() const { return offset; }
  inline unsigned int getSize() const { return size; }
  inline std::string const &getName() const { return name; }
  inline std::string const &getTypeName() const { return typeName; }
};

#endif // _reflect_Field_hpp
