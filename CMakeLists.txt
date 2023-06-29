cmake_minimum_required(VERSION 3.25.0)

project(animate-test-addons VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("lib/animate_jsapi")
add_subdirectory("lib/pugixml")
add_subdirectory("animate11")
