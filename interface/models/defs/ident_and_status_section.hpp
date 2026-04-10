# pragma once

# include <json.hpp>

# include "models/base.hpp"




struct IdentAndStatusSection : BaseModel {

    nlohmann::json to_json() const override;

};


