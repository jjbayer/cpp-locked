#include "locked.hpp"

#include <iostream>
#include <thread>


struct Foo
{
    void bar()
    {
        std::cout << "Sleep for 1 second... " << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Finished bar() " << std::endl;
    }
};


int main()
{
    Foo a;

    std::thread t1([&a](){locked(a)->bar();});
    std::thread t2([&a](){locked(a)->bar();});

    t1.join();
    t2.join();
}
