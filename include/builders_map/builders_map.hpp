# pragma  once

# include <unordered_map>
# include <string_view>
# include <functional>

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



extern std::unordered_map<
    std::string_view, 
    std::function<void(const pugi::xml_node&, ModelsRegistry&, const std::string_view)>
> BUILDERS;


