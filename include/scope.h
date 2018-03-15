#ifndef _SCOPE_H
#define _SCOPE_H

#include <functional> // std::function
#include <vector>

class Scope {
    private:
        static int count;

        int _id;
        char const *_name;
        bool _print_entry_and_exit;
        std::vector<std::function> _exit_callback;

    public:

        int const &id;
        char const * const &name;

        Scope( char const *name = "unnamed", bool print_entry_and_exit = false);
        ~Scope();

        void ShowTimeUsed( bool show = true);
        void ShowMemoryAllocation( bool show = true);
        void ShowMemoryReport( bool show = true);
        void AddExitCallback( std::function cb);
        void ClearExitCallbacks();
};

#endif

