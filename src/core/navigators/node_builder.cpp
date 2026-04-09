# include <iostream>

# include "navigators/node_builder.hpp"
# include "builders_map/builders_map.hpp"



NodeBuilder::NodeBuilder(ModelsRegistry& registry, const std::string_view scheme):
    _registry(registry),
    _scheme(scheme)
{}



void NodeBuilder::build(const pugi::xml_node& node) {

    const BuilderFn* fn = BuildersMap::instance().find(node.name());

    if (fn) (*fn)(node, _registry, _scheme);
    else    std::cout << node.name() << " is not implemented!" << '\n';

}

