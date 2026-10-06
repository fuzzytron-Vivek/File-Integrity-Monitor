cmake_minimum_required(VERSION 3.16)

project (fim LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)


add_executable(fim  
  src/main.cpp
  src/fim.cpp
)

target_link_libraries(fim PRIVATE OpenSSL::Crypto)
target_include_directories(fim PRIVATE include)

