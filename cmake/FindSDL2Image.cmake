
set(SDL2_IMAGE_VERSION 2.0.4)

if (SDL2_IMAGE_PATH)
    message("[USING SDL2_image AT: ${SDL2_IMAGE_PATH}]")

    if(APPLE)
        set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_IMAGE_PATH}/SDL2_image.framework/Headers)
        set(SDL2_IMAGE_LIB ${SDL2_IMAGE_PATH}/SDL2_image.framework)
    endif()
else()
    set(SDL2_IMAGE_PATH ${CMAKE_BINARY_DIR}/ext/sdl2_image CACHE STRING "SDL2_image lib/include root path")
    message("[DOWNLOADING SDL2_image...]")

    if(APPLE)
        file(DOWNLOAD
        https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.dmg
        ${CMAKE_BINARY_DIR}/ext/temp.dmg
        SHOW_PROGRESS)

        execute_process(COMMAND
        hdiutil attach temp.dmg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        execute_process(COMMAND
        mkdir sdl2_image
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        file(GLOB TEMP_FILES /Volumes/SDL2_image/*)
        file(COPY ${TEMP_FILES} DESTINATION ${CMAKE_BINARY_DIR}/ext/sdl2_image)

        execute_process(COMMAND
        hdiutil detach /Volumes/SDL2_image
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        execute_process(COMMAND
        rm temp.dmg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_IMAGE_PATH}/SDL2_image.framework/Headers)
        set(SDL2_IMAGE_LIB ${SDL2_IMAGE_PATH}/SDL2_image.framework)

    elseif (WIN32)
        set(IMAGE_TARBALL_NAME SDL2_image-devel-${SDL2_IMAGE_VERSION}-mingw.tar.gz)

        if (MSVC)
          message(FATAL_ERROR "CMake not ready for Visual Studio Compiler")

        elseif (NOT MSVC)
          set(TEMP_IMAGE_GZ ${CMAKE_BINARY_DIR}/tmp/${IMAGE_TARBALL_NAME})

          if (NOT EXISTS ${TEMP_IMAGE_GZ})
            file(DOWNLOAD
                 https://www.libsdl.org/projects/SDL_image/release/${IMAGE_TARBALL_NAME}
                 ${TEMP_IMAGE_GZ}
                 SHOW_PROGRESS)
          endif()

        endif()

        execute_process(COMMAND tar xfz ${TEMP_IMAGE_GZ})

        # Test 32 or 64 bits
        if ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
           set(IMAGE_LIB_PATH x86_64-w64-mingw32)
        else()
           set(IMAGE_LIB_PATH i686-w64-mingw32)
        endif()

        set(SDL2_IMAGE_ROOT ${CMAKE_BINARY_DIR}/SDL2_image-${SDL2_IMAGE_VERSION}/${IMAGE_LIB_PATH})
        set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_IMAGE_ROOT}/include/SDL2)
        set(SDL2_IMAGE_LIB ${SDL2_IMAGE_ROOT}/lib})

    endif()
  endif()

