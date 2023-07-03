project(
    animate11
    VERSION 0.0.1
    DESCRIPTION "This addon colors all titlebars in Adobe Animate the same as their windows"
    LANGUAGES CXX
)

file(
    GLOB animate11_SRC
    "src/*.cc"
)
add_library(${PROJECT_NAME} SHARED "${animate11_SRC}")

target_link_libraries(${PROJECT_NAME} PRIVATE animate_jsapi dwmapi.dll pugixml::static)
target_include_directories(${PROJECT_NAME} PRIVATE "src")
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE OFF MACOSX_BUNDLE OFF)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "$ENV{LOCALAPPDATA}/Adobe/Animate 2023/en_US/Configuration/External Libraries"
)
