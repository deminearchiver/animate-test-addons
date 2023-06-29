project(animate11 VERSION 0.0.1 LANGUAGES CXX)

file(
    GLOB animate11_SRC
    "src/*.cc"
)
add_library(${PROJECT_NAME} SHARED ${animate11_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE "src")
target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    dwmapi.dll
    animate_jsapi
    pugixml::static
)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION "$ENV{LOCALAPPDATA}/Adobe/Animate 2023/en_US/Configuration/External Libraries"
)
