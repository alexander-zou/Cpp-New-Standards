#include <iostream>

#include "../../include/scope.h"
#include "../../include/dummy.h"

using namespace std;

/*
 * 从C++11开始，允许类和结构体成员的声明里提供默认初始化器(default member initializer)。
 * 默认初始化器必须以等号或初始化列表的方式提供，并且仅在被调用的构造函数没有提供这个成员
 * 的初始化器时生效。
 */

int n = 0;

struct Foo {
    Dummy<> dummy1 = n++;       // 不可以写成：Dummy<> dummy( n ++);
    Dummy<> dummy2{ double( n++), double( n++), double( n++), double( n++)};
    Dummy<> dummy3 = dummy1;    // 可以隐式地使用this

    Foo() {}    // 会调用默认初始化器

    Foo( int x) // 不会调用默认初始化器
    : dummy1( x)
    , dummy2()
    {
    }
};

int main()
{
    Scope scope( false);
    scope.SetDummyInfo( Scope::CTORS | Scope::ID);
    scope << "now n = " << n << endl;
    Foo foo1;
    scope << "now n = " << n << endl;
    Foo foo2( 123);
    scope << "now n = " << n << endl;
    return 0;
}


// 需要注意的限制：

struct Bar {
    // 不能以圆括号方式编写默认初始化器:
    // int value( 123);

    // 默认初始化器不能用于推断数组尺寸:
    // int array[] = { 1, 2, 3};

    // 默认初始化器不能调用这个类的默认的默认构造函数:
    // Bar() = default;
    // Bar *bar = new Bar;

    // 设置位域的成员不能使用默认初始化器:
    // unsigned char flag : 4 = 0xf;

    // 静态成员不能使用默认初始化器:
    // static int s_value = 456;

    // C++14起，引用成员的默认初始化器不能为临时量:
    // int const &x = 42;

};


