# include "registries/models_registry.hpp"



ModelsRegistry::ModelsRegistry(ModelsFactory* factory): 
    _factory(factory ? factory : &_default_factory) {

}



ModelsFactory& ModelsRegistry::factory() {

    return *_factory;

}



void ModelsRegistry::defer_link(std::function<void()> fn) {
    _pending_links.push_back(std::move(fn));
}



void ModelsRegistry::resolve_links() {
    for (auto& fn : _pending_links) fn();
    _pending_links.clear();
}


