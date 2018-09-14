/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_ANYFUNCTION_HPP
#define FOUNDATIONKIT_ANYFUNCTION_HPP

#include <type_traits>
#include <memory>
#include <utility>
#include <functional>
#include "FoundationKit/Foundation/Value.hpp"
#include "FoundationKit/Base/function_traits.hpp"
#include "FoundationKit/forward.hpp"

NS_FK_BEGIN

template<size_t N>
struct PackTuple
{
	template<typename Tuple>
	static void Pack(Tuple& tp, ValueList& args)
	{
		std::get<N - 1>(tp) = args[N - 1].As<typename std::decay<decltype(std::get<N - 1>(tp))>::type>();
		PackTuple<N - 1>::Pack(tp, args);
	}
};

template<>
struct PackTuple<0>
{
	template<typename Tuple>
	static void Pack(Tuple& tp, ValueList& args)
	{
		std::get<0>(tp) = args[0].As<typename std::decay<decltype(std::get<0>(tp))>::type>();
	}
};

template<typename Tuple>
inline void PackTupleFromValueList(Tuple& tp, const ValueList& args)
{
	PackTuple<std::tuple_size<Tuple>::value>::Pack(tp, const_cast<ValueList&>(args));
}

template<>
inline void PackTupleFromValueList(std::tuple<ValueList>& tp, const ValueList& args)
{
	std::get<0>(tp) = args;
}

template <typename T>
inline void PackArgsToValueList(ValueList& al, const T &t)
{
    al.emplace_back(t);
}


template <typename T, typename...Args>
inline void PackArgsToValueList(ValueList& al, const T &t, Args&&... args)
{
    al.emplace_back(t);
    PackArgsToValueList(al, std::forward<Args>(args)...);
}

class AnyFunctionBase
{
public:
	AnyFunctionBase(){}
	virtual~AnyFunctionBase(){}
	virtual Value Invoke(const ValueList& args) = 0;
    virtual size_t GetHashCode() = 0;

    template<typename _Rty, typename... Args>
    typename std::enable_if< !std::is_void<_Rty>::value, _Rty >::type Invoke(Args&&... args)
    {
        ValueList al;
        PackArgsToValueList(al, std::forward<Args>(args)...);
        return Invoke(al).As<_Rty>();
    }

    template<typename _Rty, typename... Args>
    typename std::enable_if< std::is_void<_Rty>::value, void >::type Invoke(Args&&... args)
    {
        ValueList al;
        PackArgsToValueList(al, std::forward<Args>(args)...);
        Invoke(al).As<void>();
    }
};

typedef std::shared_ptr<AnyFunctionBase>  AnyFunctionPointer;

/// Returns a boolean indicating whether the two given vectors are equal.
/// @param left The first vector to compare.
/// @param right The second vector to compare.
/// @return True if the vectors are equal; False otherwise.
static inline bool operator ==(const AnyFunctionPointer& left, const AnyFunctionPointer& right)
{
    return left->GetHashCode() == right->GetHashCode();
}

/// Returns a boolean indicating whether the two given vectors are not equal.
/// @param left The first vector to compare.
/// @param right The second vector to compare.
/// @return True if the vectors are not equal; False if they are equal.
static inline bool operator !=(const AnyFunctionPointer& left, const AnyFunctionPointer& right)
{
    return !(left == right);
}

template<typename _Ft, size_t ArgsNum>
class AnyFunction : public AnyFunctionBase
{
	using function_traits_t  = function_traits < _Ft >;
	using function_type      = typename function_traits_t::function_type;
	using argument_meta_type = typename function_traits_t::argument_meta_type;
    typedef AnyFunction<_Ft, ArgsNum>  _Mty;
public:
	function_type       Func;
	
	AnyFunction(const function_type& fun)
	{
		Func = fun;
	}

	virtual Value Invoke(const ValueList& args) override
	{
		using is_void = std::is_same<typename function_traits_t::return_type, void>;
		return InvokeSwitch<typename function_traits_t::return_type>(is_void{}, args);
	}

    virtual size_t GetHashCode() override
    {
        return typeid(_Mty).hash_code();
    }
protected:
	template <typename _Ty, typename = typename std::enable_if< !std::is_same<_Ty, void>::value, _Ty>::type >
	Value InvokeSwitch(std::false_type, const ValueList& args)
	{
		argument_meta_type  ArgsTuple;
		PackTupleFromValueList(ArgsTuple, args);
		return std::apply(Func, ArgsTuple);
	}

