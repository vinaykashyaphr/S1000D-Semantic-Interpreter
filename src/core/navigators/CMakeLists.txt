add_library(
    navigators
    ./node_builder.cpp
    ./node_streamer.cpp
    ./tree_traversor.cpp
)



target_include_directories(
    navigators
    PUBLIC ${LIB_INTERFACE}
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${DEPS_DIR}/pugixml/src
    PUBLIC ${DEPS_DIR}/nlohmann
)



target_link_libraries(
    navigators
    PRIVATE pugixml-static
    PRIVATE builders_map
)


