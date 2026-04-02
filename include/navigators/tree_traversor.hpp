# pragma once

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



class Traversor {

    pugi::xml_node _node;
    ModelsRegistry& _registry;
    struct Visitor;

    void traverse();
    
    public:
        Traversor(pugi::xml_node node, ModelsRegistry& registry);

};


