# pragma once

# include <string_view>

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



class NodeBuilder {
    ModelsRegistry& _registry;
    const std::string_view _scheme;

    public:
        NodeBuilder(ModelsRegistry& registry, const std::string_view scheme);
        void build(const pugi::xml_node& node);

};


