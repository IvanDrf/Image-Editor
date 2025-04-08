cmake_minimum_required(VERSION 3.20)

project(ImageEditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Brush/Brush.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Button/Button.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileField/FileField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Image/Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputField/InputField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/MainWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusBar/StatusBar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputWindow/InputWindow.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Brush/Brush.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Button/Button.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileField/FileField.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Image/Image.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputField/InputField.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/MainWindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusBar/StatusBar.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputWindow/InputWindow.hpp
)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

add_executable(ImageEditor ${SOURCES} ${HEADERS})

target_link_libraries(ImageEditor sfml-system sfml-window sfml-graphics)
