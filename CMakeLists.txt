cmake_minimum_required(VERSION 3.16)
project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMAND ON)

add_subdirectory(Engine)

file(GLOB_RECURSE USER_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Games/*.cpp")

file(GLOB_RECURSE TOOLS
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/*.cpp")

file(GLOB_RECURSE GAME_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/GameFiles/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/GameFiles/*.h")

foreach(GAME_EXE ${USER_SOURCES})
    get_filename_component(GAME_NAME ${GAME_EXE} NAME_WE)

    add_executable(${GAME_NAME} ${GAME_EXE})

    target_include_directories(${GAME_NAME} PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/GameFiles"
    )
    
    target_link_libraries(${GAME_NAME} PRIVATE Engine)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${GAME_NAME} PRIVATE
            -fsanitize=address 
            -fno-omit-frame-pointer
            -g
            -O0
        )
        target_link_options(${GAME_NAME} PRIVATE
            -fsanitize=address
        )
    endif()
endforeach()

foreach(TOOL_FILE ${TOOLS})
    get_filename_component(TOOL_NAME ${TOOL_FILE} NAME_WE)

    add_executable(${TOOL_NAME} ${TOOL_FILE})

    target_link_libraries(${TOOL_NAME} PRIVATE Engine)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${GAME_NAME} PRIVATE
            -fsanitize=address 
            -fno-omit-frame-pointer
            -g
            -O0
        )
        target_link_options(${GAME_NAME} PRIVATE
            -fsanitize=address
        )
    endif()
endforeach()
