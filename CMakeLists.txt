cmake_minimum_required(VERSION 3.21)
project(enc LANGUAGES CXX)

find_package(botan CONFIG REQUIRED)

add_executable(enc main.cpp)
target_compile_features(enc PRIVATE cxx_std_20)
set(botan_target "")
foreach(candidate Botan::Botan Botan::Botan-static)
  if(TARGET ${candidate})
    set(botan_target ${candidate})
    break()
  endif()
endforeach()

if(NOT botan_target)
  message(FATAL_ERROR "No Botan target found")
endif()

target_link_libraries(enc PRIVATE ${botan_target})
