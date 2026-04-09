add_library(
    utils
    ./generic.cpp
    ./s1000d.cpp
)


target_link_libraries(
    utils
    PRIVATE
    pugixml-static
)


target_include_directories(
    utils
    PUBLIC ${INCLUDE_DIR}
    PUBLIC ${DEPS_DIR}/pugixml/src
    PUBLIC ${DEPS_DIR}/nlohmann
)


