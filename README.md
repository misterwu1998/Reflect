# 破产版 C++ 反射库
## build
CMake四步；链接库即产物`./build/libReflect.so`。
```cmake
mkdir build
cd build
cmake ..
make
```
这个的`CMakeLists.txt`很简单，随便改。<br>
头文件主要在[reflect/](./include/reflect/)；[util/](./include/util/)也带上（其实只有个单例宏）。
## 使用
可参照[test目录](./test)下的几个文件。<br>
1. 让被反射的类`Foo`继承自`reflect_Obj`; 需要包含[Obj.hpp](./include/reflect/Obj.hpp)。
2. 在`Foo`的头文件中定义成员变量，声明成员函数；在`Foo`的源文件中定义成员函数；注意，必须保留`Foo`的无参构造函数。
3. 在`Foo`的源文件中使用宏，为`Foo`注册无参构造函数、成员变量、成员函数；需要包含[Registeraction.hpp](./include/reflect/Registeraction.hpp)
4. 根据字符串获取某类的对象实例<br>
包含[Registry.hpp](./include/reflect/Registry.hpp)和[Obj.hpp](./include/reflect/Obj.hpp)；<br>
`reflect_Registry::get()`返回全局单例的注册中心`r`；<br>
如果类`Foo`的无参构造函数已经被注册，那么`r.getConstructor("Foo")`返回无参构造函数`c`；<br>
`c()`以`reflect_Obj`指针的形式返回堆上的`Foo`对象`obj`。<br>
5. 根据字符串读写某类的成员变量<br>
`obj->get<int>("id")`返回`Foo`对象`obj`的成员变量`id`的值，并且认定其值类型是`int`；<br>
`obj->set("id", 114.514)`将`id`的值设为`double`值114.514，类型是推导出来的；<br>
6. 根据字符串调用某类的成员函数<br>
`obj->callFunc<返回值类型>()`调用有返回值的成员函数，`obj->callPro()`调用无返回值的成员函数；<br>
形参比较复杂（比如含有`const&`型）时，建议显式指定形参类型，例如`id = obj->callFunc<int, std::string const&>("getId", std::string("abc"));`指明形参是`std::string const&`型，返回值是`int`型；<br>
`callPro()`无返回值，所以返回值类型的那个模板参数本来就是空缺的。
