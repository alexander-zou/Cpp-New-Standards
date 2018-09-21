#ifndef _SCOPE_H
#define _SCOPE_H

#include <iostream>
#include <functional> // std::function
#include <vector>
#include <chrono>

class Scope {

    public:
        enum InfoType : unsigned {
            SHOW_NONE = 0,
            SHOW_ALL = (unsigned)-1,
            ID                 = 1 << 0,
            DEF_CTOR           = 1 << 1,
            COPY_CTOR          = 1 << 2,
            MOVE_CTOR          = 1 << 3,
            INITER_LIST_CTOR   = 1 << 4,
            CONVERSION_CTOR    = 1 << 5,
            CTORS = DEF_CTOR | COPY_CTOR | MOVE_CTOR | INITER_LIST_CTOR | CONVERSION_CTOR,
            DTOR               = 1 << 6,
            COPY_ASSIGN        = 1 << 7,
            MOVE_ASSIGN        = 1 << 8,
            INITER_LIST_ASSIGN = 1 << 9,
            CONVERSION_ASSIGN  = 1 << 10,
            ASSIGNS = COPY_ASSIGN | MOVE_ASSIGN | INITER_LIST_ASSIGN | CONVERSION_ASSIGN,
        };

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
        InfoType _dummy_setting = SHOW_NONE;
        Scope const *_enclosure_scope = _s_current_scope;
        std::vector<std::function<void()> > _exit_callbacks;

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
        Scope &SetDummyInfo( unsigned setting);
        float TimePastInSec() const;

        static Scope const * const &s_current_scope;
        static bool SNeedPrintAlloc();
        static bool SNeedIndent();
        static InfoType SDummyInfoSetting();

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

