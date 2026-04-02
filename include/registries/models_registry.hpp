# pragma once

# include <memory>
# include <vector>

# include "models.hpp"



class ModelsRegistry {

    public:
        template<typename Model>
        Model* register_model(std::unique_ptr<Model> model_ptr) {
            Model* raw = model_ptr.get();
            models.push_back(std::move(model_ptr));
            return raw;
        }

        template<typename Model>
        Model& get_item(std::size_t index)
        {
            return dynamic_cast<Model&>(*models.at(index));
        }

    private:
        std::vector<std::unique_ptr<BaseModel>> models;

};


