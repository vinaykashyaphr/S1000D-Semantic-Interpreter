# pragma once

# include <string>
# include <optional>
# include <vector>

# include <json.hpp>



struct BaseModel {

    virtual ~BaseModel() = default;

    virtual nlohmann::json to_json() const {

        return {{"type", type}};

    };

    nlohmann::json get_json() const { 

        return to_json(); 

    }

    std::string type;

};



struct RDF_Description : BaseModel {

    nlohmann::json to_json() const override;

};



struct IdentAndStatusSection : BaseModel {

    nlohmann::json to_json() const override;

};



struct DmRef : BaseModel {

};



struct PmRef : BaseModel {

};



struct ExternalPubRef : BaseModel {

};



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

    /// @brief occurance: multiple
    std::vector<RefsChildren> children;

    nlohmann::json to_json() const override;

};



struct Content : BaseModel {

    ///@a id opt
    std::optional<std::string> id;

    struct ContentChildren {

        /// @p refs opt
        Refs* refs = nullptr;

        /// @p referencedApplicGroup opt
        /// @p referencedApplicGroupRef opt
        /// @p warningsAndCautions opt
        /// @p warningsAndCautionsRef opt
        /// @p description req

        nlohmann::json to_json() const;

    };

    /// @brief occurance: single 
    ContentChildren children;

    nlohmann::json to_json() const override;

};



struct Dmodule : BaseModel {

    /// @a id opt
    std::optional<std::string> id;

    struct DmoduleChildren {

        /// @p rdf::Description opt
        RDF_Description* rdf__description = nullptr;

        /// @p identAndStatusSection req
        IdentAndStatusSection* ident_and_status_section = nullptr;

        /// @p content req
        Content* content = nullptr;  

        nlohmann::json to_json() const;
    };

    /// @brief occurance: single
    DmoduleChildren children;

    nlohmann::json to_json() const override;

};


