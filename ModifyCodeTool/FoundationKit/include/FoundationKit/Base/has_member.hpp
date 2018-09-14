/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_HAS_MEMBER_HPP
#define FOUNDATIONKIT_HAS_MEMBER_HPP

#include <utility>
#include "FoundationKit/GenericPlatformMacros.hpp"
NS_FK_BEGIN

/**
 * this can detect inherited member functions!
 * @code
 *   struct AppDelegate
 *   {
 *      void applicationDidFinishLaunching(){}
 *   }
 *
 *   struct Scheduler
 *   {
 *      void update(float dt){}
 *   }
 *
 *   HAS_MEMBER(applicationDidFinishLaunching);
 *   HAS_MEMBER(update);
 *   bool bHave0 = has_member_applicationDidFinishLaunching<AppDelegate>::value; // true
 *   bool bHave1 = has_member_update<Scheduler,std::string>::value;   // false
 *   bool bHave2 = has_member_update<Scheduler,float>::value;  // true
 *
 *   template<typename T,typename... Args>
 *    typename std::enable_if<has_member_update<T,decltype(Args)...>::value>::type
 *    testFun()
 *     {
 *          std::cout<<"has update function" << std::endl;
 *     }
 *
 *    template<typename T,typename... Args>
 *    typename std::enable_if<!has_member_update<T,decltype(Args)...>::value>::type
 *    testFun()
 *     {
 *          std::cout<<"has no update function" << std::endl;
 *     }
 *     testFun<Scheduler, int>();   // output: has no update function
 *     testFun<Scheduler, float>(); // has update function
 * @endcode
 */
#define HAS_MEMBER(member)\
template<typename T, typename... Args>\
struct has_member_##member\
{\
private:\
	template<typename U> static auto check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
	template<typename U> static std::false_type check(...);\
public:\
	enum{value = std::is_same<decltype(check<T>(0)), std::true_type>::value};\
    using value_type = std::is_same<decltype(check<T>(0)), std::true_type>; \
};
NS_FK_END
#endif // FOUNDATIONKIT_HAS_MEMBER_HPP
