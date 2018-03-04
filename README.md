# cpp-locked
Per-object locking mechanism for exclusive access.

If an object ``x`` is accessed from multiple threads, simply wrap it like this:

```cpp
locked(x)->doSomething();
```

## How does it work?

``locked`` returns a scoped lock which is associated with the object's memory address. Because temporary objects live until [its containing full-expression has been evaluated](http://en.cppreference.com/w/cpp/language/lifetime), exclusive access is guaranteed until the call to ``doSomething()`` has returned.

## Example

```cpp
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
```
