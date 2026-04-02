# pragma once

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



class _Dmodule {

    const pugi::xml_node& _node;
    ModelsRegistry& _registry;

    void build();

    public:
        _Dmodule(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme);

};


