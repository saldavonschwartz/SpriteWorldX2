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
    endif()
endif()
