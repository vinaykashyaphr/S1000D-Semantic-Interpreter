

# include "builders/dm_ref.hpp"
# include "builders_map/builders_map.hpp"



static BuilderRegistrar _reg(
    "dmRef", 
    [](const pugi::xml_node& n, ModelsRegistry& r, std::string_view s) 
    {
        _DmRef(n, r, s);
    }
);



_DmRef::_DmRef(const pugi::xml_node& node, ModelsRegistry& registry, const std::string_view scheme):
    _node(node),
    _registry(registry),
    _scheme(scheme)
{
    current_model = _registry.register_model(_registry.factory().make<DmRef>(), _node);
    build();
}



void _DmRef::build() {
    current_model->type = _node.name();
    resolve();
}



enum class _DmRef::Attrib {

    referredFragment,
    applicRefId,
    id,

    // changeAttGroup
    changeType,
    changeMark,
    reasonForUpdateRefIds,

    // securityAttGroup
    securityClassification,
    commercialClassification,
    ceveat,
    derivativeClassificationRefId,

    // not-implemented
    // authorityAttGroup
    authorityName,
    authorityDocument,

    // xlink:XLINKATT
    xlink__type,
    xlink__href,
    xlink__title,
    xlink__show,
    xlink__actuate,

};



const std::unordered_map<std::string_view, _DmRef::Attrib> _DmRef::ATTRIBS {

    {"referredFragment", Attrib::referredFragment},
    {"applicRefId", Attrib::applicRefId},
    {"id", Attrib::id},
    {"changeType", Attrib::changeType},
    {"changeMark", Attrib::changeMark},
    {"reasonForUpdateRefIds", Attrib::reasonForUpdateRefIds},
    {"securityClassification", Attrib::securityClassification},
    {"commercialClassification", Attrib::commercialClassification},
    {"ceveat", Attrib::ceveat},
    {"derivativeClassificationRefId", Attrib::derivativeClassificationRefId},
    {"authorityName", Attrib::authorityName},
    {"authorityDocument", Attrib::authorityDocument},
    {"xlink:type", Attrib::xlink__type},
    {"xlink:href", Attrib::xlink__href},
    {"xlink:title", Attrib::xlink__title},
    {"xlink:show", Attrib::xlink__show},
    {"xlink:actuate", Attrib::xlink__actuate}

};



void _DmRef::resolve() {

    

}



