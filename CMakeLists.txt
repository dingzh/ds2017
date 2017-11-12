cmake_minimum_required(VERSION 3.8)
project(ds2017)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories( include )

set(SOURCE_FILES
        include/class-bint.hpp
        include/class-integer.hpp
        include/class-matrix.hpp
        include/deque.hpp
        include/exceptions.hpp
        include/map.hpp
        include/utility.hpp
        test/deque/deque-advan-1.cc
        test/deque/deque-advan-2.cc
        test/deque/deque-advan-3.cc
        test/deque/deque-advan-4.cc
        test/deque/deque-basic.cc
        test/map/map-advance-1.cc
        test/map/map-advance-2.cc
        test/map/map-advance-3.cc
        test/map/map-advance-4.cc
        test/map/map-advance-5.cc
        test/map/map-basic.cc
        test/map/map-hash.cc)

add_executable(deque-basic test/deque/deque-basic.cc include/deque.hpp)
add_executable(deque-advan-1 test/deque/deque-advan-1.cc include/deque.hpp)
add_executable(deque-advan-2 test/deque/deque-advan-2.cc include/deque.hpp)
add_executable(deque-advan-3 test/deque/deque-advan-3.cc include/deque.hpp)
add_executable(deque-advan-4 test/deque/deque-advan-4.cc include/deque.hpp)