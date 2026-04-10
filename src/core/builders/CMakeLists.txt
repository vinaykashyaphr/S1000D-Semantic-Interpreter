file(GLOB_RECURSE BUILDER_SOURCES CONFIGURE_DEPENDS "*.cpp")


add_library(builders ${BUILDER_SOURCES})



target_link_libraries(
    builders
    PRIVATE pugixml-static
)



target_include_directories(
    builders
    PUBLIC ${LIB_INTERFACE}
    PRIVATE ${INCLUDE_DIR}
    PRIVATE ${DEPS_DIR}/pugixml/src
    PRIVATE ${DEPS_DIR}/nlohmann
)

