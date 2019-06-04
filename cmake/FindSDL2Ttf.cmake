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
        hdiutil attach -mountpoint temp temp.dmg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        execute_process(COMMAND 
        mkdir sdl2_ttf
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        file(GLOB TEMP_FILES ${CMAKE_BINARY_DIR}/ext/temp/*)
        file(COPY ${TEMP_FILES} DESTINATION ${CMAKE_BINARY_DIR}/ext/sdl2_ttf)
        
        execute_process(COMMAND 
        hdiutil detach temp
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)

        execute_process(COMMAND 
        rm temp.dmg
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/ext/)
        
        set(SDL2_TTF_INCLUDE_DIR ${SDL2_TTF_PATH}/SDL2_ttf.framework/Headers) 
        set(SDL2_TTF_LIB ${SDL2_TTF_PATH}/SDL2_ttf.framework) 
    endif()
endif()
