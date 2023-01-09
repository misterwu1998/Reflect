#if !defined(_reflect_Field_hpp)
#define _reflect_Field_hpp

#include <string>

/// @brief 域的类型无关的属性，即，无论什么类的什么类型的域，都会有这里定义的属性。
class _reflect_Field
{ 
public:
  
  unsigned int offset;
  unsigned int size;
  std::string name;
 
  _reflect_Field(unsigned int offset, unsigned int size, std::string const& name) : offset(offset), size(size), name(name) {}
  _reflect_Field(){}

  inline unsigned int getOffset() const{return offset;}
  inline unsigned int getSize() const{return size;}
  inline std::string getName() const{return name;}
  
};

#endif // _reflect_Field_hpp
