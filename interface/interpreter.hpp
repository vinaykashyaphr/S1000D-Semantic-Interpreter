# pragma once

# include <string_view>
# include <unordered_map>
# include <string>
# include <iostream>

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



class Interpreter {

    pugi::xml_document _doc;
    ModelsRegistry _registry;

    static void replace_all(
        std::string& phrase, 
        const std::string& replace_from, 
        const std::string& replace_to
    );

    void transform_glyphs (
        std::string& xml_content,
        const std::unordered_map<std::string, std::string>& glyphs
    );

    void parse(std::string xml_content);
    void interpret();
    
    public:

        Interpreter(std::istream& stream);
        pugi::xml_node doc_root() const;
    
        template<typename Model>
        Model* get_model(const std::string_view xpath) {
            return _registry.get_model<Model>(xpath, doc_root());
        }

        template<typename Model>
        Model* get_model(const pugi::xml_node& node) {
            return _registry.get_model<Model>(node);
        }

};


