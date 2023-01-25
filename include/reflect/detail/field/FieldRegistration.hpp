#if !defined(_reflect_FieldRegistration_hpp)
#define _reflect_FieldRegistration_hpp

class reflect_Field;

template <typename FieldType>
class _reflect_FieldRegistration
{
public:
  _reflect_FieldRegistration(
    std::string const& typeName,
    std::string const& fieldName,
    reflect_Field const& field
  );
};

#endif // _reflect_FieldRegistration_hpp
