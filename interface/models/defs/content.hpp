# pragma once

# include <json.hpp>

# include "models/base.hpp"
# include "models/defs/refs.hpp"



struct Content : BaseModel {

    ///@a id opt
    std::optional<std::string> id;

    struct ContentChildren {

        /// @p refs opt
        std::optional<Refs*> refs;

        /// @p referencedApplicGroup opt
        /// @p referencedApplicGroupRef opt
        /// @p warningsAndCautions opt
        /// @p warningsAndCautionsRef opt
        /// @p description req

        nlohmann::json to_json() const;

    };

    /// @b occurance: single 
    ContentChildren children;

    nlohmann::json to_json() const override;

};


