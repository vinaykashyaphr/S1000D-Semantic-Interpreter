file(GLOB_RECURSE BUILDER_SOURCES "*.cpp")


add_library(builders ${BUILDER_SOURCES})



target_link_libraries(
    builders
    PRIVATE pugixml-static
)



target_include_directories(
    builders
    PUBLIC ${LIB_INTERFACE}
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${DEPS_DIR}/pugixml/src
    PUBLIC ${DEPS_DIR}/nlohmann
)

