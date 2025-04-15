cmake_minimum_required(VERSION 3.1.0)

project(AstroProQt VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages
find_package(Qt5 COMPONENTS 
    Widgets 
    Network
    REQUIRED
)

# Project files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    chartwidget.cpp
    chartwidget.h
    Calculators/dashacalculator.cpp
    Calculators/dashacalculator.h
    Calculators/strengthcalculator.cpp
    Calculators/strengthcalculator.h
    Calculators/yogacalculator.cpp
    Calculators/yogacalculator.h
    resources.qrc
)

# Swiss Ephemeris source files
set(SWISSEPH_SOURCES
    swiss/sweph.c
    swiss/swephexp.h
)

# Swiss Ephemeris library
add_library(swisseph STATIC ${SWISSEPH_SOURCES})
target_include_directories(swisseph PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/swiss)

if(WIN32)
    target_compile_definitions(swisseph PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Main executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Calculators
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Widgets
    Qt5::Network
    swisseph
)

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_DEPRECATED_WARNINGS
    EPHE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/ephe"
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/styles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Create ephe directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ephe)
