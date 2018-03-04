#pragma once

#include "mutexmap.hpp"


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



