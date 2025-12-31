cmake_minimum_required(VERSION 3.21)
project(enc LANGUAGES CXX)

find_package(botan CONFIG REQUIRED)

add_executable(enc main.cpp)
target_compile_features(enc PRIVATE cxx_std_20)
if(TARGET Botan::Botan)
  set(_botan_target Botan::Botan)
elseif(TARGET Botan::Botan-static)
  set(_botan_target Botan::Botan-static)
else()
  message(FATAL_ERROR "No Botan target found")
endif()

target_link_libraries(enc PRIVATE ${_botan_target})
