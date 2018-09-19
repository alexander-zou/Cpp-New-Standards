
#include <iostream>
#include <cstdint>      // uint64_t

using namespace std;

/*
 * C++11起，允许用户自定义字面常量运算符(Literal operators)，这种运算符
 * 能够定义用户自定义字面常量后缀(ud-suffix)，让带有这种后缀的普通字面常
 * 量通过字面常量运算符转化为特定的结果，以实现用户定义字面常量
 * (user-defined literals)。
 *
 * 例如下面的代码定义了将角度转化为弧度的用户定义字面常量：
 */

long double operator"" _deg( long double d)
{
    constexpr long double PI = 3.1415926536;
    return d * PI / 180;
}

void show_degree_literal()
{
    cout << "180 degree is " << 180._deg << endl;
    // 注，这里'180._deg'不可写作'180_deg'，后者会调用：
    // operator"" _deg( unsigned long long)
}

/*
 * 用户自定义字面常量运算符的语法要求:
 * 1.用户自定义字面常量后缀必须以下划线开始，不以下划线开始的字面常量后
 *   缀是标准保留的。
 * 2.这种运算符不得带有参数默认值。
 * 3.这种运算符可以是inline或constexpr的。
 * 4.这种运算符可以被显式调用，也可以取地址。
 * 5.这种运算符的参数列表必须为下列之一：
 *      a. ( char const *)
 *      b. ( unsigned long long)
 *      c. ( long double)
 *      d. ( char)
 *      e. ( wchar_t)
 *      f. ( char16_t)
 *      g. ( char32_t)
 *      h. ( char const *, size_t)
 *      i. ( wchar_t const *, size_t)
 *      j. ( char16_t const *, size_t)
 *      k. ( char32_t const *, size_t)
 *   其中：
 *   a形式仅针对同名的整数和浮点数字面常量不存在时的备选运算符
 *   从h到k形式的第二个参数为字符串长度
 * 6.这种运算符不能C链接
 * 7.若用户自定义字面常量后缀以p、P、e、E结尾，则这种用户自定义字面常量
 *   后跟的加、减号应用空格隔开，以避免与科学计数法混淆
 * 8.整数或浮点数后跟字面常量后缀时，后续的点运算符应用空格隔开，以免点
 *   被当作后缀的一部分解析。
 */

// 应用示例：

constexpr unsigned long long operator""_K( unsigned long long v) { return v * 1024;}
constexpr long double operator""_K( long double v) { return v * 1024;}
constexpr unsigned long long operator""_M( unsigned long long v) { return operator""_K( v) * 1024;}
constexpr long double operator""_M( long double v) { return operator""_K( v) * 1024;}
constexpr unsigned long long operator""_G( unsigned long long v) { return operator""_M( v) * 1024;}
constexpr long double operator""_G( long double v) { return operator""_M( v) * 1024;}
constexpr unsigned long long operator""_T( unsigned long long v) { return operator""_G( v) * 1024;}
constexpr long double operator""_T( long double v) { return operator""_G( v) * 1024;}

void show_simple_example()
{
    cout << "1.2G is " << 1.2_G << endl;
    if ( 1_T == 1024_G)
        cout << "1T equals to 1024G" << endl;
    cout << "2.25M + 100K is " << (int)2.25_M + 100_K << endl;
}

template<typename T>
class Length {
    private:
        T meter;
    public:
        template<typename T2>
        Length( Length<T2> const &other) : meter( other.ByMeter()) {}
        Length( T meter) : meter( meter) {}
        template<typename T2>
        Length operator-() const { return Length( -meter);}
        T ByMeter() const { return meter;}
        T ByKiloMeter() const { return meter / 1000;}
        T ByYard() const { return meter * 1.0936133;}
};

/*
 * 对模版类的每个实例，必须分别提供字面常量运算符，否则编译器不可能自动
 * 判断出你所写的字面常量对应的是哪个模版类实例：
 */

Length<unsigned long long> operator""_meter( unsigned long long v)
{
    return Length<unsigned long long>( v);
}

Length<long double> operator""_meter( long double v)
{
    return Length<long double>( v);
}

Length<unsigned long long> operator""_kilometer( unsigned long long v)
{
    return Length<unsigned long long>( v * 1000);
}

Length<long double> operator""_kilometer( long double v)
{
    return Length<long double>( v * 1000);
}

Length<unsigned long long> operator""_yard( unsigned long long v)
{
    return Length<unsigned long long>( v * 0.9144);
}

Length<long double> operator""_yard( long double v)
{
    return Length<long double>( v * 0.9144);
}

void show_class_literal()
{
    // 注意下面两行字面常量后缀和点运算中间必须用空格或括号隔开:
    cout << "100yard is " << (100_yard).ByMeter() << "meter" << endl;
    cout << "-0.01km is " << -0.01_kilometer .ByMeter() << "meter" << endl;
    Length<float> len( 1_kilometer);
    cout << "1km is " << len.ByYard() << "yard" << endl;
}

/*
 * 根据上面的示例，我们不能用通常意义上的模版来写字面常量运算符。
 * 但为了能够在编译期安全地处理字面常量的进制转换，标准允许编写如下形式的
 * 字面常量运算符模版:
 * template<char...> TYPE operator "" _suffix();
 * 其中模版实参为字面常量的每个字符的值，TYPE为确定的返回值，_suffix为自定
 * 义的后缀，函数参数列表必须为空。
 * 下面的代码展示了如何利用这种字面常量运算符模版来实现安全(能处理溢出的)
 * 编译期四进制转换:
 */

template<uint64_t V>
constexpr uint64_t make_quaternary() { return V;}

template<uint64_t V, char C, char... CHS>
constexpr uint64_t make_quaternary()
{
    static_assert( C >= '0' && C <= '3', "invalid quaternary literal");
    return make_quaternary< 4 * V + C - '0', CHS...>();
}

template<char...CHS>
constexpr uint64_t operator""_quaternary()
{
    static_assert( sizeof...(CHS) <= 32, "quaternary literal overflow");
    return make_quaternary< 0, CHS...>();
}

void show_literal_optr_template()
{
    cout << "quaternary( 0) is " << 0_quaternary << endl;
    cout << "quaternary( 10) is " << 10_quaternary << endl;
    cout << "quaternary( 3333) is " << 3333_quaternary << endl;
}


// 最后需要注意的是，字面常量运算符是在预编译字符串连接之后生效的：

void operator""_foobar( char const *str, size_t)
{
    cout << "operator\"\"_foobar( char const *str): str is '"
        << str << '\'' << endl;
}

int main()
{
    show_degree_literal();
    show_simple_example();
    show_class_literal();
    show_literal_optr_template();
    "hello" ", " "world" "!"_foobar;
    return 0;
}

