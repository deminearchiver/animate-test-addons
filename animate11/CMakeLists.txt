project(
    animate11
    VERSION 0.1.3
    DESCRIPTION "This addon colors all titlebars in Adobe Animate the same as their windows"
    LANGUAGES CXX
)

# qt_standard_project_setup()

# configure_file(
#     "src/version.rc.in"
#     "version.rc"
#     @ONLY
# )

file(
    GLOB animate11_SRC
    "src/*.cpp"
)
# qt_add_library(${PROJECT_NAME} SHARED "${animate11_SRC}")
add_library(${PROJECT_NAME} SHARED "${animate11_SRC}")

target_link_libraries(${PROJECT_NAME} PRIVATE animate_jsapi pugixml::static)
target_include_directories(${PROJECT_NAME} PRIVATE "src")
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE OFF MACOSX_BUNDLE OFF)

# qt_add_resources(
#     ${PROJECT_NAME} "themes"
#     PREFIX "/"
#     BASE "data"
#     FILES "data/themes/dark.qss"
# )

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "$ENV{LOCALAPPDATA}/Adobe/Animate 2024/en_US/Configuration/External Libraries"
)
