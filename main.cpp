#include "ascii85_decoder.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    bool fullMode = false;

    if (argc > 1 && std::string(argv[1]) == "--help") {
        std::cout << "Usage: " << argv[0] << " [--full]\n";
        return EXIT_SUCCESS;
    }

    if (argc > 1 && std::string(argv[1]) == "--full")
        fullMode = true;

    try {
        if (fullMode) {
            std::string data(std::istreambuf_iterator<char>(std::cin), {});
            auto decoded = ASCII85Decoder::decode(data);
            std::cout.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
        } else {
            ASCII85Decoder::decodeStream(std::cin, std::cout);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}