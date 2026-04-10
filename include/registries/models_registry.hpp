# pragma once

# include <functional>
# include <memory>
# include <vector>
# include <unordered_map>

# include <pugixml.hpp>

# include "models/base.hpp"
# include "models/factory.hpp"




class ModelsRegistry {

    std::vector<std::unique_ptr<BaseModel>> models;
    std::unordered_map<void*, BaseModel*> node_map;
    std::vector<std::function<void()>> _pending_links;

    ModelsFactory* _factory;
    ModelsFactory _default_factory;

    public:

        ModelsFactory& factory();
        explicit ModelsRegistry(ModelsFactory* factory = nullptr);

        template<typename Model>
        Model* register_model(std::unique_ptr<Model> model_ptr, const pugi::xml_node& node) {
            Model* raw = model_ptr.get();
            models.push_back(std::move(model_ptr));
            node_map[node.internal_object()] = raw;
            return raw;
        }

        template<typename Model>
        Model* get_model(const std::string_view xpath, const pugi::xml_node& root) {

            try {

                pugi::xpath_node xnode = root.select_node(xpath.data());

                if (!xnode) return nullptr;
                auto it = node_map.find(xnode.node().internal_object());

                if (it != node_map.end()) return dynamic_cast<Model*>(it->second);
                return nullptr;
            }

            catch (const pugi::xpath_exception& e) {
                return nullptr;
            }

        }

        template<typename Model>
        Model* get_model(const pugi::xml_node& node) {

            auto it = node_map.find(node.internal_object());
            if (it != node_map.end()) return dynamic_cast<Model*>(it->second);
            return nullptr;

        }

        void defer_link(std::function<void()> fn);
        void resolve_links();

};


