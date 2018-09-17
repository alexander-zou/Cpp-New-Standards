#include "../include/dummy.h"
#include "../include/scope.h"

using namespace std;

// init static members:

int
Dummy::_s_count = 0;

// member functions:

bool
Dummy::_NeedPrint( InfoType type)
{
    return _m_setting & type;
}

void
Dummy::_PrintName( Scope const &scope)
{
    scope << endl << "Dummy";
    if ( _NeedPrint( SHOW_ID))
        scope << '#' << m_id;
    scope << ": ";
}

void
Dummy::_PrintName()
{
    cout << "Dummy";
    if ( _NeedPrint( SHOW_ID))
        cout << '#' << m_id;
    cout << ": ";
}

#define PRINT_INFO( type, msg) \
    if ( Scope::s_current_scope && _NeedPrint( type)) { \
        Scope const &scope = *(Scope::s_current_scope); \
        _PrintName( scope); \
        scope << msg << endl; \
    } else if ( _NeedPrint( type)) { \
        _PrintName(); \
        cout << msg << endl; \
    }

// ctors:

#define CTOR_BODY( type, msg, init) \
    : _m_setting( Scope::SDummyInfoSetting()) \
{ \
    PRINT_INFO( type, msg) \
    init \
}

Dummy::Dummy()
CTOR_BODY( DEFAULT_CTOR, "calling default constructor.",{})

Dummy::Dummy( Dummy const &other)
CTOR_BODY( COPY_CTOR, "calling copy constructor.", {
            m_char_member = other.m_char_member;
            m_int_member = other.m_int_member;
            m_float_member = other.m_float_member;
            m_double_member = other.m_double_member;
        })

/*
Dummy::Dummy( Dummy &&other)
CTOR_BODY( MOVE_CTOR, "calling move constructor.", {
            m_char_member = other.m_char_member;
            m_int_member = other.m_int_member;
            m_float_member = other.m_float_member;
            m_double_member = other.m_double_member;
        })*/

Dummy::Dummy( Dummy &&other) noexcept
CTOR_BODY( NOEXCEPT_MOVE_CTOR, "calling noexcept move constructor.", {
            m_char_member = other.m_char_member;
            m_int_member = other.m_int_member;
            m_float_member = other.m_float_member;
            m_double_member = other.m_double_member;
        })

Dummy::Dummy( int value)
CTOR_BODY( CONVERSION_CTOR, "calling conversion constructor.", {
            m_char_member = value;
            m_int_member = value;
            m_float_member = value;
            m_double_member = value;
        })

Dummy::Dummy( initializer_list<double> list)
CTOR_BODY( INITER_LIST_CTOR, "calling initializer constructor.", {
            auto it = list.begin();
            if ( it != list.end()) {
                m_char_member = *it;
                ++ it;
            }
            if ( it != list.end()) {
                m_int_member = *it;
                ++ it;
            }
            if ( it != list.end()) {
                m_float_member = *it;
                ++ it;
            }
            if ( it != list.end()) {
                m_double_member = *it;
                ++ it;
            }
        })

#undef CTOR_BODY

// dtor:

Dummy::~Dummy()
{
    PRINT_INFO( DTOR, "calling destructor.");
}

// assignment operators:

Dummy &
Dummy::operator=( Dummy const &other)
{
    PRINT_INFO( ASSIGN_OP, "calling normal assignment operator.");
    m_char_member = other.m_char_member;
    m_int_member = other.m_int_member;
    m_float_member = other.m_float_member;
    m_double_member = other.m_double_member;
    return *this;
}

/*
Dummy &
Dummy::operator=( Dummy &&other)
{
    PRINT_INFO( MOVE_ASSIGN_OP, "calling move assignment operator.");
    m_char_member = other.m_char_member;
    m_int_member = other.m_int_member;
    m_float_member = other.m_float_member;
    m_double_member = other.m_double_member;
    return *this;
}
*/

Dummy &
Dummy::operator=( Dummy &&other) noexcept
{
    PRINT_INFO( NOEXCEPT_MOVE_ASSIGN_OP, "calling noexcept move assignment operator.");
    m_char_member = other.m_char_member;
    m_int_member = other.m_int_member;
    m_float_member = other.m_float_member;
    m_double_member = other.m_double_member;
    return *this;
}

Dummy &
Dummy::operator=( initializer_list<double> const &list)
{
    PRINT_INFO( INITER_ASSIGN_OP, "calling initializer assignment operator.");
    auto it = list.begin();
    if ( it != list.end()) {
        m_char_member = *it;
        ++ it;
    }
    if ( it != list.end()) {
        m_int_member = *it;
        ++ it;
    }
    if ( it != list.end()) {
        m_float_member = *it;
        ++ it;
    }
    if ( it != list.end()) {
        m_double_member = *it;
        ++ it;
    }
    return *this;
}

Dummy &
Dummy::operator=( int value)
{
    PRINT_INFO( CONVERSION_ASSIGN_OP, "calling conversion assignment operator.")
    m_char_member = value;
    m_int_member = value;
    m_float_member = value;
    m_double_member = value;
    return *this;
}


