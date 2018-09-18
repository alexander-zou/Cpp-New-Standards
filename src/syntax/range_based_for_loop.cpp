#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int main()
{
    cout << "array:" << endl;
    int arr[] = { 1, 2, 3, 4, 5};
    for ( int x : arr)
        cout << x << ' ';
    cout << endl;

    cout << "STL container:" << endl;
    vector<string> vec{ "abc", "def", "1234567"};
    for ( auto const &str : vec)
        cout << str << endl;

#if __cplusplus >= 201700L
    cout << "STL key-value container:" << endl;
    map<string,int> tars_settings;
    tars_settings[ "honesty"] = 95;
    tars_settings[ "humor"] = 60;

    // 利用结构化绑定声明(structured binding declaration)简化map遍历:
    for ( auto &&[ str, val] : tars_settings)
        cout << str << ':' << val << endl;
#endif
    return 0;
}

