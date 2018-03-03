#pragma once

#include <unordered_map>
#include <mutex>

template<typename T>
class LockedObject
{
    public:

        LockedObject(T & object)
            :_lock(getMutex(object))
            ,_object(&object)
        {

        }

    public:
        T * operator->() { return _object; }

    private:

        std::mutex & getMutex(const T & object)
        {
            static std::unordered_map<const void*, std::mutex> objectMutexes;
            static std::mutex mapMutex;

            // Only one object of type T can access the mutex map at a time
            std::lock_guard<std::mutex> mapLock(mapMutex);

            // TODO: refcount mutexes, delete when done
            return objectMutexes[static_cast<const void*>(&object)];
        }

        std::unique_lock<std::mutex> _lock;
        T *const _object;
};


/// Convenience wrapper
template<typename T>
LockedObject<T> locked(T & object)
{
    return LockedObject<T>(object);
}



