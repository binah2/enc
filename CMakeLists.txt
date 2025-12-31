cmake_minimum_required(VERSION 3.21)
project(enc LANGUAGES CXX)

find_package(botan CONFIG REQUIRED)

add_executable(enc main.cpp)
target_compile_features(enc PRIVATE cxx_std_20)
# Ensure a unified target name even when only the static library is available.
if(NOT TARGET Botan::Botan AND TARGET Botan::Botan-static)
  add_library(Botan::Botan ALIAS Botan::Botan-static)
endif()

target_link_libraries(enc PRIVATE Botan::Botan)
