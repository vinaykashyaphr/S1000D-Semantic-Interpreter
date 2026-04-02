# include <iostream>

# include "navigators/tree_traversor.hpp"
# include "navigators/node_builder.hpp"


Traversor::Traversor(pugi::xml_node node, ModelsRegistry& registry): 
    _node(node),
    _registry(registry)
{
    traverse();
}



struct Traversor::Visitor : public pugi::xml_tree_walker {

    NodeBuilder builder;

    Visitor(ModelsRegistry& r) : builder(r) {}

    bool for_each(pugi::xml_node& node) {

        using nt = pugi::xml_node_type;

        if (node.type() == nt::node_element) {
            builder.build(node);
        }

        return true;

    }

};



void Traversor::traverse() {

    // traverse root node explicitly
    std::cout << _node.name() << std::endl;

    Visitor visitor(_registry);
    visitor.builder.build(_node);
    _node.traverse(visitor);

}