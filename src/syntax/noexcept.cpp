/*
 * noexcept 关键字代替了C++98中的动态异常限定(dynamic exception specification)，用以
 * 对函数(包括lambda表达式，下同)是否抛出异常进行限制和保证。
 *
 * C++11起，函数分为无异常(non-throwing)和潜在异常(potentially throwing)两类。
 */

#include <iostream>

using namespace std;

/*
 * noexcept 关键字既用于函数声明时的限定符(noexcept specifier)，也可作为编译期运算符
 * 返回bool值以判断表达式是否为无异常的。
 * 用作限定符时，不带参数等同于参数为true。参数为true时表示函数是无异常的，否则表示函
 * 数是潜在异常的：
 */

void may_throw_func();
void no_throw_func() noexcept;

auto may_throw_lambda = []() noexcept( noexcept( may_throw_func())) {};
                                        // 表示异常限定与may_throw_func()相同
auto no_throw_lambda = []() noexcept( noexcept( no_throw_func())) {};
                                        // 表示异常限定与no_throw_func()相同

void show_noexcept()
{
#define PRINT( func) cout << #func << ( noexcept( func()) ? " is " : " isn't ") \
                            << "noexcept." << std::endl
    PRINT( may_throw_func);
    PRINT( no_throw_func);
    PRINT( may_throw_lambda);
    PRINT( no_throw_lambda);
#undef PRINT
}

/*
 * 不显式指定的情况下，大部分函数默认是潜在异常的，以下特例除外：
 * 1.析构函数，除非父类或成员存在潜在异常的析构函数
 * 2.默认的(通过=default自动生成的)默认/复制/移动构造函数，除非父类或成员存在的潜在异
 *   常的对应的默认/复制/移动构造函数
 * 3.默认的(通过=default自动生成的)复制/移动赋值运算，除非自动生成的函数会调用到潜在
 *   异常的赋值运算
 * 4.delete函数
 */

struct Base {
    Base() = default;               // 默认的默认构造函数，默认为noexcept
    Base( Base const &) = default;  // 默认的复制构造函数，默认为noexcept
    Base( Base &&) {}               // 自定义移动构造函数，默认不是noexcept
    ~Base() {}                      // 析构函数，默认为noexcept
    Base &operator=( Base const &) { return *this;}
                                    // 自定义复制赋值运算，默认不是noexcept
    Base &operator=( Base &&) = default;
                                    // 默认的移动赋值运算，默认为noexcept
};

struct Member {
    Member() = default;             // 默认的默认构造函数，默认为noexcept
    Member( Member const &) {}      // 自定义复制构造函数，默认不是noexcept
    Member( Member &&) = default;   // 默认的移动构造函数，默认为noexcept
    ~Member() noexcept( false) {}   // 显式指定的非noexcept析构函数
    Member &operator=( Member const &) = default;
                                    // 默认的复制赋值运算，默认为noexcept
    Member &operator=( Member &&) { return *this;}
                                    // 自定义移动复制运算，默认不是noexcept
};

struct Derived : Base {
    Member member;
    Derived() = default;            // 默认为noexcept
    Derived( Derived const &) = default;
                                    // 受Base影响，默认不是noexcept
    ~Derived() = default;           // 受Member影响，默认不是noexcept
    Derived &operator=( Derived const &) = default;
                                    // 受Base影响，默认不是noexcept
    Derived &operator=( Derived &&) = default;
                                    // 受Member影响，默认不是noexcept
};

template<typename T>
void show_method_noexcept( char const *name)
{
    T *ptr = nullptr;
    cout << endl;
    cout << "For class '" << name << "' :" << endl;
    cout << "noexcept( default_ctor() ) = " << noexcept( new(nothrow) T()) << endl;
    cout << "noexcept( copy_ctor() ) = " << noexcept( new(nothrow) T( *ptr)) << endl;
    cout << "noexcept( move_ctor() ) = " << noexcept( new(nothrow) T( move( *ptr))) << endl;
    cout << "noexcept( dtor() ) = " << noexcept( delete ptr) << endl;
    cout << "noexcept( copy_assign() ) = " << noexcept( *ptr = *ptr) << endl;
    cout << "noexcept( move_assign() ) = " << noexcept( *ptr = move( *ptr)) << endl;
}

/*
 * C++17前，无异常的函数、lambda表达式和函数指针，可以赋值给潜在异常的函数指针。
 * C++17起，改为可隐式类型转换为潜在异常的函数指针：
 */

void (*func)() noexcept( false) = no_throw_func;    // 这样是可以的，反之不能

/*
 * 需要注意：
 * 1.noexcept限定符不是函数签名的一部分，因而不允许同一个函数存在noexcept限定不同
 *   的两种声明。
 * 2.从C++17开始，noexcept成为函数类型的组成部分。而在C++17之前，noexcept不可以出
 *   现在typedef或类型别名(type alias)中。
 * 3.noexcept限定的函数中若抛出了异常，或者其中调用其他代码抛出了异常，不会导致编
 *   译期错误。而只会在运行期出发对std::terminate()的调用，其默认实现是直接调用
 *   std::abort()，也可以通过std::set_terminate()函数自定义处理方法。
 */

void no_throw_func() noexcept
{
    throw 123; // 不会编译错误
}

int main()
{
    show_noexcept();
    show_method_noexcept<Base>( "Base");
    show_method_noexcept<Member>( "Member");
    show_method_noexcept<Derived>( "Derived");
    return 0;
}

