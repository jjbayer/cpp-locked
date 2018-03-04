#pragma once

#include <mutex>
#include <unordered_map>


/// Thread-safe mutex manager
class MutexMap
{
public:

    /// Create mutex if necessary, increase reference counter
    std::mutex & getMutex(void * address);

    /// Decrease reference counter, delete mutex if necessary
    void leaveMutex(void * address);

private:

    struct CountingMutex
    {
        std::mutex mutex;
        int counter = 0;
    };

    std::mutex _mutex;
    std::unordered_map<void *, CountingMutex> _map;
};
