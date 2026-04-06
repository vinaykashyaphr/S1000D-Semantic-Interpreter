# include <string_view>

# include "builders_map/builders_map.hpp"

# include "builders/__elements__.hpp"



std::unordered_map<
    std::string_view, 
    std::function<void(const pugi::xml_node&, ModelsRegistry&, const std::string_view)>> BUILDERS {

    {"dmodule", [](const pugi::xml_node& n, ModelsRegistry& r, const std::string_view s) -> void 

        { 
            _Dmodule(n, r, s);
        }

    },

    {"content", [](const pugi::xml_node& n, ModelsRegistry& r, const std::string_view s)-> void 
        { 
            _Content(n, r, s); 
        }
    },

};


