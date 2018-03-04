#pragma once

#include <unordered_map>
#include <mutex>

/// Thread-safe mutex manager
class MutexMap
{
public:

    /// Create mutex if necessary, increase reference counter
    std::mutex & getMutex(void * address)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        auto & entry = _map[address]; // Creates entry if does not exist
        entry.counter++;

        return entry.mutex;
    }

    /// Decrease reference counter, delete mutex if necessary
    void leaveMutex(void * address)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        const auto it = _map.find(address);
        auto & entry = it->second; // must exist
        if( --entry.counter == 0) {
            _map.erase(it);
        }
    }

private:

    struct CountingMutex
    {
        std::mutex mutex;
        int counter = 0;
    };

    std::mutex _mutex;
    std::unordered_map<void *, CountingMutex> _map;
};


template<typename T>
class LockedObject
{
    public:

        LockedObject(T & object)
            :_object(&object)
            ,_lock(getMutexMap().getMutex(_object))
        {

        }

        /// Move constructor
        LockedObject(LockedObject && other)
            :_object(nullptr)
            ,_lock()
        {
            std::swap(_object, other._object);
            std::swap(_lock, other._lock);
        }

        ~LockedObject()
        {
            _lock.unlock();
            getMutexMap().leaveMutex(_object);
        }

    public:
        T * operator->() { return _object; }

    private:

        /// Class-wide mutex map
        MutexMap & getMutexMap()
        {
            static MutexMap map;

            return map;
        }

        T * _object;
        std::unique_lock<std::mutex> _lock;
};


/// Convenience wrapper
template<typename T>
LockedObject<T> locked(T & object)
{
    return LockedObject<T>(object);
}