	template <typename _Ty, typename = typename std::enable_if< std::is_same<_Ty, void>::value, _Ty>::type >
	Value InvokeSwitch(std::true_type, const ValueList& args)
	{
		argument_meta_type  ArgsTuple;
		PackTupleFromValueList(ArgsTuple, args);
		std::apply(Func, ArgsTuple);
		return Value();
	}
};

template<typename _Ft>
class AnyFunction<_Ft, 0> : public AnyFunctionBase
{
	using function_traits_t = function_traits < _Ft >;
	using function_type     = typename function_traits_t::function_type;
    typedef AnyFunction<_Ft, 0>  _Mty;
public:
	function_type   Func;

	AnyFunction(const function_type& fun)
	{
		Func = fun;
	}

	virtual Value Invoke(const ValueList& /*args*/)
	{
		using is_void = std::is_same<typename function_traits_t::return_type, void>;
		return InvokeSwitch<typename function_traits_t::return_type >(is_void{});
	}

    virtual size_t GetHashCode() override
    {
        return typeid(_Mty).hash_code();
    }

protected:
	template <typename _Ty, typename = typename std::enable_if< !std::is_same<_Ty, void>::value, _Ty>::type >
	Value InvokeSwitch(std::false_type)
	{
		return Func();
	}

	template <typename _Ty, typename = typename std::enable_if< std::is_same<_Ty, void>::value, _Ty>::type >
	Value InvokeSwitch(std::true_type)
	{
		Func();
		return Value();
	}
};


namespace detail
{
	template<size_t _Index>
	struct PlaceHolders {};
	template<>
	struct PlaceHolders<0> { static decltype(std::placeholders::_1) Get() { return std::placeholders::_1; } };
	template<>
	struct PlaceHolders<1> { static decltype(std::placeholders::_2) Get() { return std::placeholders::_2; } };
	template<>
	struct PlaceHolders<2> { static decltype(std::placeholders::_3) Get() { return std::placeholders::_3; } };
	template<>
	struct PlaceHolders<3> { static decltype(std::placeholders::_4) Get() { return std::placeholders::_4; } };
	template<>
	struct PlaceHolders<4> { static decltype(std::placeholders::_5) Get() { return std::placeholders::_5; } };
	template<>
	struct PlaceHolders<5> { static decltype(std::placeholders::_6) Get() { return std::placeholders::_6; } };
	template<>
	struct PlaceHolders<6> { static decltype(std::placeholders::_7) Get() { return std::placeholders::_7; } };
	template<>
	struct PlaceHolders<7> { static decltype(std::placeholders::_8) Get() { return std::placeholders::_8; } };
	template<>
	struct PlaceHolders<8> { static decltype(std::placeholders::_9) Get() { return std::placeholders::_9; } };
	template<>
	struct PlaceHolders<9> { static decltype(std::placeholders::_10) Get() { return std::placeholders::_10; } };

	template<typename _Ft, typename _Ty, std::size_t... index >
	AnyFunctionPointer BindAnyFunctionImpl(_Ft fun, _Ty* object, std::index_sequence<index...>)
	{
		const size_t arityvalue = function_traits < _Ft >::arity::value;
		std::shared_ptr<AnyFunction<_Ft, arityvalue> > pSelector(new AnyFunction<_Ft, arityvalue >(std::bind(fun, object, PlaceHolders<index>::Get()...)));
		return pSelector;
	}

	template<typename _Ft, std::size_t... index >
	AnyFunctionPointer BindAnyFunctionImpl(_Ft fun, std::index_sequence<index...>)
	{
		const size_t arityvalue = function_traits < _Ft >::arity::value;
		std::shared_ptr<AnyFunction<_Ft, arityvalue> > pSelector(new AnyFunction<_Ft, arityvalue>(std::bind(fun, PlaceHolders<index>::Get()...)));
		return pSelector;
	}
} // namespace detail


template<typename _Ft, typename _Ty>
AnyFunctionPointer BindAnyFunction(_Ft fun, _Ty* object)
{
	return detail::BindAnyFunctionImpl(fun, object, std::make_index_sequence<function_traits < _Ft >::arity::value>{});
}

template<typename _Ft>
AnyFunctionPointer BindAnyFunction(_Ft fun)
{
	return detail::BindAnyFunctionImpl(fun, std::make_index_sequence<function_traits < _Ft >::arity::value>{});
}

NS_FK_END

#endif // END OF FOUNDATIONKIT_ANYFUNCTION_HPP
