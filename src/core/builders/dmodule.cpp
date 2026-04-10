# include <utility>

# include "builders/dmodule.hpp"
# include "builders_map/builders_map.hpp"
# include "utils/generic.hpp"




static BuilderRegistrar _reg(
    "dmodule", 
    [](const pugi::xml_node& n, ModelsRegistry& r, std::string_view s) 

        {
            _Dmodule(n, r, s);
        }

);



_Dmodule::_Dmodule(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme): 
    _node(node),
    _registry(registry),
    _scheme(scheme)

{

    current_model = _registry.register_model(_registry.factory().make<Dmodule>(), _node);
    build();

}



enum class _Dmodule::Attrib {

    xmlns,
    xmlns_dc,
    xmlns_rdf,
    xmlns_xlink,
    xmlns_xsi,
    xsi_nil,
    xsi_noNamespaceSchemaLocation,
    xsi_schemaLocation,
    id

};



const std::unordered_map<std::string_view, _Dmodule::Attrib> _Dmodule::ATTRIBS {

    {"xmlns", Attrib::xmlns},
    {"xmlns:dc", Attrib::xmlns_dc},
    {"xmlns:rdf", Attrib::xmlns_rdf},
    {"xmlns:xlink", Attrib::xmlns_xlink},
    {"xmlns:xsi", Attrib::xmlns_xsi},
    {"xsi:nil", Attrib::xsi_nil},
    {"xsi:noNamespaceSchemaLocation", Attrib::xsi_noNamespaceSchemaLocation},
    {"xsi:schemaLocation", Attrib::xsi_schemaLocation},
    {"id", Attrib::id}

};



void _Dmodule::build() {
    current_model->type = _node.name();
    resolve();
}



void _Dmodule::resolve() {

    for (const auto& attrib : _node.attributes()) {

        const Attrib _attrib = generic_utils::find_in_umap(ATTRIBS, attrib.name(), "Dmodule");

        switch (_attrib) {

            case Attrib::id:
                current_model->id.emplace(attrib.value());
                break;

            case Attrib::xmlns:
                [[fallthrough]];

            case Attrib::xmlns_dc:
                [[fallthrough]];

            case Attrib::xmlns_rdf:
                [[fallthrough]];

            case Attrib::xmlns_xlink:
                [[fallthrough]];

            case Attrib::xmlns_xsi:
                [[fallthrough]];

            case Attrib::xsi_nil:
                [[fallthrough]];

            case Attrib::xsi_noNamespaceSchemaLocation:
                [[fallthrough]];

            case Attrib::xsi_schemaLocation:
                break;

            default:
                throw std::runtime_error("Invalid attribute: " + std::string(attrib.name()) + " for " + std::string(_node.name()));
                break;

        }

    }

}



nlohmann::json Dmodule::DmoduleChildren::to_json() const {

    nlohmann::json j = nlohmann::json::array();

    if (rdf__description) j.push_back((*rdf__description)->to_json());
    if (ident_and_status_section) j.push_back(ident_and_status_section->to_json());
    if (content) j.push_back(content->to_json());

    return j;

}



nlohmann::json Dmodule::to_json() const {

    nlohmann::json j = {{"type", type}};

    /// @b attributes
    j["id"] = id.has_value() ? nlohmann::json(id.value()) : nlohmann::json(nullptr);

    /// @b children
    if (auto children_json = children.to_json(); !children_json.empty())
        j["children"] = std::move(children_json);

    return j;

}


