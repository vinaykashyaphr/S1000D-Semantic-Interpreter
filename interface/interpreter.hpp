# pragma once

# include <unordered_map>
# include <string>
# include <iostream>

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



class Interpreter {

    pugi::xml_document _doc;

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
    pugi::xml_node doc_root() const;

    public:
        ModelsRegistry _registry;
        Interpreter(std::istream& stream);

};


