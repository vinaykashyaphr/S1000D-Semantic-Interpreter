# include "builders/dmodule.hpp"



_Dmodule::_Dmodule(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme): 
    _node(node),
    _registry(registry)

{

    
}



void _Dmodule::build() {

    Dmodule* dm = _registry.register_model(std::make_unique<Dmodule>());

    const auto& id = _node.attribute("id");

    if (!id.empty()) dm->id.emplace(id.value());

    dm->type = _node.name();

}


