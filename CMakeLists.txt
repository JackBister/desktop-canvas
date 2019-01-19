cmake_minimum_required(VERSION 3.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(desktop-canvas)

# ===SET PLATFORM DIRECTORIES===
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(IS_X64 TRUE)
    if (UNIX)
        set(PLATFORM_DIRECTORY installed/x64-linux/)
        set(PLATFORM_STATIC_DIRECTORY installed/x64-linux-static/)
    else()
        set(PLATFORM_DIRECTORY installed/x64-windows/)
        set(PLATFORM_STATIC_DIRECTORY installed/x64-windows-static/)
    endif()
else()
    set(IS_X64 FALSE)
    if (UNIX)
        set(PLATFORM_DIRECTORY installed/x86-linux/)
        set(PLATFORM_STATIC_DIRECTORY installed/x86-linux-static/)
    else()
        set(PLATFORM_DIRECTORY installed/x86-windows/)
        set(PLATFORM_STATIC_DIRECTORY installed/x86-windows-static/)
    endif()
endif()
# ===END SET PLATFORM DIRECTORIES===

# === MAIN EXECUTABLE ===
file(GLOB_RECURSE SOURCES src/*.c src/*.cpp src/*.h)
add_executable(Main ${SOURCES})

message(${SOURCES})

# Exclude Android stuff from build (but still show it in VS)
file(GLOB_RECURSE ANDROID_SOURCES src/*_android.*)
set_source_files_properties(${ANDROID_SOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

#set unicode charset in VS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")

# General compiler options
target_compile_features(Main PUBLIC cxx_std_17)
# Needed for appveyor's VS version to build websocketpp
target_compile_definitions(Main PRIVATE _HAS_AUTO_PTR_ETC 1)

# Includes
target_include_directories(Main PUBLIC src)
target_include_directories(Main SYSTEM PUBLIC ${PLATFORM_DIRECTORY}include ${PLATFORM_STATIC_DIRECTORY}include)

# Links
target_link_directories(Main PUBLIC ${PLATFORM_DIRECTORY}debug/lib)
if (UNIX) 
    target_link_libraries(Main ${CMAKE_DL_LIBS} SDL2d SDL2_ttf crypto dl freetyped bz2d png stdc++fs z)
else()
    target_link_libraries(Main SDL2d SDL2_ttf)
endif()
# ===END MAIN EXECUTABLE===

# ===GROUP BY FOLDERS===
source_group(TREE src FILES ${SOURCES})
# ===END GROUP BY FOLDERS===
