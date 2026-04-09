# include "builders/refs.hpp"
# include "definitions/models.hpp"

# include <memory>


_Refs::_Refs(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme):
    _node(node),
    _registry(registry),
    _scheme(scheme)
{
    current_model = _registry.register_model(std::make_unique<Refs>(), _node);
    build();
}



void _Refs::build() {

    current_model->type = _node.name();
    link();

}



void _Refs::link() {

    Content* parent = _registry.get_model<Content>(_node.parent());

    if (auto* content = dynamic_cast<Content*>(parent)){
        content->children.refs = current_model;
    }

}



nlohmann::json Refs::RefsChildren::to_json() const {

    nlohmann::json j = nlohmann::json::array();

    if (dm_ref) j.push_back("dm_ref");

    return j;
}



nlohmann::json Refs::to_json() const {

    nlohmann::json j = {{"type", type}};

    /// @b children
    nlohmann::json children_list = nlohmann::json::array();

    for (const auto& child : children) {

        if (auto child_json = child.to_json(); !child_json.empty())
            children_list.push_back(std::move(child_json));

    }

    if (!children_list.empty()) j["children"] = std::move(children_list);

    return j;

}



