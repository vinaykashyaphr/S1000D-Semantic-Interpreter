
add_library(
    builders_map
    ./builders_map.cpp
)



target_include_directories(
    builders_map
    PRIVATE ${INCLUDE_DIR}
    PUBLIC ${LIB_INTERFACE}
    PRIVATE ${DEPS_DIR}/pugixml/src
    PRIVATE ${DEPS_DIR}/nlohmann
)



target_link_libraries(
    builders_map
    PRIVATE pugixml-static
)

