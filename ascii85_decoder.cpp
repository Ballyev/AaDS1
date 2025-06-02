#include "ascii85_decoder.h"
#include <stdexcept>
#include <cctype>
#include <vector>

std::vector<uint8_t> ASCII85Decoder::decode(const std::string& input) {
    std::vector<uint8_t> output;
    uint32_t group = 0;
    int count = 0;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (isspace(c)) continue;

        if (c == 'z') {
            output.insert(output.end(), 4, 0);
            count = 0;
            continue;
        }

        if (c < '!' || c > 'u')
            throw std::runtime_error(std::string("Invalid ASCII85 character: ") + c);

        group = group * 85 + (c - '!');
        count++;

        if (count == 5) {
            for (int j = 3; j >= 0; --j)
                output.push_back((group >> (j * 8)) & 0xFF);
            group = 0;
            count = 0;
        }
    }

    if (count > 0) {
        if (count < 2)
            throw std::runtime_error("Incomplete ASCII85 group");

        for (int j = count; j < 5; ++j)
            group = group * 85 + 84;

        for (int j = 3; j >= 0 && count + j > 3; --j)
            output.push_back((group >> (j * 8)) & 0xFF);
    }

    return output;
}

void ASCII85Decoder::decodeStream(std::istream& in, std::ostream& out) {
    std::string buffer;
    bool inBlock = false;

    char c;
    while (in.get(c)) {
        if (!inBlock) {
            if (c == '<') {
                if (in.peek() == '~') {
                    in.get();
                    inBlock = true;
                }
            }
        } else {
            if (c == '~' && in.peek() == '>') {
                in.get();
                break;
            }
            buffer += c;
        }
    }

    if (inBlock) {
        auto decoded = decode(buffer);
        out.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
    }
}