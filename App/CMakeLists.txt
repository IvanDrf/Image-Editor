cmake_minimum_required(VERSION 3.20)
project(ImageEditor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)


set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Brush/Brush.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Button/Button.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileField/FileField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Image/Image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputField/InputField.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/MainWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainMethods/MainMethods.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusBar/StatusBar.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Brush/Brush.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Button/Button.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileField/FileField.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Image/Image.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InputField/InputField.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow/MainWindow.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MainMethods/MainMethods.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StatusBar/StatusBar.hpp
)

add_executable(ImageEditor ${SOURCES} ${HEADERS})

target_include_directories(ImageEditor PRIVATE ${GTK3_INCLUDE_DIRS})
target_link_libraries(ImageEditor ${GTK3_LIBRARIES} sfml-system sfml-window sfml-graphics)

target_compile_options(ImageEditor PRIVATE ${GTK3_CFLAGS_OTHER})
