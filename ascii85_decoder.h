#ifndef ASCII85_DECODER_H
#define ASCII85_DECODER_H
#include <vector>
#include <string>
#include <iostream>
#include <cstdint>

class ASCII85Decoder {
public:
    static std::vector<uint8_t> decode(const std::string& input);
    static void decodeStream(std::istream& in, std::ostream& out);

private:
    static void processGroup(uint32_t group, int count, std::vector<uint8_t>& output);
};

#endif