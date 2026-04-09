# pragma once

# include <string>
# include <optional>
// # include <vector>

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

};



struct IdentAndStatusSection : BaseModel {


};



struct Refs {

    std::string type;

    // struct RefsChild {

    //     std::optional<DmRef> dm_ref;
    //     std::optional<PmRef> pm_ref;
    //     std::optional<ExternalPubRef> external_pub_ref;

    // };

    // std::vector<RefsChild> children;

};




struct Content : BaseModel {

    std::optional<std::string> id;
};



struct Dmodule : BaseModel {

    std::optional<std::string> id;

    struct DmoduleChildren {
        std::optional<RDF_Description>* rdf__description = nullptr;
        IdentAndStatusSection* ident_and_status_section = nullptr;
        Content* content = nullptr;  
    };

    DmoduleChildren children;

};


