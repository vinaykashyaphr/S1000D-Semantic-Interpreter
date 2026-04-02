# include "builders_map/builders_map.hpp"
# include "builders/dmodule.hpp"
#include <string_view>



std::unordered_map<
    std::string_view, 
    std::function<void(const pugi::xml_node&, ModelsRegistry&)>> BUILDERS {

    {"dmodule", [](const pugi::xml_node& n, ModelsRegistry& r) -> void { _Dmodule(n, r, "descript"); }},
    // {"dmodule", [](const pugi::xml_node& n, ModelsRegistry& r) { _Dmodule(n, r); }},

};


