# include <charconv>

# include "utils/generic.hpp"




bool generic_utils::is_valid_numeric_type(std::string str, auto& result) {

    auto [ptr, error_code] = std::from_chars(str.data(), str.data() + str.size(), result);

    return (error_code == std::errc()) && (ptr == str.data() + str.size());

}



std::string generic_utils::normalize_spaces(const std::string_view phrase) {

    std::string result;
    result.reserve(phrase.size());
    bool in_ws = false;

    for (unsigned char ch : phrase) {

        if (std::isspace(ch)) {

            if (!in_ws) {
                result.push_back(' ');
                in_ws = true;
            }

        }

        else {
            result.push_back(ch);
            in_ws = false;
        }

    }

    return result;

}

