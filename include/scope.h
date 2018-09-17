#ifndef _SCOPE_H
#define _SCOPE_H

#include <iostream>
#include <functional> // std::function
#include <vector>
#include <chrono>

#include "../include/dummy.h"

class Scope {
    private:
        static int _s_count;
        static bool _s_newline;
        static int _s_total_indent;
        static Scope const *_s_current_scope;

        static void _SPrintIndent();
        static void _SPrintNewline();

        typedef std::ostream &(*Manipulator)( std::ostream &);

        std::chrono::time_point<std::chrono::high_resolution_clock> _start_time =
                                                std::chrono::high_resolution_clock::now();
        int _id = _s_count ++;
        char const *_name;
        bool _indent = false;
        bool _show_time = false;
        bool _show_alloc = false;
        bool _show_report = false;
        Dummy::InfoType _dummy_setting = Dummy::SHOW_NONE;
        Scope const *_enclosure_scope = _s_current_scope;
        std::vector<std::function<void()> > _exit_callbacks;
//        bool _has_enclosure_scope = _s_current_scope;

        void _InheritSettings();

    public:

        int const &id = _id;
        char const * const &name = _name;

        Scope( char const *name = "");
        Scope( bool indent, char const *name = "");
        ~Scope();

        Scope &SetTimer( bool show = true);
        Scope &ShowMemoryAllocation( bool show = true);
        Scope &ShowMemoryReport( bool show = true);
        Scope &AddExitCallback( std::function<void()> cb);
        Scope &ClearExitCallbacks();
        Scope &SetDummyInfo( Dummy::InfoType setting);
        float TimePastInSec() const;

        static Scope const * const &s_current_scope;
        static bool SNeedPrintAlloc();
        static bool SNeedIndent();
        static Dummy::InfoType SDummyInfoSetting();

        template<typename T>
        Scope const &operator<<( T val) const
        {
            _SPrintIndent();
            std::cout << val;
            return *this;
        }

        Scope const &operator<<( Manipulator m) const;


};

#endif

