# pragma once

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



class NodeBuilder {
    ModelsRegistry& _registry;

    public:
        NodeBuilder(ModelsRegistry& registry);
        void build(const pugi::xml_node& node);

};


