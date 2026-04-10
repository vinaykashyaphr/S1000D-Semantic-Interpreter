# pragma once

# include <json.hpp>

# include "models/base.hpp"




struct RDF_Description : BaseModel {

    nlohmann::json to_json() const override;

};


