#include <iostream>

using std::cout;
using std::endl;

int main()
{
    int arr[] = { 1, 2, 3, 4, 5};
    for ( int x : arr)
        cout << x << ' ';
    cout << endl;
    return 0;
}

