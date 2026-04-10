# pragma once

# include <json.hpp>

# include "models/base.hpp"
# include "models/defs/dm_ref.hpp"
# include "models/defs/pm_ref.hpp"
# include "models/defs/external_pub_ref.hpp"



struct Refs : BaseModel {

    struct RefsChildren {

        /// @p dmRef opt
        std::optional<DmRef> dm_ref;

        /// @p pmRef opt
        std::optional<PmRef> pm_ref;

        /// @p externalPubRef opt
        std::optional<ExternalPubRef> external_pub_ref;

        nlohmann::json to_json() const;

    };

    /// @b occurance: multiple
    std::vector<RefsChildren> children;

    nlohmann::json to_json() const override;

};


