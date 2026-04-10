
add_library(
    registries
    ./models_registry.cpp
)



target_include_directories(
    registries
    PRIVATE ${INCLUDE_DIR}
    PUBLIC ${LIB_INTERFACE}
    PRIVATE ${DEPS_DIR}/pugixml/src
    PRIVATE ${DEPS_DIR}/nlohmann
)



target_link_libraries(
    registries
    PRIVATE pugixml-static
)

