# pragma once

# include <json.hpp>



class BaseModel {

    public:

        virtual ~BaseModel() = default;

        virtual nlohmann::json to_json() const {

            return {{"type", type}};

        };

        std::string type;

};


