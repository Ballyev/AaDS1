#include "ascii85_decoder.h"
#include <stdexcept>
#include <cctype>
#include <vector>

std::string ASCII85Decoder::encode(const std::vector<uint8_t>& data) {
    std::string output;
    size_t i = 0;

    while (i + 4 <= data.size()) {
        uint32_t group = 0;
        for (int j = 0; j < 4; ++j)
            group = (group << 8) | data[i + j];

        if (group == 0) {
            output += 'z';
        } else {
            char encoded[5];
            for (int j = 4; j >= 0; --j) {
                encoded[j] = (group % 85) + '!';
                group /= 85;
            }
            output.append(encoded, 5);
        }

        i += 4;
    }

    if (i < data.size()) {
        uint32_t group = 0;
        int num_bytes = 0;

        for (; i < data.size(); ++i, ++num_bytes)
            group = (group << 8) | data[i];

        int pad_bytes = 4 - num_bytes;
        for (int j = 0; j < pad_bytes; ++j)
            group <<= 8;

        char encoded[5];
        for (int j = 4; j >= 0; --j) {
            encoded[j] = (group % 85) + '!';
            group /= 85;
        }

        output.append(encoded, 5 - pad_bytes);
    }

    return output;
}

std::vector<uint8_t> ASCII85Decoder::decode(const std::string& input) {
    std::vector<uint8_t> output;
    uint32_t group = 0;
    int count = 0;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (isspace(c)) continue;

        if (c == 'z') {
            if (count != 0) {
                throw std::runtime_error("'z' character in middle of group");
            }
            output.insert(output.end(), 4, 0);
            continue;
        }

        if (c < '!' || c > 'u') {
            throw std::runtime_error(std::string("Invalid ASCII85 character: ") + c);
        }

        group = group * 85 + (c - '!');
        count++;

        if (count == 5) {
            output.push_back((group >> 24) & 0xFF);
            output.push_back((group >> 16) & 0xFF);
            output.push_back((group >> 8) & 0xFF);
            output.push_back(group & 0xFF);
            group = 0;
            count = 0;
        }
    }


    if (count > 1) {
        while (count < 5) {
            group = group * 85 + 84;
            count++;
        }


        output.push_back((group >> 24) & 0xFF);
        if (count > 2) output.push_back((group >> 16) & 0xFF);
        if (count > 3) output.push_back((group >> 8) & 0xFF);
    }
    else if (count == 1) {
        throw std::runtime_error("Incomplete ASCII85 group (1 character)");
    }

    return output;
}

void ASCII85Decoder::decodeStream(std::istream& in, std::ostream& out) {
    std::string buffer;
    bool inBlock = false;

    char c;
    while (in.get(c)) {
        if (!inBlock) {
            if (c == '<' && in.peek() == '~') {
                in.get();
                inBlock = true;
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
        try {
            auto decoded = decode(buffer);
            out.write(reinterpret_cast<const char*>(decoded.data()), decoded.size());
        } catch (const std::exception& e) {
            throw std::runtime_error(std::string("Decoding failed: ") + e.what());
        }
    }
}