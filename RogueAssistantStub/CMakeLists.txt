cmake_minimum_required(VERSION 3.31)
project(RogueAssistantStub LANGUAGES CXX)

add_executable(RogueAssistantStub)

target_sources(RogueAssistantStub
    PRIVATE
        RogueAssistantStub.cpp
        RogueAssistantStub.rc
)

set_target_properties(RogueAssistantStub PROPERTIES
    OUTPUT_NAME RogueAssistant
)

target_compile_definitions(RogueAssistantStub
    PRIVATE
        _CONSOLE
        UNICODE
        _UNICODE
        "$<$<CONFIG:Debug>:_DEBUG>"
        "$<$<CONFIG:Release>:NDEBUG>"
        "$<$<STREQUAL:${CMAKE_CXX_COMPILER_ARCHITECTURE_ID},X86>:WIN32>"
)

target_link_directories(RogueAssistantStub
    PRIVATE
        "${OutDir}"
)

target_link_libraries(RogueAssistantStub
    PRIVATE
        RogueAssistantCpp
)

target_compile_options(RogueAssistantStub
    PRIVATE
        /W3
)

