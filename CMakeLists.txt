cmake_minimum_required(VERSION 3.10)
project(ech-test)


add_executable(ech-test dns.cpp)
target_link_libraries(ech-test ldns)
target_compile_features(ech-test PUBLIC cxx_std_17)
