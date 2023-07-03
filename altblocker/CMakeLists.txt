project(
    altblocker
    VERSION 0.0.1
    DESCRIPTION "Prevents Alt from blocking keyboard input in Adobe Animate"
    LANGUAGES CXX
)

file(
    GLOB altblocker_SRC
    "src/*.cc"
)
add_library(${PROJECT_NAME} SHARED "${altblocker_SRC}")

target_link_libraries(${PROJECT_NAME} PRIVATE animate_jsapi)
target_include_directories(${PROJECT_NAME} PRIVATE "src")
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE OFF MACOSX_BUNDLE OFF)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "$ENV{LOCALAPPDATA}/Adobe/Animate 2023/en_US/Configuration/External Libraries"
)
