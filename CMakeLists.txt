cmake_minimum_required(VERSION 3.8)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(desktop-canvas C CXX)

# === MAIN EXECUTABLE ===
file(GLOB_RECURSE SOURCES src/*.c src/*.cpp src/*.h)
add_executable(Main ${SOURCES})

# Exclude Android stuff from build (but still show it in VS)
file(GLOB_RECURSE ANDROID_SOURCES src/*_android.*)
set_source_files_properties(${ANDROID_SOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

# Set unicode charset in VS
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE /STD /std:c++17")
else()
    # I don't trust CMake to properly set std=c++17
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs")
endif()


# General compiler options
# Needed for appveyor's VS version to build websocketpp
target_compile_definitions(Main PRIVATE _HAS_AUTO_PTR_ETC)

# Includes
find_path(ASIO_INCLUDE_DIR asio.hpp)
find_path(SDL2_INCLUDE_DIR SDL2/SDL.h)
find_path(SDL2_MIXER_INCLUDE_DIR SDL2/SDL_mixer.h)
find_path(SDL2_TTF_INCLUDE_DIR SDL2/SDL_ttf.h)
find_path(WEBSOCKETPP_INCLUDE_DIR websocketpp/client.hpp)
target_include_directories(Main PUBLIC src)
target_include_directories(Main SYSTEM PUBLIC ${ASIO_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${WEBSOCKETPP_INCLUDE_DIR})

# Links
find_library(SDL2_LIBRARY SDL2)
find_library(SDL2_MIXER_LIBRARY SDL2_mixer)
find_library(SDL2_TTF_LIBRARY SDL2_ttf)
target_link_libraries(Main ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_TTF_LIBRARY})
if (UNIX)
        find_package(Threads)
        find_library(BZ2_LIBRARY bz2)
        find_library(FLAC_LIBRARY FLAC)
        find_library(FREETYPE_LIBRARY freetype)
        find_library(MODPLUG_LIBRARY modplug)
        find_library(MPG123_LIBRARY mpg123)
        find_library(OGG_LIBRARY ogg)
        find_library(OUT123_LIBRARY out123)
        find_library(PNG_LIBRARY png)
        find_library(VORBIS_LIBRARY vorbis)
        find_library(VORBISENC_LIBRARY vorbisenc)
        find_library(VORBISFILE_LIBRARY vorbisfile)
        find_library(ZLIB_LIBRARY z)
        target_link_libraries(Main ${CMAKE_DL_LIBS}
                ${FREETYPE_LIBRARY}
                ${BZ2_LIBRARY}
                ${PNG_LIBRARY}
                ${CMAKE_THREAD_LIBS_INIT}
                ${ZLIB_LIBRARY}
                ${FLAC_LIBRARY}
                ${MODPLUG_LIBRARY}
                ${MPG123_LIBRARY}
                ${OUT123_LIBRARY}
                ${VORBISENC_LIBRARY}
                ${VORBISFILE_LIBRARY}
                ${VORBIS_LIBRARY}
                ${OGG_LIBRARY}
        /usr/lib/gcc/x86_64-linux-gnu/8/libstdc++fs.a)
endif()
# ===END MAIN EXECUTABLE===

# ===GROUP BY FOLDERS===
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src FILES ${SOURCES})
# ===END GROUP BY FOLDERS===
