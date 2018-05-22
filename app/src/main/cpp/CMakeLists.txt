# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# include glm library
add_subdirectory(glm)
message( STATUS "Architecture: ${ARCHITECTURE}" )
# include free type library
set(FREE_TYPE_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/libfreetype2.a")


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp
             common.h
             SurfaceApp.cpp
             Surface.h
             Surface.cpp
             WindowSurface.h
             WindowSurface.cpp
             opengl/EGLCore.h
             opengl/EGLCore.cpp
             opengl/ConfigComparator.h
             opengl/ConfigComparator.cpp
             opengl/Texture2DProgram.h
             opengl/Texture2DProgram.cpp
             opengl/GLUtil.h
             opengl/GLUtil.cpp
             opengl/EGLError.h
             opengl/EGLError.cpp
             opengl/Transformer.h
             opengl/Transformer.cpp
             opengl/Drawable2D.h
             opengl/Drawable2D.cpp
             opengl/ZoomDrawable2D.h
             opengl/ZoomDrawable2D.cpp
             opengl/FrameRect.h
             opengl/FrameRect.cpp
             opengl/Font.h
             opengl/Font.cpp
             opengl/FontTextureProgram.h
             opengl/FontTextureProgram.cpp
             )

target_include_directories(native-lib PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/glm
            ${CMAKE_CURRENT_SOURCE_DIR}/freetype2/include
            ${CMAKE_CURRENT_SOURCE_DIR}/opengl)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       android
                       mediandk
                       EGL
                       GLESv2
                       ${FREE_TYPE_LIB}
                      )