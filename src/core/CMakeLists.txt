add_subdirectory(builders)
add_subdirectory(navigators)
add_subdirectory(utils)
add_subdirectory(builders_map)


add_library(
    semantic_interpreter_core
    ./interpreter.cpp
)


target_include_directories(
    semantic_interpreter_core
    PRIVATE ${INCLUDE_DIR}
    PUBLIC ${LIB_INTERFACE}
    PUBLIC ${DEPS_DIR}/pugixml/src
    PUBLIC ${DEPS_DIR}/nlohmann
)


target_compile_options(
    semantic_interpreter_core
    PRIVATE -Wall -Wextra -Wconversion -Werror
)


target_link_libraries(
    semantic_interpreter_core
    PRIVATE
    pugixml-static
    $<LINK_LIBRARY:WHOLE_ARCHIVE,builders>
    navigators
    utils
    builders_map
)



