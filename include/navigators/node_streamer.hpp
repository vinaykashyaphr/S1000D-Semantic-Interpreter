# pragma once

# include <unordered_map>
# include <string_view>
# include <utility>

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



class NodeStreamer {
    const pugi::xml_node& _node;
    ModelsRegistry& _registry;

    enum class Schema;
    static const std::unordered_map<std::string_view, std::pair<Schema, std::string_view>> SCHEMA_MAP;

    std::string_view get_schema_name();
    void unsupported_xml();

    public:
        NodeStreamer(const pugi::xml_node& node, ModelsRegistry& registry);
        void stream();

};


