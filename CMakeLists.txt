project(cpp-locked)
set(CMAKE_CXX_STANDARD 14)

add_executable(example example.cpp mutexmap.cpp)
target_link_libraries(example pthread)
