# include <iostream>

# include "navigators/node_streamer.hpp"
# include "navigators/tree_traversor.hpp"
# include "utils/generic.hpp"



enum class NodeStreamer::Schema {

    appliccrossreftable,
    brdoc,
    brex,
    checklist,
    comment,
    comrep,
    condcrossreftable,
    container,
    crew,
    ddn,
    descript,
    dml,
    fault,
    frontmatter,
    icnmetadata,
    ipd,
    learning,
    pm,
    prdcrossreftable,
    proced,
    process,
    sb,
    schedul,
    scocontent,
    scromcontentpackage,
    update,
    wrngdata,
    wrngflds

};



const std::unordered_map<
    std::string_view, 
    std::pair<NodeStreamer::Schema, std::string_view>
> NodeStreamer::SCHEMA_MAP = {

    {"appliccrossreftable", {Schema::appliccrossreftable, "dmodule"}},
    {"brdoc", {Schema::brdoc, "dmodule"}},
    {"brex", {Schema::brex, "dmodule"}},
    {"checklist", {Schema::checklist, "dmodule"}},
    {"comment", {Schema::comment, "comment"}},
    {"comrep", {Schema::comrep, "dmodule"}},
    {"condcrossreftable", {Schema::condcrossreftable, "dmodule"}},
    {"container", {Schema::container, "dmodule"}},
    {"crew", {Schema::crew, "dmodule"}},
    {"ddn", {Schema::ddn, "ddn"}},
    {"descript", {Schema::descript, "dmodule"}},
    {"dml", {Schema::dml, "dml"}},
    {"fault", {Schema::fault, "dmodule"}},
    {"frontmatter", {Schema::frontmatter, "dmodule"}},
    {"icnmetadata", {Schema::icnmetadata, "icnMetadataFile"}},
    {"ipd", {Schema::ipd, "dmodule"}},
    {"learning", {Schema::learning, "dmodule"}},
    {"pm", {Schema::pm, "pm"}},
    {"prdcrossreftable", {Schema::prdcrossreftable, "dmodule"}},
    {"proced", {Schema::proced, "dmodule"}},
    {"process", {Schema::process, "dmodule"}},
    {"sb", {Schema::sb, "dmodule"}},
    {"schedul", {Schema::schedul, "dmodule"}},
    {"scocontent", {Schema::scocontent, "dmodule"}},
    {"scromcontentpackage", {Schema::scromcontentpackage, "scormContentPackage"}},
    {"update", {Schema::update, "dataUpdateFile"}},
    {"wrngdata", {Schema::wrngdata, "dmodule"}},
    {"wrngflds", {Schema::wrngflds, "dmodule"}}

};



NodeStreamer::NodeStreamer(const pugi::xml_node& node, ModelsRegistry& registry) : 
    _node(node), 
    _registry(registry) {};



void NodeStreamer::unsupported_xml() {
    throw std::runtime_error("Unsupported XML type!\n");
};



std::string_view NodeStreamer::get_schema_name() {

    // TODO: also consider xsi:schemaLocation
    const pugi::xml_attribute schema_loc = _node.attribute("xsi:noNamespaceSchemaLocation");
    if (!schema_loc) unsupported_xml();

    const std::string_view schema_url = schema_loc.value();

    auto pos = schema_url.find_last_of('/');
    if (pos == std::string_view::npos) unsupported_xml();

    const std::string_view filename = schema_url.substr(pos + 1);
    const std::string_view suffix = ".xsd";
    if (filename.empty() || !filename.ends_with(suffix)) unsupported_xml();

    const std::string_view schema_name = filename.substr(0, filename.size() - suffix.size());

    return schema_name;

}



void NodeStreamer::stream() {

    auto unsuppored_xml = []() -> void {
        throw std::runtime_error("Unsupported XML type!\n");
    };

    const std::string_view schema_name = get_schema_name();
    const auto& [schema, root_name] = generic_utils::find_in_umap(SCHEMA_MAP, std::string(schema_name), "Schema");

    switch (schema) {

        case Schema::descript: {

            if (_node.name() != root_name) unsuppored_xml();
            Traversor traversor(_node, _registry);
            break;
        }

        default:
            std::cerr << "Schema is not implemented: " << schema_name << '\n';
            break;

    }

}






