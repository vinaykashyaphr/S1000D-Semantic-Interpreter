# pragma once

# include <type_traits>
# include <typeindex>
# include <functional>
# include <unordered_map>
# include <memory>

# include "models/base.hpp"




class ModelsFactory {

    std::unordered_map<std::type_index, std::function<BaseModel*()>> _overrides;

    public:

        template<typename Model>
        std::unique_ptr<Model> make() {

            auto it = _overrides.find(typeid(Model));

            if (it != _overrides.end()) 
                return std::unique_ptr<Model>(static_cast<Model*>(it->second()));

            return std::make_unique<Model>();

        }


        template<typename Base, typename Derived>
        void override_with() {

            static_assert(std::is_base_of_v<Base, Derived>);

            _overrides[typeid(Base)] = []() -> Base* {

                return new Derived();

            };

        }


};


