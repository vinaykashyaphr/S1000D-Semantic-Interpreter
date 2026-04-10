# pragma once

# include <string_view>
# include <iostream>
# include <memory>

# include "models/base.hpp"
# include "models/factory.hpp"



class Interpreter {

    struct Impl;
    std::unique_ptr<Impl> _impl;

    BaseModel* get_model_base(std::string_view xpath);

    public:

        explicit Interpreter(std::istream& stream, ModelsFactory* factory = nullptr);
        ~Interpreter();

        template<typename Model>
        Model* get_model(std::string_view xpath) {
            return dynamic_cast<Model*>(get_model_base(xpath));
        }
    
};


