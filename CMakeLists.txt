cmake_minimum_required(VERSION 3.20)


# SETUP

project(
    semantic_interpreter 
    VERSION 1.0
    DESCRIPTION "Semantic interpreter and structural transformer for S1000D XMLs"
    LANGUAGES CXX
    )


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)
set(LIB_INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/interface)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Dependencies

# External
add_subdirectory(${DEPS_DIR}/pugixml)

# Internal
add_subdirectory(${SRC_DIR}/core)



# Executable

add_executable(
    semantic_interpreter
    ${SRC_DIR}/main.cpp
)


target_include_directories(
    semantic_interpreter
    PRIVATE ${INCLUDE_DIR}
    PUBLIC ${LIB_INTERFACE}
)


target_link_libraries(
    semantic_interpreter
    PRIVATE semantic_interpreter_core
)

