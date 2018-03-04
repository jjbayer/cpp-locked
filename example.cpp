#include "locked.hpp"

#include <iostream>
#include <thread>


void writeChars(std::ostream & out, char start, char end)
{
    for(char c = start; c <= end; c++) out << c;
    out << "\n";
} 

int main()
{
    // Prints something like
    // abcdefghijklmnopAqBCDrEFGHstuvwxIyzJKL
    // MNOPQRSTUVWXYZ
    std::thread t1([](){ writeChars(std::cout, 65,  90); });
    std::thread t2([](){ writeChars(std::cout, 97, 122); });
    t1.join();
    t2.join();
    std::cout << std::endl;

    // Prints
    // ABCDEFGHIJKLMNOPQRSTUVWXYZ
    // abcdefghijklmnopqrstuvwxyz
    std::thread t3([](){ writeChars(*locked(std::cout), 65,  90); });
    std::thread t4([](){ writeChars(*locked(std::cout), 97, 122); });
    t3.join();
    t4.join();
    std::cout << std::endl;

    return 0;
}
