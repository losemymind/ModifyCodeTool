/****************************************************************************
  Copyright (c) 2018 libo All rights reserved.
  
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_SPINLOCK_HPP
#define FOUNDATIONKIT_SPINLOCK_HPP

#include <atomic>
#include <thread>
#include <chrono>
class spinlock
{
    volatile std::atomic_flag lck;
public:
    spinlock()
    {
        lck.clear();
    }

    void lock()
    {
        if (std::atomic_flag_test_and_set_explicit(&lck, std::memory_order_acquire))
        {
            auto delta_time = std::chrono::milliseconds(1); //1ms
            while (std::atomic_flag_test_and_set_explicit(&lck, std::memory_order_acquire))
            {
                std::this_thread::sleep_for(delta_time);
                delta_time *= 2;
            }
        }
    }

    bool try_lock()
    {
        return (!std::atomic_flag_test_and_set_explicit(&lck, std::memory_order_acquire));
    }

    void unlock()
    {
        std::atomic_flag_clear_explicit(&lck, std::memory_order_release);
    }
};

#endif // END OF FOUNDATIONKIT_SPINLOCK_HPP