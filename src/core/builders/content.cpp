# include "builders/content.hpp"
#include "models.hpp"
# include "utils/generic.hpp"
#include <iostream>


_Content::_Content(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme):
    _node(node),
    _registry(registry),
    _scheme(scheme)
{
    std::cout << "content constructor" << '\n';
    current_model = _registry.register_model(std::make_unique<Content>(), _node);
    build();
    link_to_dmodule();
}



enum class _Content::Attrib {

    id

};



const std::unordered_map<std::string_view, _Content::Attrib> _Content::ATTRIBS {

    {"id", Attrib::id}

};



void _Content::build() {
    current_model->type = _node.name();
    resolve_attribs();
}



void _Content::resolve_attribs() {

    for (const auto& attrib : _node.attributes()) {

        const Attrib _attrib = generic_utils::find_in_umap(ATTRIBS, attrib.name(), "Content");

        switch (_attrib) {

            case Attrib::id:
                current_model->id.emplace(attrib.value());
                break;

            default:
                std::runtime_error("Invalid attribute: " + std::string(attrib.name()) + " for " + std::string(_node.name()));
                break;

        }

    }
}



void _Content::link_to_dmodule() {

    pugi::xml_node parent_node = _node.parent();
    BaseModel* parent = _registry.get_by_node(parent_node);
    
    if (auto* dmodule = dynamic_cast<Dmodule*>(parent)) {
        dmodule->children.content = current_model;
    }

}


