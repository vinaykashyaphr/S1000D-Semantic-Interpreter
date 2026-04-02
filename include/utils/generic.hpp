# pragma once

# include <string>
# include <string_view>
# include <stdexcept>
# include <unordered_map>



class generic_utils {

    public:

        template <typename ReturnObj>
        static ReturnObj find_in_umap(
            const std::unordered_map<std::string_view, ReturnObj>& map, 
            const std::string_view key, 
            const std::string_view tag
        ) {

            auto it = map.find(key);
            if (it != map.end()) return it->second;

            throw std::runtime_error("Unknown Entity: " + std::string(key) + " : " + std::string(tag));

        };


        template<typename String, std::size_t N>
        requires (std::same_as<String, std::string> || std::same_as<String, std::string_view>)
        static std::string join(const std::array<String, N>&& str_container, char sep) {

            std::string result = "";

            for (std::size_t i = 0; i < N; ++i) {

                result += str_container.at(i);
                if (i < N - 1) result += sep;

            }

            return result;

        }

        static bool is_valid_numeric_type(std::string str, auto& result);
        static std::string normalize_spaces(const std::string_view phrase);

};

