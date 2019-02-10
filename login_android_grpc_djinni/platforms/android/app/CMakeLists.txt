# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

include_directories(
        ../../../deps/support-lib/
        ../../../deps/support-lib/jni/
        ../../../generated-src/cpp/
        ../../../generated-src/jni/
        ../../../src/cpp/
        src/main/cpp/
)

file(GLOB login_sources
        ../../../deps/support-lib/jni/*.hpp
        ../../../deps/support-lib/jni/*.cpp
        ../../../generated-src/jni/*.hpp
        ../../../generated-src/jni/*.cpp
        src/main/cpp/*.h
        src/main/cpp/*.cpp
        ../../../src/cpp/*.hpp
        ../../../src/cpp/*.h
        ../../../src/cpp/*.cpp
        )

add_library( # Sets the name of the library.
        mylogin

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${login_sources}
        )

target_link_libraries(mylogin
        android
        log)

