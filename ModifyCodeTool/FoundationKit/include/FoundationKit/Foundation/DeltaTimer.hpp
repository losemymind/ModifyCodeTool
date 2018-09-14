/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_ELAPSEDTIMER_HPP
#define FOUNDATIONKIT_ELAPSEDTIMER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <chrono>
#include "FoundationKit/GenericPlatformMacros.hpp"

namespace std
{
    namespace chrono
    {
        typedef duration<int,    std::ratio<3600 * 24> > day;
        typedef duration<double, std::nano> nanoseconds_f;
        typedef duration<double, std::micro> microseconds_f;
        typedef duration<double, std::milli> milliseconds_f;
        typedef duration<double> seconds_f;
        typedef duration<double, std::ratio<60> > minutes_f;
        typedef duration<double, std::ratio<3600> > hours_f;
        typedef duration<double, std::ratio<3600 * 24> > day_f;
    }
}

NS_FK_BEGIN

class DeltaTimer final
{
public:
    typedef std::chrono::high_resolution_clock clock_type;
    DeltaTimer() : _begin(clock_type::now()){}
    long long Nanoseconds() const{ return Elapsed<std::chrono::nanoseconds>(); }
    long long Microseconds()const{ return Elapsed<std::chrono::microseconds>(); }
    long long Milliseconds()const{ return Elapsed<std::chrono::milliseconds>(); }
    long long Seconds()     const{ return Elapsed<std::chrono::seconds>(); }
    long long Minutes()     const{ return Elapsed<std::chrono::minutes>(); }
    long long Hours()       const{ return Elapsed<std::chrono::hours>(); }
    long long Days()        const{ return Elapsed<std::chrono::day>(); }
    double Nanosecondsf()   const{ return Elapsed<std::chrono::nanoseconds_f>(); }
    double Microsecondsf()  const{ return Elapsed<std::chrono::microseconds_f>(); }
    double Millisecondsf()  const{ return Elapsed<std::chrono::milliseconds_f>(); }
    double Secondsf()       const{ return Elapsed<std::chrono::seconds_f>(); }
    double Minutesf()       const{ return Elapsed<std::chrono::minutes_f>(); }
    double Hoursf()         const{ return Elapsed<std::chrono::hours_f>(); }
    double Daysf()          const{ return Elapsed<std::chrono::day_f>(); }
    void   Reset(){ _begin = clock_type::now(); }
    template<typename Duration = std::chrono::milliseconds>
    bool   IsElapsed(const Duration& _Rel_time) const
    {
        return Elapsed<Duration>() >= _Rel_time.count();
    }

    bool operator == (const DeltaTimer& dt) const
    {
        return (_begin.time_since_epoch().count() == dt._begin.time_since_epoch().count());
    }
    bool operator != (const DeltaTimer& dt) const
    {
        return (_begin.time_since_epoch().count() != dt._begin.time_since_epoch().count());
    }
    bool operator >  (const DeltaTimer& dt) const
    {
        return (_begin.time_since_epoch().count() > dt._begin.time_since_epoch().count());
    }
    bool operator >= (const DeltaTimer& dt) const
    {
        return (_begin.time_since_epoch().count() >= dt._begin.time_since_epoch().count());
    }
    bool operator <  (const DeltaTimer& dt) const
    {
        return (_begin.time_since_epoch().count() < dt._begin.time_since_epoch().count());
    }
    bool operator <= (const DeltaTimer& dt) const
    {
        return (_begin.time_since_epoch().count() <= dt._begin.time_since_epoch().count());
    }
protected:
    template<typename Duration = std::chrono::milliseconds>
    typename Duration::rep Elapsed()const
    {
        return std::chrono::duration_cast<Duration>(clock_type::now() - _begin).count();
    }
private:
    clock_type::time_point _begin;
};

NS_FK_END

#endif // FOUNDATIONKIT_ELAPSEDTIMER_HPP
