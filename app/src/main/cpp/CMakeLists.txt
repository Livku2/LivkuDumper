# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("LivkuDumper")


macro(SET_OPTION option value)
    set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

SET_OPTION(DOBBY_GENERATE_SHARED OFF)
SET_OPTION(Plugin.SymbolResolver OFF)
add_subdirectory(Dobby)


add_compile_definitions(kNO_KEYSTONE)
file(GLOB_RECURSE KITTY_MEMORY_SOURCES KittyMemory/KittyMemory/*.cpp)

add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp
        ${KITTY_MEMORY_SOURCES})


target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
        Dobby/include
        KittyMemory/KittyMemory)

target_link_libraries(${CMAKE_PROJECT_NAME}
        log
        dobby)