#include "FoundationKit/GenericPlatformMacros.hpp"
NS_FK_BEGIN
#include <functional>

template<typename T>
class Task;

template<typename R, typename...Args>
class Task<R(Args...)>
{
public:
    typedef R                         return_type;
    typedef std::function<R(Args...)> function_type;

    Task(function_type&& func) :task_func(std::move(func)) {}
    Task(function_type& func) :task_func(func) {}

    template<typename F>
    auto Then(F&& f)->Task<typename std::result_of<F(R)>::type(Args...)>
    {
        typedef typename std::result_of<F(R)>::type ReturnType;
        auto func = std::move(task_func);
        return Task<ReturnType(Args...)>([func, &f](Args&&... args)
        {
            std::future<R> lastf = std::async(func, std::forward<Args>(args)...);
            return std::async(f, lastf.get()).get();
        });
    }

    ~Task()
    {

    }

    void Wait() const
    {
        value_future.wait();
    }

    template<class _Rep, class _Per>
    std::future_status WaitFor(const chrono::duration<_Rep, _Per>& _Rel_time) const
    {
        return value_future.wait_for(_Rel_time);
    }

    template<class _Clock, class _Dur>
    std::future_status WaitUntil(  const chrono::time_point<_Clock, _Dur>& _Abs_time) const
    {
        return value_future.wait_until(_Abs_time);
    }

    template<class _Rep, class _Per>
    std::future_status WaitUntil(const chrono::duration<_Rep, _Per>& _Rel_time) const
    {
        return WaitUntil(std::chrono::system_clock::now() + _Rel_time);
    }

    R Get()
    {
        return value_future.get();
    }

    template<typename... Args>
    std::shared_future<R> Run(Args&&... args)
    {
        value_future = std::async(task_func, std::forward<Args>(args)...);
        return value_future;
    }
private:
    function_type         task_func;
    std::shared_future<R> value_future;
};


#include <string>
#include <iostream>
#include <thread>
#include <chrono>



void TaskThen()
{
    Task<int()> t([] {return 32; });
    auto r1 = t.Then([](int result) {cout << result << endl; return result + 3; }).Then([](int result) {cout << result << endl; return result + 3; }).Run().get();

    //auto f1 = [](int result){cout << result << endl; return "ok"; };
    Task<int(int)> t1([](int i) {return i; });
    t1.Then([](int result) {return std::to_string(result); }).Then([](const string& str) {cout << str << endl; }).Run(1).get();

    Task<string(string)> t2([](string& str) {return str; });
    auto r2 = t2.Then([](string& str) { cout << str.c_str() << endl; return str + " ok"; }).Run("test").get();
    cout << r2 << endl;
}

static size_t NoCache(size_t n)
{
    return (n < 2) ? n : NoCache(n - 1) + NoCache(n - 2);
}

static void TestTask()
{
    TaskThen();

    std::cout << "TestTask "<< std::this_thread::get_id() << std::endl;
    Task<size_t(size_t)> Task0([](size_t arg) 
    {
        std::cout << "NoCache " << std::this_thread::get_id() << std::endl;
        return NoCache(arg);
    
    });
    Task0.Run(42);
    //Task0.Wait();
    //std::cout << Task0.Get() << std::endl;
}


NS_FK_END