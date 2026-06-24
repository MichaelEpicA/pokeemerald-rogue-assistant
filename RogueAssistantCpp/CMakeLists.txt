cmake_minimum_required(VERSION 3.28)
project(RogueAssistantCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(SFML)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

find_package(Freetype REQUIRED)
find_package(glfw3 REQUIRED)
pkg_check_modules(LUA REQUIRED lua5.4)
pkg_check_modules(ENET REQUIRED libenet)

add_library(RogueAssistantCpp SHARED)

target_include_directories(RogueAssistantCpp
    PRIVATE
        ${sfml_SOURCE_DIR}/include
        ${LUA_INCLUDE_DIRS}
        ${ENET_INCLUDE_DIRS}
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

target_link_libraries(RogueAssistantCpp
    PRIVATE
    OpenGL::GL
    glfw
    Freetype::Freetype

    -Wl,--whole-archive
    ${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/libsfml-graphics.a
    ${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/libsfml-window.a
    ${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/libsfml-system.a
    ${CMAKE_BINARY_DIR}/_deps/sfml-build/lib/libsfml-network.a
    -Wl,--no-whole-archive

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


target_compile_options(RogueAssistantCpp
    PRIVATE
        -Wall
)
