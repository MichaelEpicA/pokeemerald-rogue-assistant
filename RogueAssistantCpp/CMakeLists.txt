cmake_minimum_required(VERSION 3.28)
project(RogueAssistantCpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)
if(WIN32)
add_custom_target(
EmbeddedFiles
COMMENT "Embedding files..."
COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/External/bin2cpp/bin/bin2cpp.exe --dir=${CMAKE_CURRENT_SOURCE_DIR}/Assets --output=${CMAKE_CURRENT_SOURCE_DIR}/Embedded
)
elseif(LINUX)
add_custom_target(
EmbeddedFiles
COMMENT "Embedding files..."
COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/External/bin2cpp/bin/bin2cpp --dir=${CMAKE_CURRENT_SOURCE_DIR}/Assets --output=${CMAKE_CURRENT_SOURCE_DIR}/Embedded
)
endif()
    include(FetchContent)
    #[[
    FetchContent_Declare(bin2cpp_cli
    GIT_REPOSITORY https://github.com/end2endzone/bin2cpp.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
    FetchContent_MakeAvailable(bin2cpp_cli)
    add_custom_target(bin2cpp_cli
        COMMAND ${CMAKE_COMMAND} -E make_directory "${bin2cpp_cli_BINARY_DIR}"
        COMMAND ${CMAKE_COMMAND} "${bin2cpp_cli_SOURCE_DIR}" -B "${bin2cpp_BINARY_DIR}" -DCMAKE_BUILD_TYPE=Release
        COMMAND ${CMAKE_COMMAND} --build "${bin2cpp_cli_BINARY_DIR}"
        COMMENT "Building bin2cpp..."
    ) 
    #]]

if(LINUX)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
    FetchContent_MakeAvailable(SFML)

    find_package(OpenGL REQUIRED)
    find_package(PkgConfig REQUIRED)

    find_package(Freetype REQUIRED)
    find_package(glfw3 REQUIRED)
    pkg_check_modules(LUA REQUIRED lua5.4)
    pkg_check_modules(ENET REQUIRED libenet)
endif()

add_library(RogueAssistantCpp SHARED)

add_dependencies(RogueAssistantCpp EmbeddedFiles)

target_include_directories(RogueAssistantCpp
    PRIVATE
        if(LINUX)
        ${sfml_SOURCE_DIR}/include
        ${LUA_INCLUDE_DIRS}
        ${ENET_INCLUDE_DIRS}
        endif()
        "${CMAKE_CURRENT_SOURCE_DIR}/Inc"
        "${CMAKE_CURRENT_SOURCE_DIR}/External/glfw-3.3.8/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui"
        "${CMAKE_CURRENT_SOURCE_DIR}/External/ENet/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/External/SFML/Win64/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/External/Lua/Win64/include"
)

target_sources(RogueAssistantCpp
    PRIVATE
        Src/UI/PrimaryUI.cpp
        Src/UI/Window.cpp
        External/imgui/imgui_demo.cpp
        External/imgui/imgui_draw.cpp
        External/imgui/imgui_impl_glfw.cpp
        External/imgui/imgui_impl_opengl2.cpp
        External/imgui/imgui_tables.cpp
        External/imgui/imgui_widgets.cpp
        External/imgui/imgui.cpp
        Src/Assets.cpp
        Src/Behaviours/CommonBehaviour.cpp
        Src/Behaviours/HomeBoxBehaviour.cpp
        Src/Behaviours/MultiplayerBehaviour.cpp
        Src/DataStream.cpp
        Src/GameConnection.cpp
        Src/GameConnectionManager.cpp
        Src/GameConnectionRPCs.cpp
        Src/GameData.cpp
        Src/LuaMain.cpp
        Src/Main.cpp
        Src/ObservedGameMemory.cpp
        Src/Timer.cpp
        Src/UserData.cpp
)

set_target_properties(RogueAssistantCpp PROPERTIES
    OUTPUT_NAME RogueAssistant
)

target_compile_features(RogueAssistantCpp
    PRIVATE
        cxx_std_20
)


target_compile_definitions(RogueAssistantCpp
    PRIVATE
        SFML_STATIC
        _CONSOLE
        UNICODE
        _UNICODE
        "$<$<CONFIG:Debug>:_DEBUG>"
        "$<$<CONFIG:Release>:NDEBUG>"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>"
)


target_link_directories(RogueAssistantCpp
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/External/ENet"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:${CMAKE_CURRENT_SOURCE_DIR}/External/SFML/Win32/lib>"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:${CMAKE_CURRENT_SOURCE_DIR}/External/Lua/Win32>"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:${CMAKE_CURRENT_SOURCE_DIR}/External/glfw-3.3.8/lib/Win32>"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},x64>:${CMAKE_CURRENT_SOURCE_DIR}/External/SFML/Win64/lib>"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},x64>:${CMAKE_CURRENT_SOURCE_DIR}/External/Lua/Win64>"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},x64>:${CMAKE_CURRENT_SOURCE_DIR}/External/glfw-3.3.8/lib/Win64>"
)
if(WIN32)
    target_link_libraries(RogueAssistantCpp
        PRIVATE
        opengl32.lib
        lua54.lib
        freetype.lib
        winmm.lib
        gdi32.lib
        glfw3.lib
        ws2_32.lib
        "$<$<CONFIG:Debug>:sfml-system-s-d.lib>"
        "$<$<CONFIG:Debug>:sfml-network-s-d.lib>"
        "$<$<CONFIG:Debug>:sfml-window-s-d.lib>"
        "$<$<CONFIG:Debug>:sfml-graphics-s-d.lib>"
        "$<$<CONFIG:Release>:sfml-system-s.lib>"
        "$<$<CONFIG:Release>:sfml-network-s.lib>"
        "$<$<CONFIG:Release>:sfml-window-s.lib>"
        "$<$<CONFIG:Release>:sfml-graphics-s.lib>"
       "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:enet.lib>"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},x64>:enet64.lib>"
    )
endif()


if(LINUX)
    target_link_libraries(RogueAssistantCpp
            PRIVATE
            OpenGL::GL
            glfw
            Freetype::Freetype

            sfml-graphics
            sfml-window
            sfml-system
            sfml-network

            ${LUA_LIBRARIES}
            ${ENET_LIBRARIES}

            mbedtls
            mbedx509
            mbedcrypto
            udev
            ssh2
            Xrandr
            Xcursor
    )
endif()

target_compile_options(RogueAssistantCpp
    PRIVATE
        -Wall
)