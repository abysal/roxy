cmake_minimum_required(VERSION 3.31)
project(roxy)

set(CPM_SOURCE_CACHE ${PROJECT_SOURCE_DIR}/cpm-cache)
include(cmake/CPM.cmake)

set(CMAKE_CXX_STANDARD 26)
CPMAddPackage(
        NAME lua
        GIT_REPOSITORY https://github.com/lua/lua.git
        VERSION 5.4.8
        DOWNLOAD_ONLY YES
)

if (lua_ADDED)
    FILE(GLOB lua_sources ${lua_SOURCE_DIR}/*.c)
    list(REMOVE_ITEM lua_sources "${lua_SOURCE_DIR}/lua.c" "${lua_SOURCE_DIR}/luac.c")
    add_library(lua STATIC ${lua_sources})

    target_include_directories(lua
            PUBLIC
            $<BUILD_INTERFACE:${lua_SOURCE_DIR}>
    )
endif ()

set(REFLECTCPP_JSON OFF CACHE BOOL "Enable JSON support")
set(REFLECTCPP_USE_STD_EXPECTED ON CACHE BOOL "Use std::expected")
set(REFLECT_CPP_C_ARRAYS_OR_INHERITANCE ON CACHE BOOL "Enable C arrays or inheritance")


CPMAddPackage("gh:ThePhD/sol2#c1f95a773c6f8f4fde8ca3efe872e7286afe4444")
CPMAddPackage("gh:getml/reflect-cpp#a428035be1802d34c934f4f46075cb7f40f8df81")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "compiler/*.cpp" "compiler/*.hpp")
add_executable(roxy ${SOURCES})
target_link_libraries(roxy PUBLIC
        sol2 lua reflectcpp
)
target_include_directories(roxy PUBLIC compiler)