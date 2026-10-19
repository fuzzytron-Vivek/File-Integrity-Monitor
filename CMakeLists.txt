cmake_minimum_required(VERSION 3.16)

project (fim LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(fim  
  src/main.cpp
  src/fim.cpp
)

target_link_libraries(fim PRIVATE 
  OpenSSL::Crypto
  nlohmann_json::nlohmann_json
)
target_include_directories(fim PRIVATE include)

