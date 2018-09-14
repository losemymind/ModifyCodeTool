/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_ANY_HPP
#define FOUNDATIONKIT_ANY_HPP

#include "functional.hpp"

#if CPP_CURRENT_VERSION >= CPP_VERSION_17
#include <any>
#else
#include <type_traits>

namespace std
{
    class bad_any_cast : public std::bad_cast
    {
    public:
        virtual const char * what() const _NOEXCEPT override
        {
            return "std::bad_any_cast: "
                "failed conversion using std::any_cast";
        }
    };

    class any
    {
    public:
        any()_NOEXCEPT
            : content(0)
        {
        }

        template<typename _Ty>
        any(const _Ty & value)
            : content(new holder<std::decay_t<_Ty >>(value))
        {
        }

        any(const any & other)
            : content(other.content ? other.content->clone() : 0)
        {
        }

        // Move constructor
        any(any&& other) _NOEXCEPT
            : content(other.content)
        {
            other.content = 0;
        }

        // Perfect forwarding of _Ty
        template<typename _Ty>
        any(_Ty&& value
            , typename stdx::disable_if<std::is_same<any&, _Ty>::value >::type* = 0 // disable if value has type `any&`
            , typename stdx::disable_if<std::is_const<_Ty>::value>::type* = 0) // disable if value has type `const _Ty&&`
            : content(new holder< typename std::decay<_Ty>::type >(static_cast<_Ty&&>(value)))
        {
        }


        ~any() _NOEXCEPT
        {
            delete content;
        }

    public: // modifiers

        any & swap(any & rhs) _NOEXCEPT
        {
            std::swap(content, rhs.content);
            return *this;
        }

        any & operator=(const any& rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        // move assignement
        any & operator=(any&& rhs) _NOEXCEPT
        {
            rhs.swap(*this);
            any().swap(rhs);
            return *this;
        }

        // Perfect forwarding of _Ty
        template <class _Ty>
        any & operator=(_Ty&& rhs)
        {
            any(static_cast<_Ty&&>(rhs)).swap(*this);
            return *this;
        }

    public: // queries

        bool empty() const _NOEXCEPT
        {
            return !content;
        }

        void clear() _NOEXCEPT
        {
            any().swap(*this);
        }

        const type_info& type() const _NOEXCEPT
        {
            return content ? content->type() : typeid(void);
        }
    private:
        class placeholder
        {
        public: // structors

            virtual ~placeholder()
            {
            }

        public: // queries

            virtual const type_info& type() const _NOEXCEPT = 0;

            virtual placeholder * clone() const = 0;

        };

        template<typename _Ty>
        class holder : public placeholder
        {
        public: // structors

            holder(const _Ty & value)
                : held(value)
            {
            }

            holder(_Ty&& value)
                : held(static_cast<_Ty&&>(value))
            {
            }

        public: // queries

            virtual const type_info& type() const _NOEXCEPT
            {
                return typeid(_Ty);
            }

            virtual placeholder * clone() const
            {
                return new holder(held);
            }

        public: // representation

            _Ty held;

        private: // intentionally left unimplemented
            holder & operator=(const holder &) = delete;
        };
    private: // representation

        template<typename _Ty>
        friend _Ty * any_cast(any *) _NOEXCEPT;

        template<typename _Ty>
        friend _Ty * unsafe_any_cast(any *) _NOEXCEPT;
        placeholder * content;

    };

    inline void swap(any & lhs, any & rhs) _NOEXCEPT
    {
        lhs.swap(rhs);
    }

    template<typename _Ty>
    _Ty * any_cast(any * operand) _NOEXCEPT
    {
        return operand && operand->type() == typeid(_Ty)
            ? std::addressof(static_cast<any::holder<std::decay_t<_Ty>> *>(operand->content)->held)
            : 0;
    }

    template<typename _Ty>
    inline const _Ty * any_cast(const any * operand) _NOEXCEPT
    {
        return any_cast<_Ty>(const_cast<any *>(operand));
    }

    template<typename _Ty>
    _Ty any_cast(any & _Any)
    {
        static_assert(std::is_constructible<_Ty, std::remove_cv_t<std::remove_reference_t<_Ty>>&>::value,
            "any_cast<T>(any&) requires T to be constructible from remove_cv_t<remove_reference_t<T>>&");
        const auto _Ptr = any_cast<std::remove_cv_t<std::remove_reference_t<_Ty>>>(&_Any);
        if (!_Ptr)
        {
            throw bad_any_cast();
        }

        return (static_cast<_Ty>(*_Ptr));
    }

    template<typename _Ty>
    inline _Ty any_cast(const any & _Any)
    {
        static_assert(std::is_constructible<_Ty, const std::remove_cv_t<std::remove_reference_t<_Ty>>&>::value,
            "any_cast<T>(const any&) requires T to be constructible from const remove_cv_t<remove_reference_t<T>>&");
        const auto _Ptr = any_cast<std::remove_cv_t<std::remove_reference_t<_Ty>>>(&_Any);
        if (!_Ptr)
        {
            throw bad_any_cast();
        }

        return (static_cast<_Ty>(*_Ptr));
    }

    template<typename _Ty>
    inline _Ty any_cast(any&& _Any)
    {
        static_assert(std::is_constructible<_Ty, std::remove_cv_t<std::remove_reference_t<_Ty>>>::value,
            "any_cast<_Ty>(any&&) requires _Ty to be constructible from remove_cv_t<remove_reference_t<_Ty>>");
        const auto _Ptr = any_cast<std::remove_cv_t<std::remove_reference_t<_Ty>>>(&_Any);
        if (!_Ptr)
        {
            throw bad_any_cast();
        }

        return (static_cast<_Ty>(std::move(*_Ptr)));
    }


    template<typename _Ty>
    inline _Ty * unsafe_any_cast(any * _Any) _NOEXCEPT
    {
        return std::addressof(static_cast<any::holder<_Ty> *>(_Any->content)->held);
    }

    template<typename _Ty>
    inline const _Ty * unsafe_any_cast(const any * _Any) _NOEXCEPT
    {
        return unsafe_any_cast<_Ty>(const_cast<any *>(_Any));
    }
}

#endif //#if CPP_CURRENT_VERSION >= CPP_VERSION_17

#endif // END OF FOUNDATIONKIT_ANY_HPP
