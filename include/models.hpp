# pragma once

# include <string>
# include <optional>



struct BaseModel {

    virtual ~BaseModel() = default;

    std::string type;

};



struct RDF_Description : BaseModel {

};



struct IdentAndStatusSection : BaseModel {


};



struct Content : BaseModel {

    std::optional<std::string> id;
};



struct Dmodule : BaseModel {

    std::optional<std::string> id;

    struct DmoduleChildren {
        std::optional<RDF_Description>* rdf_description = nullptr;
        IdentAndStatusSection* ident_and_status_section = nullptr;
        Content* content = nullptr;  
    };

    DmoduleChildren children;

};


