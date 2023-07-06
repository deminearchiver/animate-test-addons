project(
    qt-in-animate
    VERSION 0.0.1
    DESCRIPTION ""
    LANGUAGES CXX
)

qt_standard_project_setup()

file(
    GLOB qt-in-animate_SRC
    "src/*.cc"
)
qt_add_library(${PROJECT_NAME} SHARED "${qt-in-animate_SRC}")

target_link_libraries(${PROJECT_NAME} PRIVATE animate_jsapi Qt6::Widgets Qt6::GuiPrivate)
target_include_directories(${PROJECT_NAME} PRIVATE "src")
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE OFF MACOSX_BUNDLE OFF)

file(
    GLOB qt-in-animate_RESOURCE_THEMES
    "data/themes/*.qss"
)
qt_add_resources(
    ${PROJECT_NAME} "themes"
    PREFIX "/"
    BASE "data"
    FILES "${qt-in-animate_RESOURCE_THEMES}"
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "$ENV{LOCALAPPDATA}/Adobe/Animate 2023/en_US/Configuration/External Libraries"
)
