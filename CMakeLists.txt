cmake_minimum_required(VERSION 2.8.11)

project(testproject)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the QtWidgets library
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test)
find_package(OpenCV REQUIRED)

set(CORE_SOURCES core/wave.cpp
        core/waveprocessor.cpp)

# Tell CMake to create the helloworld executable
add_executable(wave
        main.cpp
        gui/mainwindow.cpp
        graphics_items/QGraphicsCursorItem.cpp
        gui/drawcanvas.cpp
        ${CORE_SOURCES})

add_executable(
        test_wave
        tests/main.cpp
        tests/test_wave.cpp
        ${CORE_SOURCES}
)

# Use the Widgets module from Qt 5.
target_link_libraries(wave Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Test ${OpenCV_LIBS} )
target_link_libraries(test_wave Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Test ${OpenCV_LIBS} )

find_program(VALGRIND "valgrind")
if(VALGRIND)
    add_custom_target(valgrind_memcheck
            COMMAND "${VALGRIND}" --tool=memcheck --leak-check=yes --num-callers=20 --track-fds=yes $<TARGET_FILE:test_wave>)
    add_custom_target(valgrind_profile
            COMMAND "${VALGRIND}" --tool=callgrind --callgrind-out-file=callgrind.out $<TARGET_FILE:test_wave>)
endif()