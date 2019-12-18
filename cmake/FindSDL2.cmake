set(SDL2_VERSION 2.0.9)

if (SDL2_PATH)
    message("[USING SDL2 AT: ${SDL2_PATH}]")
    if(APPLE)
        set(SDL2_INCLUDE_DIR ${SDL2_PATH}/SDL2.framework/Headers)
        set(SDL2_LIB ${SDL2_PATH}/SDL2.framework)
    endif()
    
else()
    set(SDL2_PATH ${CMAKE_BINARY_DIR}/ext/sdl2 CACHE STRING "SDL2 lib/include root path")
    message("[DOWNLOADING SDL2...]")

    if(APPLE)
        file(DOWNLOAD
        https://www.libsdl.org/release/SDL2-2.0.9.dmg
        ${CMAKE_BINARY_DIR}/ext/temp.dmg
        SHOW_PROGRESS)

        execute_process(COMMAND
        hdiutil attach temp.dmg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        execute_process(COMMAND
        mkdir sdl2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        file(GLOB TEMP_FILES /Volumes/SDL2/*)
        file(COPY ${TEMP_FILES} DESTINATION ${CMAKE_BINARY_DIR}/ext/sdl2)

        execute_process(COMMAND
        hdiutil detach /Volumes/SDL2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        execute_process(COMMAND
        rm temp.dmg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        set(SDL2_INCLUDE_DIR ${SDL2_PATH}/SDL2.framework/Headers)
        set(SDL2_LIB ${SDL2_PATH}/SDL2.framework)


    elseif (WIN32)
        set(SDL_TARBALL_NAME SDL2-devel-${SDL2_VERSION}-mingw.tar.gz)

        if (MSVC)
          message(FATAL_ERROR "CMake not ready for Visual Studio Compiler")

        elseif (NOT MSVC)
            set(TEMP_GZ ${CMAKE_BINARY_DIR}/tmp/${SDL_TARBALL_NAME})

            if (NOT EXISTS ${TEMP_GZ})
 	          file(DOWNLOAD
                 http://libsdl.org/release/${SDL_TARBALL_NAME}
                       ${TEMP_GZ}
                       SHOW_PROGRESS)
            endif()
        endif()

        execute_process(COMMAND tar xfz ${CMAKE_BINARY_DIR}/tmp/${SDL_TARBALL_NAME})

        # Test 32 or 64 bits
        if ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            set(LIB_PATH x86_64-w64-mingw32)
        else()
            set(LIB_PATH i686-w64-mingw32)
        endif()

        set(SDL2_ROOT ${CMAKE_BINARY_DIR}/SDL2-${SDL2_VERSION}/${LIB_PATH})
        set(SDL2_INCLUDE_DIR ${SDL2_ROOT}/include/SDL2)
        set(SDL2_LIB ${SDL2_ROOT}/lib})

    endif()
endif()
