# pragma once

# include <string_view>

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



class Traversor {

    pugi::xml_node _node;
    ModelsRegistry& _registry;
    const std::string_view _scheme;

    struct Visitor;

    void traverse();
    
    public:
        Traversor(pugi::xml_node node, ModelsRegistry& registry, const std::string_view scheme);

};


