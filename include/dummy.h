#ifndef _DUMMY_H
#define _DUMMY_H

#include <initializer_list>
#include <string>
#include <sstream>

#include "../include/scope.h"

class DummyBase {
    friend class Scope;

    protected:
        static int _s_count;

        int _m_id = _s_count ++;
        Scope::InfoType _m_setting;

        DummyBase() noexcept;
        bool _NeedPrint( Scope::InfoType type) const noexcept;
        void _PrintInfo( std::string const &, Scope::InfoType, Scope const *scope = nullptr) const noexcept;

    private:
        void _PrintName( Scope const *scope = nullptr) const noexcept;
        void _PrintMsg( std::string const &msg, Scope const *scope = nullptr) const noexcept;

    public:
        int const &m_id = _m_id;

}; // class DummyBase

template<bool IS_NOEXCEPT = true>
class Dummy : public DummyBase {
    friend class Scope;
    
    public:
        char m_char_member = 0;
        int m_int_member = 0;
        float m_float_member = 0;
        double m_double_member = 0;

        // ctors:
        Dummy() noexcept( IS_NOEXCEPT)
        {
            _PrintInfo( "calling default constructor", Scope::DEF_CTOR);
        }
        Dummy( Dummy const &other) noexcept( IS_NOEXCEPT)
        : m_char_member( other.m_char_member)
        , m_int_member( other.m_int_member)
        , m_float_member( other.m_float_member)
        , m_double_member( other.m_double_member)
        {
            std::ostringstream oss;
            oss << "calling copy constructor";
            if ( _NeedPrint( Scope::ID))
                oss << " with Dummy#" << other.m_id;
            _PrintInfo( oss.str(), Scope::COPY_CTOR);
        }
        Dummy( Dummy &&other) noexcept( IS_NOEXCEPT)
        : m_char_member( other.m_char_member)
        , m_int_member( other.m_int_member)
        , m_float_member( other.m_float_member)
        , m_double_member( other.m_double_member)
        {
            std::ostringstream oss;
            oss << "calling move constructor";
            if ( _NeedPrint( Scope::ID))
                oss << " with Dummy#" << other.m_id;
            _PrintInfo( oss.str(), Scope::MOVE_CTOR);
        }
        Dummy( int value) noexcept( IS_NOEXCEPT)
        : m_char_member( value)
        , m_int_member( value)
        , m_float_member( value)
        , m_double_member( value)
        {
            std::ostringstream oss;
            oss << "calling conversion constructor with " << value;
            _PrintInfo( oss.str(), Scope::CONVERSION_CTOR);
        }
        Dummy( std::initializer_list<double> list) noexcept( IS_NOEXCEPT)
        {
            std::ostringstream oss;
            oss << "calling initializer constructor with { ";
            auto it = list.begin();
            if ( it != list.end()) {
                m_char_member = *it;
                oss << *it << ", ";
                ++ it;
            }
            if ( it != list.end()) {
                m_int_member = *it;
                oss << *it << ", ";
                ++ it;
            }
            if ( it != list.end()) {
                m_float_member = *it;
                oss << *it << ", ";
                ++ it;
            }
            if ( it != list.end()) {
                m_double_member = *it;
                oss << *it << ", ";
                ++ it;
            }
            oss << '}';
            _PrintInfo( oss.str(), Scope::INITER_LIST_CTOR);
        }

        // dtor:
        virtual ~Dummy() noexcept( IS_NOEXCEPT)
        {
            _PrintInfo( "calling destructor", Scope::DTOR);
        }

        // assignment operators:
        virtual Dummy &operator=( Dummy const &other) noexcept( IS_NOEXCEPT)
        {
            std::ostringstream oss;
            oss << "calling copy assignment operator";
            if ( _NeedPrint( Scope::ID))
                oss << " with Dummy#" << other.m_id;
            _PrintInfo( oss.str(), Scope::COPY_ASSIGN);
            m_char_member = other.m_char_member;
            m_int_member = other.m_int_member;
            m_float_member = other.m_float_member;
            m_double_member = other.m_double_member;
            return *this;
        }
        virtual Dummy &operator=( Dummy &&other) noexcept( IS_NOEXCEPT)
        {
            std::ostringstream oss;
            oss << "calling move assignment operator";
            if ( _NeedPrint( Scope::ID))
                oss << " with Dummy#" << other.m_id;
            _PrintInfo( oss.str(), Scope::MOVE_ASSIGN);
            m_char_member = other.m_char_member;
            m_int_member = other.m_int_member;
            m_float_member = other.m_float_member;
            m_double_member = other.m_double_member;
            return *this;
        }
        virtual Dummy &operator=( int value) noexcept( IS_NOEXCEPT)
        {
            std::ostringstream oss;
            oss << "calling conversion assignment operator with " << value;
            _PrintInfo( oss.str(), Scope::CONVERSION_ASSIGN);
            m_char_member = value;
            m_int_member = value;
            m_float_member = value;
            m_double_member = value;
            return *this;
        }
        virtual Dummy &operator=( std::initializer_list<double> const &list) noexcept( IS_NOEXCEPT)
        {
            std::ostringstream oss;
            oss << "calling initializer assignment operator with {";
            auto it = list.begin();
            if ( it != list.end()) {
                m_char_member = *it;
                oss << *it << ", ";
                ++ it;
            }
            if ( it != list.end()) {
                m_int_member = *it;
                oss << *it << ", ";
                ++ it;
            }
            if ( it != list.end()) {
                m_float_member = *it;
                oss << *it << ", ";
                ++ it;
            }
            if ( it != list.end()) {
                m_double_member = *it;
                oss << *it << ", ";
                ++ it;
            }
            oss << '}';
            _PrintInfo( oss.str(), Scope::INITER_LIST_ASSIGN);
            return *this;
        }

}; // class Dummy

#endif

