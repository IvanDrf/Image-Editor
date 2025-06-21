cmake_minimum_required(VERSION 3.20)

project(ImageEditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Back/Back.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Brush/Brush.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Button/Button.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileField/FileField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Image/Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusBar/StatusBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/Interface/Interface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/Interface/Config/Config.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/Interface/Windows/Windows.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/Interface/Positions/Positions.cpp
)


find_package(SFML 3.0 COMPONENTS System Window Graphics REQUIRED)

add_executable(ImageEditor ${SOURCES})

target_compile_options(ImageEditor PUBLIC -g)


target_link_libraries(ImageEditor 
    PRIVATE 
    SFML::System 
    SFML::Window 
    SFML::Graphics
)