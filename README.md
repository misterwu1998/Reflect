# 引入
库的使用者平时正常的调用：
```cpp
using namespace std;
template <typename T> using Ptr<T> = shared_ptr<T>;
Ptr<ClassName> obj = new ClassName;
FT& field = obj->fieldName;//访问成员
T r = obj->funcName(Ts... as);//调用函数
```
抛开C++语法，这些语句表明正常情况下他们在编译前的代码编写期间就已经知道：
1. ClassName 类名
2. fieldName 域名 + FT 域的类型
3. funcName 函数名称 + {Ts...} 参数列表
4. T 函数返回值类型 <i>最迟也能在编译期知道；void 是一种特例</i>

现在他们的局面是，有很多信息延迟获知。因此，反射需要额外准备一些与字符串相关的映射：
1. 有效的 "ClassName" -> 对象原型 obj
2. 无效的 "ClassName" -> nullptr
3. 有效的 {"ClassName","fieldName",FT} -> 读或写成员
4. 无效的 {"ClassName","fieldName",FT} -> 什么也不做
5. 有效的 {"ClassName","funcName", T, Ts...} -> 执行方法
6. 无效的 {"ClassName","funcName", T, Ts...} -> 什么也不做

其中，带引号的元素是字符串，不带引号的元素是用户代码，并且用户将
1. 在编译前的代码编写期间指定，或者在编译期间由编译器推导出不带引号的元素，交给反射库
2. 在运行期获取带引号的元素，交给反射库

# 设计
从使用的角度考虑，以上映射应当
1. 代码侵入尽量轻微
2. 在运行期进入`main()`之前构建完成
3. 全局单例，全程不变

