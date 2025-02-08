add_library(process process.hpp process.cpp)

target_include_directories(process PUBLIC .)

target_link_libraries(process parsing linkedlist)