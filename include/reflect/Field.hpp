#if !defined(_reflect_Field_hpp)
#define _reflect_Field_hpp

#include <string>

class reflect_Field
{ 
public:
  
  unsigned int offset;
  unsigned int size;
  std::string name;
 
  reflect_Field(unsigned int offset, unsigned int size, std::string const& name) : offset(offset), size(size), name(name) {}
  reflect_Field(){}

  inline unsigned int getOffset(){return offset;}
  inline unsigned int getSize(){return size;}
  std::string getName(){return name;}
  
};

#endif // _reflect_Field_hpp
