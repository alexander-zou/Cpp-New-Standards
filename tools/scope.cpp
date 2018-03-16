
#include "include/scope.h"
#include "include/memory_helper.h"

using namespace std;

// init static members:

int
Scope::_s_count = 0;

bool
Scope::_s_newline = true;

vector<ScopeSetting>
Scope::_s_stack;

// static methods:

void
Scope::_print_indent()
{
    if ( _s_newline) {
        _s_newline = false;
        for ( auto const &setting : _s_stack)
            if ( setting.indent)
                cout << "| ";
    }
}

void
Scope::_print_newline()
{
    _print_indent();
    cout << endl;
    _s_newline = true;
}

// methods:

Scope::Scope( char const *name, bool indent)
    : _start_time( high_resolution_clock::now()),
      _id( count ++),
      _name( name),
      id( _id),
      name( _name)
{
    if ( indent) {
        _print_newline();
        (*this) << "_#" << id << ' ' << name << endl;
    }
    ScopeSetting setting;
    setting.indent = indent;
}

Scope::~Scope()
{
    // TODO: print mem report:
    bool indent = _s_stack.end().indent;
    _s_stack.pop_back();
    if ( indent) {
        _print_newline();
        (*this) << "\\#" << id << ' ' << name << endl; 
    }
}

Scope &
Scope::SetTimer( bool show)
{
    _show_time = show;
    return *this;
}

Scope &
Scope::operator<<( Manipulator m)
{
    if ( m == (Manipulator)endl)
        _print_newline();
    else
        cout << m;
    return *this;
}


