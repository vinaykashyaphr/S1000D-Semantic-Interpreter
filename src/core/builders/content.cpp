# include <memory>
# include <string>

# include "builders/content.hpp"
# include "builders_map/builders_map.hpp"
# include "utils/generic.hpp"




static BuilderRegistrar _reg(
    "content", 
    [](const pugi::xml_node& n, ModelsRegistry& r, std::string_view s) 
    {
        _Content(n, r, s);
    }
);



_Content::_Content(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme):
    _node(node),
    _registry(registry),
    _scheme(scheme)
{
    current_model = _registry.register_model(std::make_unique<Content>(), _node);
    build();
}



enum class _Content::Attrib {

    id

};



const std::unordered_map<std::string_view, _Content::Attrib> _Content::ATTRIBS {

    {"id", Attrib::id}

};



void _Content::build() {
    current_model->type = _node.name();
    resolve();
    link();
}



void _Content::resolve() {

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



void _Content::link() {

    _registry.defer_link(

        [model = current_model, node = _node, &reg = _registry]() {

            Dmodule* parent = reg.get_model<Dmodule>(node.parent());
            if (!parent) throw std::runtime_error(std::string(node.name()) + ": expected parent \"dmodule\" not found");
            parent->children.content = model;

        }

    );

}



nlohmann::json Content::ContentChildren::to_json() const {

    nlohmann::json j = nlohmann::json::array();

    if (refs) j.push_back((*refs)->to_json());
    // j.push_back(ident_and_status_section->to_json());
    // j.push_back(content->to_json());
    return j;

};



nlohmann::json Content::to_json() const {

    nlohmann::json j = {{"type", type}};

    /// @b attributes
    j["id"] = id.has_value() ? nlohmann::json(id.value()) : nlohmann::json(nullptr);

    /// @b children
    if (auto children_json = children.to_json(); !children_json.empty())
        j["children"] = std::move(children_json);

    return j;

}


