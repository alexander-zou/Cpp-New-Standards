#ifndef _DUMMY_H
#define _DUMMY_H

#include <initializer_list>

class Scope;

class Dummy {
    friend class Scope;
    
    public:
        enum InfoType : unsigned {
            SHOW_NONE = 0,
            SHOW_ALL = (unsigned)-1,
            SHOW_ID                 = 1 << 0,
            DEFAULT_CTOR            = 1 << 1,
            COPY_CTOR               = 1 << 2,
            MOVE_CTOR               = 1 << 3,
            NOEXCEPT_MOVE_CTOR      = 1 << 4,
            INITER_LIST_CTOR        = 1 << 5,
            CONVERSION_CTOR         = 1 << 6,
            DTOR                    = 1 << 7,
            ASSIGN_OP               = 1 << 8,
            MOVE_ASSIGN_OP          = 1 << 9,
            NOEXCEPT_MOVE_ASSIGN_OP = 1 << 10,
            INITER_ASSIGN_OP        = 1 << 11,
            CONVERSION_ASSIGN_OP    = 1 << 12,
        };

    private:
        static int _s_count;

        int _m_id = _s_count ++;
        InfoType _m_setting = SHOW_NONE;

        bool _NeedPrint( InfoType type);
        void _PrintName();
        void _PrintName( Scope const &scope);

    public:
        int const &m_id = _m_id;
        char m_char_member = 0;
        int m_int_member = 0;
        float m_float_member = 0;
        double m_double_member = 0;

        // ctors:
        Dummy();
        Dummy( Dummy const &other);
        //Dummy( Dummy &&other);
        Dummy( Dummy &&other) noexcept;
        Dummy( int value);
        Dummy( std::initializer_list<double> list);

        // dtor:
        virtual ~Dummy();

        // assignment operators:
        virtual Dummy &operator=( Dummy const &other);
        //virtual Dummy &operator=( Dummy &&other);
        virtual Dummy &operator=( Dummy &&other) noexcept;
        virtual Dummy &operator=( int value);
        virtual Dummy &operator=( std::initializer_list<double> const &list);

};

#endif

