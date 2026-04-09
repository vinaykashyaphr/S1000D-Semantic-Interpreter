# pragma once

# include <unordered_map>
# include <string_view>
# include <functional>
# include <string>

# include <pugixml.hpp>

# include "registries/models_registry.hpp"



using BuilderFn = std::function<void(const pugi::xml_node&, ModelsRegistry&, std::string_view)>;



class BuildersMap {

    std::unordered_map<std::string, BuilderFn> _map;

    public:

        static BuildersMap& instance() {
            static BuildersMap inst;
            return inst;
        }

        void register_builder(std::string_view name, BuilderFn fn) {
            _map.emplace(std::string(name), std::move(fn));
        }

        const BuilderFn* find(std::string_view name) const {
            auto it = _map.find(std::string(name));
            if (it != _map.end()) return &it->second;
            return nullptr;
        }

};



struct BuilderRegistrar {
    BuilderRegistrar(std::string_view name, BuilderFn fn) {
        BuildersMap::instance().register_builder(name, std::move(fn));
    }
};


