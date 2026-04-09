add_library(
    navigators
    ./node_builder.cpp
    ./node_streamer.cpp
    ./tree_traversor.cpp
)



target_include_directories(
    navigators
    PUBLIC ${LIB_INTERFACE}
    PRIVATE ${INCLUDE_DIR}
    PRIVATE ${DEPS_DIR}/pugixml/src
    PRIVATE ${DEPS_DIR}/nlohmann
)



target_link_libraries(
    navigators
    PRIVATE pugixml-static
    PRIVATE builders_map
)


