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
    endif()
endif()
