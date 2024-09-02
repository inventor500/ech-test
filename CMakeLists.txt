cmake_minimum_required(VERSION 3.10)
project(ech-test)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -Wpedantic -g -Og")
set(CMAKE_CXX_FLAGS_DEBUG_RELEASE "-Wall -Wpedantic -Ofast")


add_library(dns STATIC dns.cpp dns.hpp)
target_link_libraries(dns ldns)

add_library(request STATIC request.hpp request.cpp)
target_link_libraries(request curl)

add_executable(ech-test main.cpp)
target_link_libraries(ech-test PUBLIC dns request)
target_compile_features(ech-test PUBLIC cxx_std_17)