最后的实现可能和预想的有出入，以代码和[使用部分](#使用)为准。
## 映射1、映射2
不难想到：
```cpp
unordered_map<
  string /*类名*/,
  Ptr<Obj> (*)(void)/*无参构造函数*/> constructors;
```
然后借助`RegisterAction_*`静态对象的构造函数，在`main()`之前把键值对注册上去；利用宏定义缩短用户代码；这种办法其它映射也效仿。<br>
需要注意的是，如果要允许用户在被反射类`Foo`的头文件`Foo.hpp`中就完成注册，而无需担心`Foo.hpp`被多个源文件include造成`RegisterAction_*`对象重复定义，~~那么`RegisterAction_*`应当定义为单例，重复调用其`getSingleton(string const& className, Ptr<Obj> (*constructor)())`也不会重复注册。~~（没关系，重复就重复，给`RegisterAction_*`对象加`static`完事）
## 映射3、映射4
一开始想到的是
```cpp
unordered_map<
  string /*类名*/,
  unordered_map<
    string /*域名*/,
    【?】/*FT -> 是否读写成员*/>> fields;
```
前两层映射利用字符串，不难，问题是 FT -> 是否读写成员 的映射好像实现不了吧？思考了一下发现这样设计不对，因为 FT 最迟在库使用者的编译期被交给库，早于"className"、"fieldName"，因此映射3、映射4改写成这样会更贴近实现：
-  有效的 {FT,"ClassName","fieldName"} -> 读或写成员
-  无效的 {FT,"ClassName","fieldName"} -> 什么也不做

这跟原来的映射3、映射4完全等效，有效还是无效，是三个元素共同决定的，只不过它们被传达到反射库的顺序又先后。<br>
这样设计就比较可行了：
```cpp
template <typename FieldType>
class FieldRegistry{
public:
  static unordered_map<
    string/*类名*/,
    unordered_map<
      string/*域名*/,
      Field/*域无关于FT的属性封装类，
        名称、偏移地址、尺寸等等*/>> fields;
  /*安全访问fields的其它静态函数*/...
};
```
~~fields设为指针能减小空fields的内存占用，即，有人要来注册了再new；另外，不必delete。<br>~~
库的使用者在编译前或编译期把 FT 传过来，于是`FieldRegistry<FT>`被模板实例化，与其它可能也被实例化的`FieldRegistry<int>`、`FieldRegistry<Foo>`啥的就完全隔离了。<br>
`Obj::get()`和`Obj::set()`负责接收用户传达的 FT，问`FieldRegistry<FT>`要`Field`，要到了就说明注册了，否则就啥也不做。

## 映射5、映射6
如法炮制，只不过模板参数不定：
```cpp
template <typename RetType, typename ... ArgTypes>
class MethodRegistry{
public:
  static unordered_map<
    string/*类名*/,
    unordered_map<
      string/*函数名*/,
      Method/*
        函数无关于RetType,ArgTypes的属性封装类，
        好像只需记录地址*/>> methods;
  /*安全访问methods的其它静态函数*/...
};
```
`Obj::callPro()`和`Obj::callFunc()`负责接收用户的 T, Ts...，问`MethodRegistry<T, Ts...>`要`Method`，要到了就说明注册了，执行函数，返回0；否则就啥也不做，然后返回-1反馈用户。

## 回到映射1、映射2
有了映射5、映射6的经验，有参构造方法也能搞定了，映射1、映射2扩展为：
- 有效的 {Ts..., "ClassName"} -> 已初始化的对象 obj
- 无效的 {Ts..., "ClassName"} -> nullptr
```cpp
template <typename ... Ts>
class ClassRegistry{
public:
  static unordered_map<
    string/*类名*/,
    Ptr<Obj> (*)(Ts...)/*任意参数构造函数*/> classes;
};
```
相应地，单例`RegisterAction_*`也套上`template <typename ... Ts>`。

## T, T&, T&&, T const&, T const
引用型的成员变量不支持注册，所以`get()`,`set()`使用简单的模板参数，让编译器简单将`FieldType`视为值类型即可。但涉及可变参的构造函数、成员函数就比较麻烦。如果要让用户在反射调用成员函数（包括构造函数）时完全不需要费事去指定模板参数，那么反射库需要对参数列表中任意位置的函数参数实现以下映射：

![如果允许反射调用函数时完全不需要指定模板参数……](invoke.png)

这个我目前真不会……隐约感觉能通过什么办法递归展开Action类模板，实际为某类型的参数把相应的几个形式全注册上去？我想即使能实现，也会额外有很大开销，就暂时不寻找、学习这个问题的方案了。

# 使用
参考[test/](test/)下的例子。
## 0. Include路径
[reflect目录所在的目录](./include/)
## 1. 头文件
包含[single_include.hpp](include/reflect/single_include.hpp)即可。
## 2. 继承`reflect_Obj`
## 3. 注册
借助以下宏定义完成。可以在头文件中注册，但建议在源文件中注册。
### REFLECT_REGISTER_CONSTRUCTOR(反射类,昵称, 构造函数形参类型列表)
```cpp
REFLECT_REGISTER_CONSTRUCTOR(Foo, prototype)
REFLECT_REGISTER_CONSTRUCTOR(Foo, rvInt,    int&&)
REFLECT_REGISTER_CONSTRUCTOR(Foo, crStr,    std::string const&)
REFLECT_REGISTER_CONSTRUCTOR(Foo, both,     int, const char*)
```
`昵称`仅用于区分可能被注册的多个构造函数，不能重复；随便起名即可。
### REFLECT_REGISTER_FIELD(反射类,域,值类型)
```cpp
REFLECT_REGISTER_FIELD(Foo,id,int)
REFLECT_REGISTER_FIELD(Foo,name, std::string)
```
<b>仅支持值类型的成员变量的注册。</b>不要注册类的引用型成员。

### REFLECT_REGISTER_METHOD(反射类,方法,形参类型列表)
```cpp
REFLECT_REGISTER_METHOD(Foo,getId,int)
REFLECT_REGISTER_METHOD(Foo,shout,void,double)
REFLECT_REGISTER_METHOD(Foo,nothing,void)
REFLECT_REGISTER_METHOD(Foo,func, int* (*)(std::unique_ptr<float>), std::string, Foo&&)
```

## 4. 反射
Tips. 如果不确定需不需要列出模板参数，就干脆都列出，或者看看IDE给出的模板参数推导结果是否符合实际。
### reflect_Ptr
即`std::shared_ptr<reflect_Obj>`。因此，不同的反射类之间需要避免循环引用。
### reflect_new(), reflect_share()
分别返回`reflect_Obj*`, `reflect_Ptr`。
- 如果要使用反射类的无参构造方法，或者这个构造方法的形参全部都是值类型，模板参数就不必指定。
```cpp
obj = reflect_new/*<int, char const*>*/("Foo", 123,"John");//指针也是值
```
- 如果构造方法的形参含有引用类型，模板参数就必须全部列出。
```cpp
obj = reflect_new<std::string const&>("Foo","John");
```
`reflect_new()`遇到[设计部分](#设计)所提到的“无效的”情况，将返回`nullptr`。
### reflect_Obj::get()
很多情况下，如果无需指定模板参数。
```cpp
int id;
ret = obj->get("id",id);
std::string name;
ret = obj->get("name",name);
```
成功，返回1；遇到任何“无效的”情况，返回0。
### reflect_Obj::set()
```cpp
ret = obj->set("id",123);
```
成功，返回1；遇到任何“无效的”情况，返回0。

`set()`需要列出模板参数的情况就多一些，比如
1. 假设`Foo::id`是`unsigned long`型，那么编译器默认推断123是`int`型就不吻合了
2. 一些平时会隐式转换的类型，比如`std::string`，传字面量进去会推断为`char const*`
### reflect_Obj::pro()
调用没有返回值的成员方法。类似地，形参全部都是值类型时，模板参数就不必指定，其余时候都必须指定。
```cpp
ret = obj->pro/*<double>*/("shout",114.514);
```
成功，返回1；遇到任何“无效的”情况，返回0。
### reflect_Obj::func()
调用有返回值的成员方法。类似地，形参全部都是值类型时，模板参数就不必指定，其余时候都必须指定。
```cpp
int* (*ptr)(std::unique_ptr<float>);
ret = obj->func("getId",id);
ret = obj->func
  < decltype(ptr),  std::string,  Foo&&        >("unknown",   
    ptr,            "reflect!",   Foo{42,"答案"});
```
成功，返回1；遇到任何“无效的”情况，返回0。
### reflect_Obj::getFields(), reflect_Obj::getMethods()
获取被注册的域和方法。这里不考虑序列化、JSON化、XML化之类的问题，仅提供无关于类型的reflect_Field、reflect_Method的遍历途径。
