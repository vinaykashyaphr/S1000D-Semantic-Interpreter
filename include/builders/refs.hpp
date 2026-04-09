# pragma once

# include <string_view>

# include <pugixml.hpp>

# include "definitions/models.hpp"
# include "registries/models_registry.hpp"



class _Refs {

    const pugi::xml_node& _node;
    const std::string_view _scheme;

    ModelsRegistry& _registry;
    Refs* current_model;

    void build();
    void link();

    public:
        _Refs(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme);

};


