cmake_minimum_required(VERSION 2.8.4)
project(CyclicBarrier)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")

# Generate shared library libCyclicBarrier.so file.
ADD_LIBRARY(CyclicBarrier SHARED CyclicBarrier.cpp)

# Add test Examples below.
add_executable(Example1.out Example1.cpp)
add_executable(Example2.out Example2.cpp)
add_executable(Example3.out Example3.cpp)
add_executable(Example4.out Example4.cpp)
add_executable(Example5.out Example5.cpp)
add_executable(Example6.out Example6.cpp)
add_executable(Example7.out Example7.cpp)

target_link_libraries(Example1.out CyclicBarrier pthread PocoFoundation)
target_link_libraries(Example2.out CyclicBarrier pthread PocoFoundation)
target_link_libraries(Example3.out CyclicBarrier pthread PocoFoundation)
target_link_libraries(Example4.out CyclicBarrier pthread PocoFoundation)
target_link_libraries(Example5.out CyclicBarrier pthread PocoFoundation)
target_link_libraries(Example6.out CyclicBarrier pthread PocoFoundation)
target_link_libraries(Example7.out CyclicBarrier pthread PocoFoundation)
