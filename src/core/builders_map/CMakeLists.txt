
add_library(
    builders_map
    ./builders_map.cpp
)



target_include_directories(
    builders_map
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${DEPS_DIR}/pugixml/src
)



target_link_libraries(
    builders_map
    PRIVATE pugixml-static
    PRIVATE builders
)

