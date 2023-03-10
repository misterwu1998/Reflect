#if !defined(_reflect_FieldRegistry_hpp)
#define _reflect_FieldRegistry_hpp

#include <unordered_map>
#include <memory>
#include <string>

class reflect_Field;

template <typename FieldType>
class _reflect_FieldRegistry
{
  static std::unordered_map<
      std::string /*类名*/,
      std::unordered_map<
          std::string /*域名*/,
          reflect_Field>> &
  getMap()
  {
    static std::unordered_map<
        std::string /*类名*/,
        std::unordered_map<
            std::string /*域名*/,
            reflect_Field>>
        fields;
    return fields;
  }

public:
  static void set(
      std::string const &className,
      std::string const &fieldName,
      reflect_Field const &field);

  /// @brief
  /// @param className
  /// @param fieldName
  /// @param field [out]
  /// @return -1 -- class unregistered;
  //          -2 -- field unregistered
  static int get(
      std::string const &className,
      std::string const &fieldName,
      reflect_Field &field);
};

#endif // _reflect_FieldRegistry_hpp
