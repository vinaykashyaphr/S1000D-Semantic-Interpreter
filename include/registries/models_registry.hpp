# pragma once

# include <memory>
# include <vector>
# include <unordered_map>

# include <pugixml.hpp>

# include "models.hpp"


class ModelsRegistry {

    public:
        template<typename Model>
        Model* register_model(std::unique_ptr<Model> model_ptr, const pugi::xml_node& node) {
            Model* raw = model_ptr.get();
            models.push_back(std::move(model_ptr));
            node_map[node.internal_object()] = raw;
            return raw;
        }

        template<typename Model>
        Model& get_item(std::size_t index)
        {
            return dynamic_cast<Model&>(*models.at(index));
        }

        BaseModel* get_by_node(const pugi::xml_node& node) {
            auto it = node_map.find(node.internal_object());
            if (it != node_map.end()) return it->second;
            return nullptr;
        }

    private:
        std::vector<std::unique_ptr<BaseModel>> models;
        std::unordered_map<void*, BaseModel*> node_map;

};


