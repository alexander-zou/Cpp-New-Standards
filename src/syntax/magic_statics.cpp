
/*
 * magic statics (也称 function-local static initialization) 是指函数局部定义的
 * 静态对象返回给调用方的写法。
 */

#include <iostream>

#include "../../include/dummy.h"
#include "../../include/scope.h"

using std::endl;

/*
 * C++11起，标准规定 magic statics 是线程安全的，因而能够安全地用于编写线程安全
 * 的单例：
 */
class Singleton final {
    public:
        static Singleton &GetInstance()
        {
            static Singleton instance;
            return instance;
        }

        void bar() const
        {
            Scope scope( false);
            scope << "Singleton::bar() called" << endl;
        }

    private:
        Singleton() = default;
        ~Singleton() = default;

        Singleton( Singleton const &) = delete;
        Singleton &operator=( Singleton const &) = delete;

}; // class Singleton

/*
 * magic statics 对象会在第一次执行到声明语句时构造(lazy initialization)，会在
 * 进程退出时析构:
 */
Dummy<> &foo()
{
    Scope scope( "foo()");
    static Dummy<> dummy;
    return dummy;
}

int main()
{
    Scope scope( "main()");
    scope.SetDummyInfo( Scope::SHOW_ALL);
    auto &ret1 = foo();
    scope << "return addr: " << &ret1 << endl;
    auto &ret2 = foo();
    scope << "return addr: " << &ret2 << endl;
    Singleton::GetInstance().bar();
    return 0;
}

