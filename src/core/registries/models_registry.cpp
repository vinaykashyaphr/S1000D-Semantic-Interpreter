# include "registries/models_registry.hpp"




void ModelsRegistry::defer_link(std::function<void()> fn) {
    _pending_links.push_back(std::move(fn));
}



void ModelsRegistry::resolve_links() {
    for (auto& fn : _pending_links) fn();
    _pending_links.clear();
}


