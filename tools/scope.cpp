
#include <cstdlib>  // malloc(), free()
#include "../include/scope.h"

using namespace std;
using namespace std::chrono;

// tool functions:

static void *offset( void *ptr, ssize_t off)
{
    return reinterpret_cast<char *>( ptr) + off;
}

static void const *offset( void const *ptr, ssize_t off)
{
    return reinterpret_cast<char const *>( ptr) + off;
}

// init static members:

int
Scope::_s_count = 0;

bool
Scope::_s_newline = true;

int
Scope::_s_total_indent = 0;

Scope *
Scope::_s_current_scope = nullptr;

Scope * const &
Scope::s_current_scope = _s_current_scope;

Scope::endl_t
Scope::endl;

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
    cout << std::endl;
    _s_newline = true;
}

void *
Scope::_SAllocMem( size_t size)
{
    void *result = malloc( size + sizeof( size_t));
    if ( result) {
        *reinterpret_cast<size_t *>( result) = size;
        result = offset( result, sizeof( size_t));
        if ( _s_current_scope) {
            _s_current_scope->_mem_allocated += size;
            if ( _s_current_scope->_show_alloc)
                ( *_s_current_scope) << "[[MEM]] allocate "
                    << result << " +" << size << endl;
        }
    } else {
        new_handler handler = get_new_handler();
        if ( handler != nullptr)
            handler();
    }
    return result;
}

void *
Scope::_SAllocMemNoexcept( size_t size) noexcept
{
    void *result = malloc( size + sizeof( size_t));
    if ( result) {
        *reinterpret_cast<size_t *>( result) = size;
        result = offset( result, sizeof( size_t));
        if ( _s_current_scope) {
            _s_current_scope->_mem_allocated += size;
            if ( _s_current_scope->_show_alloc)
                ( *_s_current_scope) << "[[MEM]] ALLOCATE: "
                    << result << " +" << size << endl;
        }
    }
    return result;
}

void
Scope::_SReleaseMem( void *ptr) noexcept
{
    if ( ptr) {
        ptr = offset( ptr, -(int)sizeof( size_t));
        size_t size = *reinterpret_cast<size_t *>( ptr);
        if ( _s_current_scope) {
            _s_current_scope->_mem_freed += size;
            if ( _s_current_scope->_show_alloc)
                ( *_s_current_scope) << "[[MEM]] RELEASE : "
                    << ptr << " -" << size << endl;
        }
        free( ptr);
    }
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
    if ( _show_report) {
        (*this) << "[[MEM]] TOTAL: +" << _mem_allocated
                << " -" << _mem_freed << endl;
    }
    if ( _indent) {
        _SPrintNewline();
        _s_total_indent --;
        (*this) << "\\#" << id << ' ' << name << endl;
    }
    _s_current_scope = _enclosure_scope;
    if ( _s_current_scope) {
        _s_current_scope->_mem_allocated += _mem_allocated;
        _s_current_scope->_mem_freed += _mem_freed;
    }
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
Scope::operator<<( endl_t &endl) const
{
    _SPrintNewline();
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

// override new and delete

void *operator new( size_t size)
{
    return Scope::_SAllocMem( size);
}

void *operator new( size_t size, nothrow_t const &) noexcept
{
    return Scope::_SAllocMemNoexcept( size);
}

void operator delete( void *ptr) noexcept
{
    Scope::_SReleaseMem( ptr);
}

void *operator new[]( size_t size)
{
    return operator new( size);
}

void *operator new[]( size_t size, nothrow_t const &nothrow) noexcept
{
    return operator new( size, nothrow);
}

void operator delete( void *ptr, nothrow_t const &) noexcept
{
    operator delete( ptr);
}

void operator delete[]( void *ptr) noexcept
{
    operator delete( ptr);
}

void operator delete[]( void *ptr, nothrow_t const &) noexcept
{
    operator delete( ptr);
}


