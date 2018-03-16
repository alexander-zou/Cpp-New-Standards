#ifndef _SCOPE_H
#define _SCOPE_H

#include <ostream>
#include <functional> // std::function
#include <vector>

#include "include/dummy.h"

class Scope {
    friend class Dummy;
    private:
        static int _s_count;
        static bool _s_newline;

        struct ScopeSetting {
            bool indent = true;
            bool printMem = false;
            Dummy::InfoType dummyInfo = Dummy::NONE;
        };

        static vector<ScopeSetting> _s_stack;

        static void _print_indent();
        static void _print_newline();

        static bool _need_print_mem();
        static Dummy::InfoType _dummy_info_setting();

        typedef std::ostream &(*Manipulator)( std::ostream &);

        std::high_resolution_clock::time_point _start_time;
        int _id;
        char const *_name;
        std::vector<std::function> _exit_callback;
        bool _show_time = false;

    public:

        int const &id;
        char const * const &name;

        Scope( char const *name = "", bool indent = true);
        ~Scope();

        Scope &SetTimer( bool show = true);
        Scope &ShowMemoryAllocation( bool show = true);
        Scope &ShowMemoryReport( bool show = true);
        Scope &AddExitCallback( std::function cb);
        Scope &ClearExitCallbacks();
        Scope &SetDummyInfo();
        float TimePastInSec() const;

        static Scope &CurrentScope();

        template<typename T>
        Scope &operator<<( T val)
        {
            _print_indent();
            std::cout << val;
            return *this;
        }

        Scope &operator<<( Manipulator m);


};

#endif

