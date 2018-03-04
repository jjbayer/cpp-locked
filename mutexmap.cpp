#include "mutexmap.hpp"

std::mutex &MutexMap::getMutex(void *address)
{
    std::lock_guard<std::mutex> lock(_mutex);

    auto & entry = _map[address]; // Creates entry if does not exist
    entry.counter++;

    return entry.mutex;
}

void MutexMap::leaveMutex(void *address)
{
    std::lock_guard<std::mutex> lock(_mutex);

    const auto it = _map.find(address);
    auto & entry = it->second; // must exist
    if( --entry.counter == 0) {
        _map.erase(it);
    }
}
