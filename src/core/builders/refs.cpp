# include <memory>
# include <string>

# include "builders/refs.hpp"
# include "builders_map/builders_map.hpp"
# include "definitions/models.hpp"



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

    _registry.defer_link(

        [model = current_model, node = _node, &reg = _registry]() {

            Content* parent = reg.get_model<Content>(node.parent());
            if (!parent) throw std::runtime_error(std::string(node.name()) + ": expected parent \"content\" not found");
            parent->children.refs = model;

        }

    );

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



static BuilderRegistrar _reg(
    "refs", 
    [](const pugi::xml_node& n, ModelsRegistry& r, std::string_view s) 
        {
            _Refs(n, r, s);
        }
);


