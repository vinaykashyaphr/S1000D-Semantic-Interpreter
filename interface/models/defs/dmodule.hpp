# include <optional>


# include "models/base.hpp"
# include "models/defs/rdf__description.hpp"
# include "models/defs/ident_and_status_section.hpp"
# include "models/defs/content.hpp"



struct Dmodule : BaseModel {

    /// @a id opt
    std::optional<std::string> id;

    struct DmoduleChildren {

        /// @p rdf::Description opt
        std::optional<RDF_Description*> rdf__description;

        /// @p identAndStatusSection req
        IdentAndStatusSection* ident_and_status_section = nullptr;

        /// @p content req
        Content* content = nullptr;  

        nlohmann::json to_json() const;
    };

    /// @b occurance: single
    DmoduleChildren children;

    nlohmann::json to_json() const override;

};


