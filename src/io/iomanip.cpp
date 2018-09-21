
#include <iostream>
#include <sstream>  // istringstream, stringstream
#include <ios>      // showbase(), noshowbase()
#include <string>
#include <locale>
#include <iomanip>
#include <ctime>    // tm

using namespace std;

void Money()
{
    // C++11引入的get_money和put_money函数，用以输入和输出本地化的货币数据:
    
    long double v1;
    string v2;
    istringstream iss( "$1,234.56 2.22 USD");
    iss.imbue( std::locale( "en_US.UTF-8"));
    iss >> get_money( v1) >> get_money( v2, true);
    cout << "v1 = " << v1 << endl;          // 读出123456
    cout << "v2 = " << v2 << endl;          // 读出222

    cout.imbue( locale( "zh_CN.utf-8"));
    cout << showbase;   // 需要加上showbase来开启输出单位
    cout << put_money( 123) << endl;           // 输出结果: ￥1.23
    cout << put_money( "345", true) << endl;   // 输出结果: CNY 3.45
    cout << noshowbase;
}

void Time()
{
    // C++11引入的get_time和put_time函数，用于输入和输出本地化的时间和日期:
    tm t;
    istringstream iss( "2018-十一月-29");
    iss.imbue( std::locale( "zh_CN.UTF-8"));
    iss >> get_time( &t, "%Y-%b-%d");

    cout.imbue( locale( "en_US.UTF-8"));
    cout << put_time( &t, "%x") << endl;

    /*
     * 这里用到的格式匹配符与POSIX函数strptime()相同，参见:
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/strptime.html
     */
}

void Quoted()
{
    // C++14引入的quoted函数
#if __cplusplus >= 201400L
    // 输出字符串时添加引号:
    string const msg( "This message contains \"quotes\".");
    stringstream ss;
    ss << quoted( msg);
    string quoted_msg = ss.str();
    cout << "quoted message:" << endl << quoted_msg << endl;
    
    // 输入字符串时去除引号:
    string unquoted_msg;
    ss >> quoted( unquoted_msg);
    cout << "unquoted message:" << endl << unquoted_msg << endl;

    // 第二和第三个参数用来设置引号字符和逃脱字符:
    istringstream iss( "'This message contains %'single quotes%'.'");
    iss >> quoted( unquoted_msg, '\'', '%');
    cout << "unquoted message:" << endl << unquoted_msg << endl;

#endif
}

int main()
{
    Money();
    Time();
    Quoted();
    return 0;
}

