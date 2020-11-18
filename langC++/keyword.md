[TOC]
# c
if else do while switch case default for continue break goto
void bool signed unsigned int short long char float double
enum const extern register volatile inline
typedef struct union return sizeof asm
true false

# try throw catch
# explict
禁止单参数构造函数"被用于自动型别转换
# class

# static

# template

# this

# using namespace

# typeid

# typename

# auto
# virtual
# operator
1. 一元运算符 + - / * %
2. 二元运算符 ==  !=  <  >  <=  >= 
3. 逻辑运算符 ||  &&  !
4. 单目运算符 + - *(指针) &(取址)
5. 迭代运算符 ++  --
6. 位运算符   |  &  ~  ^  <<  >>
7. 赋值运算符 =  +=  -=  *=  /=  %=  |=  ^=  <<=  >>=
8. 内存运算符 new  delete  new[]  delete[]
9. 其他运算符 ()函数调用 ->成员访问  []下标访问
10. 不可重载的运算符:
    1. `.`  成员运算符
    2. `.*`  `->*` 成员指针访问
    3. `::` 域运算符
    4. `sizeof` 长度运算符
    5. `? :` 三元条件运算符
    6. `#`   预处理标记

# friend
# template
# private public protected
# new delete
# const_cast dynamic_cast static_cast reinterrupt_cast
1. const_cast: 常量{对象/指针/引用} 转化为 非常量{对象/指针/引用}
```c++
void use_const_cast() {
    // 指针
    const int *a;
    int *p = const_cast<int *>a;
    // 引用
    const int &b;
    int &q = const_cast<int &>b;
    // 对象
    const C c;
    C r = const_cast<C> c;
}
```
2. dynamic_cast: 带安全检查的 层次类指针的上行转换 和 下行转换; 类之间的交叉转换
    1. 上行转换(子类指针 转换 基类指针): 转换时; 检查子类指针指向的对象; 不论实际指向的对象; 转换成功
    2. 下行转换(基类指针 转换 子类指针): 转换时; 检查基类指针指向的对象; 如果基类对象; 转换失败; 返回 nullptr
    + 实际上 dynamic_cast 使用 RTT 技术; 检查运行时对象的实际上 class_id
3. static_cast: 无安全检查的 层次类指针的上行转换 和 下行转换; 类之间的交叉转换
    1. 上行转换 and 下行转换 均不检查
```c++
class Parent {
class Parent {
    public:
        virtual void f() { printf("Parent object\n"); return; }
};
class ChildX : public Parent {
    public:
        void f() { printf("ChildX object\n"); return; }
        int member;
};

int main( int argc, char **argv ) {
    Parent parent;
    ChildX childX;
    Parent *parent_ptr = NULL;
    ChildX *childX_ptr = NULL;

    // 动态上级转换a) 转换成功; 无因患
    childX_ptr = &childX;
    parent_ptr = dynamic_cast<Parent *>(childX_ptr);

    // 静态上级转换a) 转换成功; 无因患
    childX_ptr = &childX;
    parent_ptr = static_cast<Parent *>(childX_ptr);

    // 动态下级转换 a) 上级指针实际上指向下级对象; 转换成功
    parent_ptr = &childX;
    childX_ptr = dynamic_cast<ChildX *>(parent_ptr);
    printf("[%d] [%s] dynamic_cast<ChildX *>(parent_ptr)\n",
            __LINE__, childX_ptr ? "success" : "failed");

    // 动态下级转换 b) 上级指针实际上指向上级对象; 转换失败
    parent_ptr = &parent;
    childX_ptr = dynamic_cast<ChildX *>(parent_ptr);
    printf("[%d] [%s] dynamic_cast<ChildX *>(parent_ptr)\n",
            __LINE__, childX_ptr ? "success" : "failed");

    // 静态下级转换 b) 上级指针实际上指向上级对象; 转换成功; 但有危险操作
    parent_ptr = &parent;
    childX_ptr = static_cast<ChildX *>(parent_ptr);
    printf("[%d] [%s] static_cast<ChildX *>(parent_ptr); will be dangerous\n",
            __LINE__, childX_ptr ? "success" : "failed");
    printf("[%d] unsafe read member %d\n", childX_ptr->member); // 如果是写操作; 问题很大

    return 0;
}
```
4. reinterprut_cast 实际上 几乎同于 c语言的 指针强转
```c++
int n = 0x6162636400; // 0x61是字符'a'的ASCII码
int  *pint = &num;
char *pstr = reinterpret_cast<char *>(&num);
char *pstr_c = (char *)&n;
```