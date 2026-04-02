# include <iostream>
# include <filesystem>
# include <fstream>

# include "interpreter.hpp"


namespace fs = std::filesystem;


int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.xml>\n";
        return 1;
    }

    fs::path input  = argv[1];
    std::ifstream in_file(input.string());

    Interpreter interpreter(in_file);

    Dmodule& dmodule = interpreter._registry.get_item<Dmodule>(0);

    std::cout << (dmodule.id.has_value() ? dmodule.id.value() : "none") << " " << dmodule.type << '\n';

    return 0;

}

