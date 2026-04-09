
add_library(
    builders_map
    ./builders_map.cpp
)



target_include_directories(
    builders_map
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${LIB_INTERFACE}
    PUBLIC ${DEPS_DIR}/pugixml/src
    PUBLIC ${DEPS_DIR}/nlohmann
)



target_link_libraries(
    builders_map
    PRIVATE pugixml-static
    PRIVATE builders
)

