#include <iostream>
#include "../include/dummy.h"

using namespace std;

// init static members:

int
DummyBase::_s_count = 0;

// member functions:

DummyBase::DummyBase() noexcept
: _m_setting( Scope::SDummyInfoSetting())
{
}

bool
DummyBase::_NeedPrint( Scope::InfoType type) const noexcept
{
    return _m_setting & type;
}

void
DummyBase::_PrintName( Scope const *scope) const noexcept
{
    if ( ! scope)
        scope = Scope::s_current_scope;
    if ( scope) {
        *scope << endl << "Dummy";
        if ( _NeedPrint( Scope::ID))
            *scope << '#' << m_id;
        *scope << ": ";
    } else {
        cout << endl << "Dummy";
        if ( _NeedPrint( Scope::ID))
            cout << '#' << m_id;
        cout << ": ";
    }
}

void
DummyBase::_PrintMsg( string const &msg, Scope const *scope) const noexcept
{
    if ( ! scope)
        scope = Scope::s_current_scope;
    _PrintName( scope);
    if ( scope)
        *scope << msg << endl;
    else
        cout << msg << endl;
}

void
DummyBase::_PrintInfo( string const &msg, Scope::InfoType type, Scope const *scope) const noexcept
{
    if ( _NeedPrint( type))
        _PrintMsg( msg, scope);
}



