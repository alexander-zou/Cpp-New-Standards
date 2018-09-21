
#include "../include/scope.h"
//#include "../include/memory_helper.h"

using namespace std;
using namespace std::chrono;

// init static members:

int
Scope::_s_count = 0;

bool
Scope::_s_newline = true;

int
Scope::_s_total_indent = 0;

Scope const *
Scope::_s_current_scope = nullptr;

Scope const * const &
Scope::s_current_scope = _s_current_scope;

// static methods:

void
Scope::_SPrintIndent()
{
    if ( _s_newline) {
        _s_newline = false;
        for ( int i = 0; i < _s_total_indent; i ++)
            cout << "| ";
    }
}

void
Scope::_SPrintNewline()
{
    _SPrintIndent();
    cout << endl;
    _s_newline = true;
}

bool
Scope::SNeedPrintAlloc()
{
    if ( s_current_scope)
        return s_current_scope->_show_alloc;
    return false;
}

bool
Scope::SNeedIndent()
{
    if ( s_current_scope)
        return s_current_scope->_indent;
    return true;
}

Scope::InfoType
Scope::SDummyInfoSetting()
{
    if ( s_current_scope)
        return s_current_scope->_dummy_setting;
    return SHOW_NONE;
}

// methods:

void
Scope::_InheritSettings()
{
    if ( _enclosure_scope) {
        _show_time = _enclosure_scope->_show_time;
        _show_alloc = _enclosure_scope->_show_alloc;
        _dummy_setting = _enclosure_scope->_dummy_setting;
    }
}

Scope::Scope( char const *name)
    : Scope( SNeedIndent(), name)
{
    _s_current_scope = this;
}

Scope::Scope( bool indent, char const *name)
    : _name( name)
{
    _InheritSettings();
    _indent = indent;
    if ( _indent) {
        _SPrintNewline();
        (*this) << "_#" << id << ' ' << name << endl;
        _s_total_indent ++;
    }
    _s_current_scope = this;
}

Scope::~Scope()
{
    for ( auto &cb : _exit_callbacks)
        cb();
    // TODO: print mem report
    if ( _indent) {
        _SPrintNewline();
        _s_total_indent --;
        (*this) << "\\#" << id << ' ' << name << endl;
    }
    _s_current_scope = _enclosure_scope;
}

Scope &
Scope::SetTimer( bool show)
{
    _show_time = show;
    return *this;
}

Scope &
Scope::ShowMemoryAllocation( bool show)
{
    _show_alloc = show;
    return *this;
}

Scope &
Scope::ShowMemoryReport( bool show)
{
    _show_report = show;
    return *this;
}

float
Scope::TimePastInSec() const
{
    auto now = high_resolution_clock::now();
    duration<float> past = duration_cast<duration<float>>( now - _start_time);
    return past.count();
}

Scope &
Scope::SetDummyInfo( unsigned setting)
{
    _dummy_setting = (InfoType)setting;
    return *this;
}

Scope const &
Scope::operator<<( Manipulator m) const
{
    if ( m == (Manipulator)endl)
        _SPrintNewline();
    else
        cout << m;
    return *this;
}

Scope &
Scope::AddExitCallback( function<void()> cb)
{
    _exit_callbacks.push_back( cb);
    return *this;
}

Scope &
Scope::ClearExitCallbacks()
{
    _exit_callbacks.clear();
    return *this;
}


