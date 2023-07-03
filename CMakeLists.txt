cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(animate-test-addons)

set(CMAKE_PREFIX_PATH "C:/Qt/6.5.1/msvc2019_64/lib/cmake")
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_subdirectory("lib/animate_jsapi")
add_subdirectory("lib/pugixml")


add_subdirectory("animate11")
add_subdirectory("qt-in-animate")
add_subdirectory("altblocker")