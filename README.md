此前的破产版本明显是类型不安全的，需要借助模板元编程改进一下。
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

现在他们的局面是：以上大部分信息延迟获知。因此，反射需要额外准备一些与字符串相关的映射：
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
        名称、偏移地址、尺寸等等*/>>* fields;
  /*安全访问fields的其它静态函数*/...
};
```
fields设为指针能减小空fields的内存占用，即，有人要来注册了再new；另外，不必delete。<br>
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
        好像只需记录地址*/>>* methods;
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
    Ptr<Obj> (*)(Ts...)/*任意参数构造函数*/>* classes;
};
```
相应地，单例`RegisterAction_*`也套上`template <typename ... Ts>`。

# 实现
具体实现可能和[设计](#设计)部分预想的有出入。
