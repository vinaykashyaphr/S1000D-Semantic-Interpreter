# include "interpreter.hpp"
# include "utils/static_maps.hpp"
# include "navigators/node_streamer.hpp"


Interpreter::Interpreter(std::istream& xml_stream) {

    std::istreambuf_iterator<char> begin(xml_stream);
    std::istreambuf_iterator<char> end;

    std::string xml_content(begin, end);

    transform_glyphs(xml_content, GLYPHS_MAP);
    parse(xml_content);
    interpret();

}



void Interpreter::replace_all(
    std::string& phrase, const std::string& replace_from, const std::string& replace_to
) {

    if (replace_from.empty()) return;

    size_t pos = 0;

    while ((pos = phrase.find(replace_from, pos)) != std::string::npos) {

        phrase.replace(pos, replace_from.length(), replace_to);
        pos += replace_to.length();

    }

}



void Interpreter::transform_glyphs(std::string& xml_content, const std::unordered_map<std::string, std::string>& glyphs ) {
    
    for (const auto& [key, value] : glyphs) {
        replace_all(xml_content, key, value);
    }

}



void Interpreter::parse(std::string xml_content) {

    pugi::xml_parse_result result = _doc.load_string(xml_content.data());

    if (!result) {

        std::cerr << "XML parse failed." << '\n';
        std::cerr << "Status type: " << result.status << '\n';
        std::cerr << "Offset: " << result.offset << '\n';

        throw std::runtime_error(result.description());

    }

}



pugi::xml_node Interpreter::doc_root() const {
    return _doc.document_element();
}



void Interpreter::interpret() {

    const pugi::xml_node& node = doc_root();
    NodeStreamer streamer(node, _registry);
    streamer.stream();

}





