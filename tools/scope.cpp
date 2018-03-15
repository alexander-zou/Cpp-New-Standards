
#include "include/scope.h"
#include "include/memory_helper.h"


int
Scope::count = 0;

Scope::Scope( char const *name, bool print)
    : _id( count ++),
      _name( name),
      _print_entry_and_exit( print),
      id( _id),
      name( _name)
{
}

