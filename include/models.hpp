# pragma once

# include <string>
# include <optional>
// # include <vector>



struct BaseModel {

    virtual ~BaseModel() = default;

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


