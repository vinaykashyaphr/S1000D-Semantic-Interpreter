# include "utils/s1000d.hpp"
# include "utils/generic.hpp"
# include "utils/static_maps.hpp"


std::optional<std::pair<std::string, std::string>> s1000d_utils::get_language(const std::string& liso, const std::string& ciso) {

    auto l_it = LANGUAGE_MAP.find(liso);
    auto c_it = COUNTRY_MAP.find(ciso);

    if ((l_it != LANGUAGE_MAP.end()) && (c_it != COUNTRY_MAP.end())) 
        return std::pair{l_it->second, c_it->second};

    return std::nullopt;

}



std::string s1000d_utils::get_text(const pugi::xml_node& node) {

    using nt = pugi::xml_node_type;

    if (node.type() != nt::node_element) {
        throw std::invalid_argument(std::string(node.name()) + " is not an element!");
    }

    std::string text = "";

    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {

        if (child.type() == nt::node_pcdata || child.type() == nt::node_cdata) 
            text += child.text().get();

        else if (child.type() == nt::node_pi || child.type() == nt::node_comment)
            continue;

        else break;

    }


    return generic_utils::normalize_spaces(text);

}



std::string s1000d_utils::get_tail(const pugi::xml_node& node) {

    using nt = pugi::xml_node_type;

    if (node.type() != nt::node_element) {
        throw std::invalid_argument(std::string(node.name()) + " is not an element!");
    }

    auto is_text= [](const pugi::xml_node& cn) -> bool {
        return cn.type() == nt::node_pcdata || cn.type() == nt::node_cdata;
    };

    auto is_ignorable = [](const pugi::xml_node& cn) -> bool {
        return cn.type() == nt::node_comment || cn.type() == nt::node_pi;
    };

    std::string tail = "";

    for (
        pugi::xml_node cn = node.next_sibling(); 
        (cn && (is_text(cn) || is_ignorable(cn))); 
        cn = cn.next_sibling()
    ) {

        if (is_text(cn)) tail += cn.text().get();

    }

    return generic_utils::normalize_spaces(tail);
}


