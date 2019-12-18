
set(SDL2_TTF_VERSION 2.0.15)

if (SDL2_TTF_PATH)
    message("[USING SDL2_ttf AT: ${SDL2_TTF_PATH}]")

    if(APPLE)
      set(SDL2_TTF_INCLUDE_DIR ${SDL2_TTF_PATH}/SDL2_ttf.framework/Headers)
        set(SDL2_TTF_LIB ${SDL2_TTF_PATH}/SDL2_ttf.framework)
    endif()

else()
    set(SDL2_TTF_PATH ${CMAKE_BINARY_DIR}/ext/sdl2_ttf CACHE STRING "SDL2_ttf lib/include root path")
    message("[DOWNLOADING SDL2_ttf...]")

    if(APPLE)
        file(DOWNLOAD
        https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-2.0.15.dmg
        ${CMAKE_BINARY_DIR}/ext/temp.dmg
        SHOW_PROGRESS)

        execute_process(COMMAND
        hdiutil attach temp.dmg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        execute_process(COMMAND
        mkdir sdl2_ttf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        file(GLOB TEMP_FILES /Volumes/SDL2_ttf/*)
        file(COPY ${TEMP_FILES} DESTINATION ${CMAKE_BINARY_DIR}/ext/sdl2_ttf)

        execute_process(COMMAND
        hdiutil detach /Volumes/SDL2_ttf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        execute_process(COMMAND
        rm temp.dmg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        set(SDL2_TTF_INCLUDE_DIR ${SDL2_TTF_PATH}/SDL2_ttf.framework/Headers)
        set(SDL2_TTF_LIB ${SDL2_TTF_PATH}/SDL2_ttf.framework)

    elseif (WIN32)
        set(TTF_TARBALL_NAME SDL2_ttf-devel-${SDL2_TTF_VERSION}-mingw.tar.gz)

        if (MSVC)
            message(FATAL_ERROR "CMake not ready for Visual Studio Compiler")

        elseif(NOT MSVC)
            set(TEMP_TTF_GZ ${CMAKE_BINARY_DIR}/tmp/${TTF_TARBALL_NAME})

            if (NOT EXISTS ${TEMP_TTF_GZ})

                file(DOWNLOAD
                http://libsdl.org/projects/SDL_ttf/release/${TEMP_TTF_GZ}
                ${TEMP_TTF_GZ}
                SHOW_PROGRESS)
            endif()
        endif()

        execute_process(COMMAND tar xfz ${CMAKE_BINARY_DIR}/tmp/${TTF_TARBALL_NAME})

        # Test 32 or 64 bits
        if ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
            set(TTF_LIB_PATH x86_64-w64-mingw32)
        else()
            set(TTF_LIB_PATH i686-w64-mingw32)
        endif()

        set(SDL2_TTF_ROOT ${CMAKE_BINARY_DIR}/SDL2_ttf-${SDL2_TTF_VERSION}/${TTF_LIB_PATH})
        set(SDL2_TTF_INCLUDE_DIR ${SDL2_TTF_ROOT}/include/SDL2)
        set(SDL2_TTF_LIB ${SDL2_TTF_ROOT}/lib})

    endif()

endif()

