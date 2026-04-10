# include <iostream>
# include <filesystem>
# include <fstream>

# include "interpreter.hpp"
# include "models/defs/content.hpp"
# include "models/defs/dmodule.hpp"
# include "models/factory.hpp"


namespace fs = std::filesystem;


struct MyContent : Content {
    nlohmann::json to_json() const override {
        nlohmann::json j = Content::to_json();  // get default structure

        // modify
        j["custom_id"] = id.value_or("none");

        // delete
        j.erase("id");

        // add
        j["source"] = "s1000d";

        return j;
    }
};




int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.xml>\n";
        return 1;
    }

    fs::path input  = argv[1];
    std::ifstream in_file(input.string());

    ModelsFactory factory;
    factory.override_with<Content, MyContent>();

    Interpreter interpreter(in_file, &factory);

    Dmodule* dmodule = interpreter.get_model<Dmodule>(".");

    if (!dmodule) return 1;

    std::cout << (dmodule->id.has_value() ? dmodule->id.value() : "none") << " " << dmodule->type << '\n';

    std::cout << dmodule->to_json().dump(4) << '\n';
    // std::cout << (dmodule.children.content->id.has_value() ? dmodule.children.content->id.value() : "none") << " " << dmodule.children.content->type << '\n';

    return 0;

}


