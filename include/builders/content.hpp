# pragma once

# include <string_view>
# include <unordered_map>

# include <pugixml.hpp>

# include "definitions/models.hpp"
# include "registries/models_registry.hpp"



class _Content {

    const pugi::xml_node& _node;
    const std::string_view _scheme;

    ModelsRegistry& _registry;
    Content* current_model;

    enum class Attrib;
    static const std::unordered_map<std::string_view, Attrib> ATTRIBS;

    void build();
    void resolve();
    void link();

    public:
        _Content(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme);

};


