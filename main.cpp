#include "ascii85_decoder.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    bool fullMode = false;
    bool encodeMode = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [--full] [--encode]\n";
            return EXIT_SUCCESS;
        } else if (arg == "--full") {
            fullMode = true;
        } else if (arg == "--encode") {
            encodeMode = true;
        }
    }

    try {
        if (encodeMode) {
            std::string data(std::istreambuf_iterator<char>(std::cin), {});
            auto encoded = ASCII85Decoder::encode(
                std::vector<uint8_t>(data.begin(), data.end())
            );
            std::cout << encoded;
        } else {
            if (fullMode) {
                std::string data(std::istreambuf_iterator<char>(std::cin), {});
                auto decoded = ASCII85Decoder::decode(data);
                std::cout.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
            } else {
                ASCII85Decoder::decodeStream(std::cin, std::cout);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}