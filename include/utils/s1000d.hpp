# pragma once

# include <utility>
# include <string>
# include <optional>

# include <pugixml.hpp>



class s1000d_utils {

    public:
        std::optional<std::pair<std::string, std::string>> get_language(const std::string& liso, const std::string& ciso);
        std::string get_text(const pugi::xml_node& node);
        std::string get_tail(const pugi::xml_node& node);

};


