# include <iostream>
# include <string_view>

# include "navigators/tree_traversor.hpp"
# include "navigators/node_builder.hpp"


Traversor::Traversor(pugi::xml_node node, ModelsRegistry& registry, const std::string_view scheme): 
    _node(node),
    _registry(registry),
    _scheme(scheme)
{
    traverse();
}



struct Traversor::Visitor : public pugi::xml_tree_walker {

    NodeBuilder builder;

    Visitor(ModelsRegistry& r, const std::string_view s) : builder(r, s) {}

    bool for_each(pugi::xml_node& node) {

        using nt = pugi::xml_node_type;

        if (node.type() == nt::node_element) {
            builder.build(node);
        }

        return true;

    }

};



void Traversor::traverse() {

    std::cout << _node.name() << std::endl;

    Visitor visitor(_registry, _scheme);

    // traversing root node explicitly
    visitor.builder.build(_node);

    // traversing descendents of root
    _node.traverse(visitor);

    // linking model to relations after models built
    _registry.resolve_links();

}