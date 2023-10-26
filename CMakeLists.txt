project (KOMPUTE_TEST)

cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(kompute)

add_executable(komputeTest Main.cpp)

set_target_properties(komputeTest PROPERTIES
    DEBUG_POSTFIX _DEBUG 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)

target_include_directories(komputeTest PUBLIC kompute/src/include)
target_link_libraries(komputeTest PUBLIC  kompute)